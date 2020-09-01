#include "Framework.h"
#include "Particle.h"
#include "Utilities/Xml.h"

Particle::Particle(wstring file)
	: Renderer(L"38_Particle.fxo")
{
	ReadFile(L"../../_Textures/Particles/" + file + L".xml");

	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	sBuffer = shader->AsConstantBuffer("CB_Particle");

	sMap = shader->AsSRV("ParticleMap");

	Reset();
}

Particle::~Particle()
{
	SafeDelete(buffer);
	SafeDelete(map);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);
}

void Particle::Reset()
{
	currentTime = 0.0f;
	lastAddTime = Time::Get()->Running();
	leadCount = gpuCount = activeCount = deactiveCount = 0;


	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);


	vertices = new VertexParticle[data.MaxParticles * 4];
	for (UINT i = 0; i < data.MaxParticles; i++)
	{
		vertices[i * 4 + 0].Corner = Vector2(-1, -1);
		vertices[i * 4 + 1].Corner = Vector2(-1, +1);
		vertices[i * 4 + 2].Corner = Vector2(+1, -1);
		vertices[i * 4 + 3].Corner = Vector2(+1, +1);
	}

	indices = new UINT[data.MaxParticles * 6];
	for (UINT i = 0; i < data.MaxParticles; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 1;
		indices[i * 6 + 5] = i * 4 + 3;
	}

	vertexBuffer = new VertexBuffer(vertices, data.MaxParticles * 4, sizeof(VertexParticle), 0, true);
	indexBuffer = new IndexBuffer(indices, data.MaxParticles * 6);
}

void Particle::Add(Vector3 & position)
{
	if (Time::Get()->Running() - lastAddTime < 60 / 1000.0f)
		return;

	
	lastAddTime = Time::Get()->Running();

	UINT next = leadCount + 1;

	if (next >= data.MaxParticles)
	{
		if (data.bLoop == true)
			next = 0;
		else
		{
			next = data.MaxParticles;// 꽉찼다.
			return; //추가 안하면 돼 //한번만 할거면
		}
	}

	if (next == deactiveCount)
		return;


	Vector3 velocity = Vector3(1, 1, 1);
	velocity *= data.StartVelocity;

	float horizontalVelocity = Math::Lerp<float>(data.MinHorizontalVelocity, data.MaxHorizontalVelocity, Math::Random(0.0f, 1.0f));
	float horizontalAngle = Math::PI * 2.0f * Math::Random(0.0f, 1.0f);

	//x방향쪽으로 갈려면 대각선 속도 * cos 각도 , y방향은 sin각도
	velocity.x += horizontalVelocity * cos(horizontalAngle);
	velocity.y += horizontalVelocity * sin(horizontalAngle);
	velocity.z += Math::Lerp<float>(data.MinVerticalVelocity, data.MaxVerticalVelocity, Math::Random(0.0f, 1.0f));
	//새로 만든다.
	Vector4 random = Math::RandomColor4();

	//leadCount 면 하나당당 점 4개니까 (정점 4개는 값이 모두 같다. uv만 달라질뿐)
	for (UINT i = 0; i < 4; i++)
	{
		vertices[leadCount * 4 + i].Position = position;
		vertices[leadCount * 4 + i].Velocity = velocity;
		vertices[leadCount * 4 + i].Random = random;
		vertices[leadCount * 4 + i].Time = currentTime; //현재시간 (최초로 만들어진시간)	
	}
	//다음에 해당위치에다가 또 추가할수 있게 해줘야겠지
	leadCount = next;
}

void Particle::Update()
{
	Super::Update();

	currentTime += Time::Delta();

	MapVertices();
	Activate();
	Deactivate();
	//다 따라온 시간 그러면 0으로 보내준다. 첨부터 다시시작하려고
	if (activeCount == leadCount)
		currentTime = 0.0f;


	desc.MinColor = data.MinColor;
	desc.MaxColor = data.MaxColor;

	desc.Gravity = data.Gravity;
	desc.EndVelocity = data.EndVelocity;

	desc.RotateSpeed = Vector2(data.MinRotateSpeed, data.MaxRotateSpeed);
	desc.StartSize = Vector2(data.MinStartSize, data.MaxStartSize);
	desc.EndSize = Vector2(data.MinEndSize, data.MaxEndSize);

	desc.ReadyTime = data.ReadyTime;
	desc.ReadyRandomTime = data.ReadyRandomTime;
}

