#include "Framework.h"
#include "ModelRender.h"

ModelRender::ModelRender(Shader * shader)
	: shader(shader)
{
	model = new Model();
	
	//이 초기화하는 부분있지 이거 모델 애니메이터에 넣어줘
	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		D3DXMatrixIdentity(&worlds[i]);
	//////////////////////////////////////////
	instanceBuffer = new VertexBuffer(worlds, MAX_MODEL_INSTANCE, sizeof(Matrix), 1, true);


	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		colors[i] = Color(1, 1, 1, 1);
	instanceColorBuffer = new VertexBuffer(colors, MAX_MODEL_INSTANCE, sizeof(Color), 2);
	//이거 넣어준다. 컬러버퍼
}

ModelRender::~ModelRender()
{
	SafeDelete(model);
	
	for (Transform* transform : transforms)
		SafeDelete(transform);

	SafeDelete(instanceBuffer);
	SafeDelete(instanceColorBuffer);
	SafeRelease(texture);
	SafeRelease(transformsSRV);
}

void ModelRender::Update()
{
	if (texture == NULL)
	{
		for (ModelMesh* mesh : model->Meshes())
			mesh->SetShader(shader);

		CreateTexture();
	}

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelRender::Render()
{
	instanceBuffer->Render();
	instanceColorBuffer->Render();

	for (ModelMesh* mesh : model->Meshes())
		mesh->Render(transforms.size());
}

void ModelRender::ReadMesh(wstring file)
{
	model->ReadMesh(file);
}

void ModelRender::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
}

Transform * ModelRender::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	return transform;
}

void ModelRender::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
		memcpy(worlds[i], transforms[i]->World(), sizeof(Matrix));

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MAX_MODEL_INSTANCE);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
}

void ModelRender::Pass(UINT pass)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

void ModelRender::UpdateBoneTransform(UINT instance, UINT boneIndex, Transform * transform)
{	//인스턴스는 그냥 넘겨주고 본인덱스 넣어서 그대로 콜해주면 돼
	UpdateBoneTransform(instance, model->BoneByIndex(boneIndex), transform);
}

void ModelRender::UpdateBoneTransform(UINT instance, ModelBone * bone, Transform * transform)
{	//우리가수정할 월드 가져와
	Matrix destMatrix = transform->World();
	boneTransforms[instance][bone->Index()] = destMatrix * boneTransforms[instance][bone->Index()];
	//트랜스폼에서 transform srt넣어서 주면 자식들쫙 적용시키겠다는 이야기야.
	//몇번본을 받을거니까. 본의 인덱스 / 지가 루트가 되는거니까 지것만 곱해주면 되잖아.
	for (ModelBone* child : bone->Childs()) // 애부터 자식으로 쭉 내려가면 되지
	{
		Matrix parent = boneTransforms[instance][bone->Index()];

		Matrix invParent;
		D3DXMatrixInverse(&invParent, NULL, &parent);
		//자식이 바로 다음본 아닐수 있잖아.
		Matrix temp = boneTransforms[instance][child->Index()] * invParent;
		boneTransforms[instance][child->Index()] = temp * destMatrix * parent;
		//재귀
		UpdateBoneTransform(instance, child, transform);
	}
}

void ModelRender::UpdateBoneTransform()
{
	//어떤본을 바꿀지 한가지가 더 들어가야지

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, boneTransforms, MAX_MODEL_INSTANCE * MAX_MODEL_TRANSFORMS * sizeof(Matrix));
	}
	D3D::GetDC()->Unmap(texture, 0);
}

void ModelRender::CreateTexture()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = MAX_MODEL_TRANSFORMS * 4; //왜 곱하기 4하는지 알지? 매트릭스 니까
	desc.Height = MAX_MODEL_INSTANCE; //y를 인스턴트
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.Usage = D3D11_USAGE_DYNAMIC; //회전같은거 할라면 다시 써야하잖아. 그래서 다이나믹으로 놓은거야.
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.ArraySize = 1; //어레이 사이즈 한장
	desc.SampleDesc.Count = 1;

	Matrix bones[MAX_MODEL_TRANSFORMS];
	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{
			ModelBone* bone = model->BoneByIndex(b);

			Matrix parent;
			int parentIndex = bone->ParentIndex();

			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];

			Matrix matrix = bone->Transform();
			bones[b] = parent;
			boneTransforms[i][b] = matrix * bones[b];
		}//for(b)
	}//for(i)

	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = boneTransforms;
	//한줄의 사이즈
	subResource.SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
	//한 면의 사이즈
	subResource.SysMemSlicePitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix) * MAX_MODEL_INSTANCE;

	Check(D3D::GetDevice()->CreateTexture2D(&desc, &subResource, &texture));

	//쉐이더로 넘겨줘야하잖아- 쉐이더 리소스 뷰 있어야겠지
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Format = desc.Format;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &transformsSRV));

	//전체 메시를 트랜스폼srv로 넘겨줄거야.
	for (ModelMesh* mesh : model->Meshes())
		mesh->TransformsSRV(transformsSRV);
}