void Particle::MapVertices()
{
	if (leadCount == gpuCount) return;
	//이러면 복사할게 없다.

	D3D11_MAPPED_SUBRESOURCE subResource;
	//복사할게 있다면 복사에 들어가야하지

	if (leadCount > gpuCount)
	{	//맵을 걸어줘, 한번썼자나 그럼 쓴데까지 거기서부터 다시쓰는거야, 덮어씌우질 않을뿐이야. 지우지 않은상태에서 
		D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subResource);
		{
			//geometryCount는 하나가지고 만들수 있어서 *4가 빠져, 이거는 정점 4개니까 4
			UINT start = gpuCount * 4;
			UINT size = (leadCount - gpuCount) * sizeof(VertexParticle) * 4; //정점의 크기의 *4
			UINT offset = gpuCount * sizeof(VertexParticle) * 4;
			//시작바이트이다.
	 		BYTE* p = (BYTE *)subResource.pData + offset; //시작주소를 계산해보자
			//gpu쪽 주소이다.
			memcpy(p, vertices + start, size); //cpu쪽에서 여기까지 온거지 , cpu의 차만큼 gpu에다가 복사해주는거야.
			//거기에서 스타트 만큼 갈것이다.
		}
		//이 아래쪽에 있어야 한다. 없으면 에러날수도 있다.
		D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
	}
	else
	{
		D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subResource);
		{
			UINT start = gpuCount * 4;
			UINT size = (data.MaxParticles - gpuCount) * sizeof(VertexParticle) * 4;
			UINT offset = gpuCount * sizeof(VertexParticle) * 4;

			BYTE* p = (BYTE *)subResource.pData + offset;
			memcpy(p, vertices + start, size);
		}

		if (leadCount > 0)
		{
			UINT size = leadCount * sizeof(VertexParticle) * 4;

			memcpy(subResource.pData, vertices, size);
		}
		D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
	}
	//마무리로 이걸로 넣어준다. gpu가 lead로 따라 와야하잖아. 
	gpuCount = leadCount;
	//그다음에 active 따라가고 deactive 따라가야지
}
//에디터는 노가다야
void Particle::Activate()
{	
	//gpucount가지고 그려야지, leadcount가 아니라. active가 쫒아가잖아.
	//시간의 간격이 생겨서 어색한게 생겨 leadCount가.
	while (activeCount != gpuCount)
	{
		float age = currentTime - vertices[activeCount * 4].Time;
		if (age < data.ReadyTime)
			return;

		vertices[activeCount * 4].Time = currentTime;
		activeCount++;

		if (activeCount >= data.MaxParticles)
			activeCount = (data.bLoop == true)? 0 : data.MaxParticles;
	}
}

void Particle::Deactivate()
{
	while (deactiveCount != activeCount)
	{
		float age = currentTime - vertices[activeCount * 4].Time;
		if (age > data.ReadyTime)
			return;

		deactiveCount++;

		if (deactiveCount >= data.MaxParticles)
			deactiveCount = (data.bLoop == true)? true: data.MaxParticles;
	}
}//파티클은 객체이다. 하나 인터페이스로 해서 만들어야 한다.


void Particle::Render()
{
	Super::Render();


	desc.CurrentTime = currentTime;
	
	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	sMap->SetResource(map->SRV());

	UINT pass = (UINT)data.Type;
	if (activeCount != leadCount)
	{
		if (leadCount > activeCount)
		{
			shader->DrawIndexed(0, pass, (leadCount - activeCount) * 6, activeCount * 6);
		}
		else
		{
			shader->DrawIndexed(0, pass, (data.MaxParticles - activeCount) * 6, activeCount * 6);

			if (leadCount > 0)
				shader->DrawIndexed(0, pass, leadCount * 6);
		}
	}
}

void Particle::ReadFile(wstring file)
{
	Xml::XMLDocument* document = new Xml::XMLDocument();
	Xml::XMLError error = document->LoadFile(String::ToString(file).c_str());
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();

	Xml::XMLElement* node = root->FirstChildElement();
	data.Type = (ParticleData::BlendType)node->IntText();

	node = node->NextSiblingElement();
	data.bLoop = node->BoolText();

	node = node->NextSiblingElement();
	wstring textureFile = String::ToWString(node->GetText());
	data.TextureFile = L"Particles/" + textureFile;
	map = new Texture(data.TextureFile);


	node = node->NextSiblingElement();
	data.MaxParticles = node->IntText();

	node = node->NextSiblingElement();
	data.ReadyTime = node->FloatText();

	node = node->NextSiblingElement();
	data.ReadyRandomTime = node->FloatText();

	node = node->NextSiblingElement();
	data.StartVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.EndVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.MinHorizontalVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.MaxHorizontalVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.MinVerticalVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.MaxVerticalVelocity = node->FloatText();

	node = node->NextSiblingElement();
	data.Gravity.x = node->FloatAttribute("X");
	data.Gravity.y = node->FloatAttribute("Y");
	data.Gravity.z = node->FloatAttribute("Z");

	node = node->NextSiblingElement();
	data.MinColor.r = node->FloatAttribute("R");
	data.MinColor.g = node->FloatAttribute("G");
	data.MinColor.b = node->FloatAttribute("B");
	data.MinColor.a = node->FloatAttribute("A");

	node = node->NextSiblingElement();
	data.MaxColor.r = node->FloatAttribute("R");
	data.MaxColor.g = node->FloatAttribute("G");
	data.MaxColor.b = node->FloatAttribute("B");
	data.MaxColor.a = node->FloatAttribute("A");

	node = node->NextSiblingElement();
	data.MinRotateSpeed = node->FloatText();

	node = node->NextSiblingElement();
	data.MaxRotateSpeed = node->FloatText();

	node = node->NextSiblingElement();
	data.MinStartSize = node->FloatText();

	node = node->NextSiblingElement();
	data.MaxStartSize = node->FloatText();

	node = node->NextSiblingElement();
	data.MinEndSize = node->FloatText();

	node = node->NextSiblingElement();
	data.MaxEndSize = node->FloatText();

	SafeDelete(document);
}

void Particle::SetTexture(wstring file)
{
	SafeDelete(map);

	map = new Texture(file);
}
