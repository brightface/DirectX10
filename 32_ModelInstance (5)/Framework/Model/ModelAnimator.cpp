#include "Framework.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(Shader * shader)
	: shader(shader)
{
	model = new Model();

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		D3DXMatrixIdentity(&worlds[i]);

	instanceBuffer = new VertexBuffer(worlds, MAX_MODEL_INSTANCE, sizeof(Matrix), 1, true);
	//어제 했던거랑 똑같다. 인스턴스 버퍼 생성하는거 (메시할때랑 같다)
	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		colors[i] = Color(1, 1, 1, 1); //컬러값 다 똑같이 넣어준다.
	instanceColorBuffer = new VertexBuffer(colors, MAX_MODEL_INSTANCE, sizeof(Color), 2);


	frameBuffer = new ConstantBuffer(&tweenDesc, sizeof(TweenDesc) * MAX_MODEL_INSTANCE);
	blendBuffer = new ConstantBuffer(&blendDesc, sizeof(BlendDesc) * MAX_MODEL_INSTANCE);

	//Create ComputeShader
	{
		computeShader = new Shader(L"25_GetBones.fxo");

		sComputeWorld = computeShader->AsMatrix("World");

		sComputeFrameBuffer = computeShader->AsConstantBuffer("CB_AnimationFrame");
		sComputeBlendBuffer = computeShader->AsConstantBuffer("CB_BlendFrame");
		sComputeTransformsSRV = computeShader->AsSRV("TransformsMap");

		
		//computeBoneBuffer = new StructuredBuffer(NULL, sizeof(Matrix), MAX_MODEL_TRANSFORMS, sizeof(Matrix), MAX_MODEL_TRANSFORMS);
		computeBoneBuffer = new StructuredBuffer(NULL, sizeof(Matrix), MAX_MODEL_TRANSFORMS);
		sComputeInputBoneBuffer = computeShader->AsSRV("InputBones");

		//computeWorldBuffer = new StructuredBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE, sizeof(Matrix), MAX_MODEL_INSTANCE);
		computeWorldBuffer = new StructuredBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE);
		sComputeInputWorldBuffer = computeShader->AsSRV("InputWorlds");

		//텍스처로 리턴받으니 텍스처를 만들어줄거야.
		//인스턴스별로 높이가 달라져 텍스처, 다 따로 계산해야되는거야 . 인스턴스별로 다 프레임이 달라져
		ID3D11Texture2D* texture;
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_INSTANCE;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		Check(D3D::GetDevice()->CreateTexture2D(&desc, NULL, &texture));

		computeOutputBuffer = new TextureBuffer(texture);
		sComputeOutputBuffer = computeShader->AsUAV("Output");
		//UAV : UNOREDRED ACCES VIABLE 인가.
		SafeRelease(texture);


		attachBones = new Matrix*[MAX_MODEL_INSTANCE];
		for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
			attachBones[i] = new Matrix[MAX_MODEL_TRANSFORMS];
	}
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(model);

	for (Transform* transform : transforms)
		SafeDelete(transform);

	SafeDelete(instanceBuffer);
	SafeDelete(instanceColorBuffer);

	SafeRelease(transformsSRV);

	SafeDelete(frameBuffer);
	SafeDelete(blendBuffer);


	SafeDelete(computeShader);
	SafeDelete(computeBoneBuffer);

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		SafeDeleteArray(attachBones[i]);
	SafeDeleteArray(attachBones);
}

void ModelAnimator::Update()
{
	//ImGui::SliderInt("Clip", (int *)&keyframeDesc.Clip, 0, model->ClipCount() - 1);
	
	//int frameCount = (int)model->ClipByIndex(keyframeDesc.Clip)->FrameCount();
	//ImGui::SliderInt("Frame", (int *)&keyframeDesc.CurrFrame, 0, frameCount - 1);
	//ImGui::InputInt("Frame", (int *)&keyframeDesc.CurrFrame);
	//keyframeDesc.CurrFrame %= frameCount;

	if (texture == NULL)
	{
		SetShader(shader);

		CreateTexture();


		Matrix bones[MAX_MODEL_TRANSFORMS];
		for (UINT i = 0; i < model->BoneCount(); i++)
			bones[i] = model->BoneByIndex(i)->Transform();

		computeBoneBuffer->CopyToInput(bones);
	}

	//30초당 1번계산하면 돼. 나중에 속도가 떨어지긴해
	for (UINT i = 0; i < transforms.size(); i++)
	{
		if (blendDesc[i].Mode == 0)
			UpdateAnimationFrame(i);
		else
			UpdateBlendingFrame(i);
	}


	frameBuffer->Render();
	blendBuffer->Render();

	frameTime += Time::Delta();
	if (frameTime > (1.0f / frameRate))
	{
		//컴퓨트 쉐이더에 넘겨주는거/ 왜 넘겨줘야하는지 알지?
		//최종월드 만들어줘야하니까
		sComputeFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
		sComputeBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());
		sComputeTransformsSRV->SetResource(transformsSRV);
		//최종월드 만들어줘야하니까
		sComputeInputBoneBuffer->SetResource(computeBoneBuffer->SRV());
		sComputeInputWorldBuffer->SetResource(computeWorldBuffer->SRV()); //얘가 다 위치 바뀔때마다 갱신시켜줄거야.
		sComputeOutputBuffer->SetUnorderedAccessView(computeOutputBuffer->UAV());
		//y가 모델 인스턴스 , 텍스처가 1:1매칭
		computeShader->Dispatch(0, 0, 1, MAX_MODEL_INSTANCE, 1);

		
		ID3D11Texture2D* texture = computeOutputBuffer->CopyFromOutput();
		D3D11_MAPPED_SUBRESOURCE subResource;
		D3D::GetDC()->Map(texture, 0, D3D11_MAP_READ, 0, &subResource);
		{
			for (UINT i = 0; i < transforms.size(); i++)
				memcpy(attachBones[i], (BYTE *)subResource.pData + (i * subResource.RowPitch), sizeof(Matrix) * MAX_MODEL_TRANSFORMS);
		}
		D3D::GetDC()->Unmap(texture, 0);
	}
	frameTime = fmod(frameTime, (1.0f / frameRate));


	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelAnimator::UpdateAnimationFrame(UINT instance)
{
	//KeyframeDesc& desc = keyframeDesc;
	//ModelClip* clip = model->ClipByIndex(desc.Clip);

	//desc.RunningTime += Time::Delta();

	//float time = 1.0f / clip->FrameRate() / desc.Speed;
	//if (desc.Time >= 1.0f)
	//{
	//	desc.RunningTime = 0;

	//	desc.CurrFrame = (desc.CurrFrame + 1) % clip->FrameCount();
	//	desc.NextFrame = (desc.CurrFrame + 1) % clip->FrameCount();
	//}
	//desc.Time = desc.RunningTime / time;


	TweenDesc& desc = tweenDesc[instance];
	ModelClip* clip = model->ClipByIndex(desc.Curr.Clip);

	//현재 애니메이션
	{
		desc.Curr.RunningTime += Time::Delta();

		float time = 1.0f / clip->FrameRate() / desc.Curr.Speed;
		if (desc.Curr.Time >= 1.0f)
		{
			desc.Curr.RunningTime = 0;

			desc.Curr.CurrFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
			desc.Curr.NextFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
		}
		desc.Curr.Time = desc.Curr.RunningTime / time;
	}

	//바뀔 애니메이션이 존재함
	if (desc.Next.Clip > -1)
	{
		ModelClip* nextClip = model->ClipByIndex(desc.Next.Clip);

		desc.ChangeTime += Time::Delta();
		desc.TweenTime = desc.ChangeTime / desc.TakeTime;

		if (desc.TweenTime >= 1.0f)
		{
			desc.Curr = desc.Next;

			desc.Next.Clip = -1;
			desc.Next.CurrFrame = 0;
			desc.Next.NextFrame = 0;
			desc.Next.Time = 0;
			desc.Next.RunningTime = 0.0f;

			desc.ChangeTime = 0.0f;
			desc.TweenTime = 0.0f;
		}
		else
		{
			desc.Next.RunningTime += Time::Delta();

			float time = 1.0f / nextClip->FrameRate() / desc.Next.Speed;
			if (desc.Next.Time >= 1.0f)
			{
				desc.Next.RunningTime = 0;

				desc.Next.CurrFrame = (desc.Next.CurrFrame + 1) % nextClip->FrameCount();
				desc.Next.NextFrame = (desc.Next.CurrFrame + 1) % nextClip->FrameCount();
			}
			desc.Next.Time = desc.Next.RunningTime / time;
		}
	}
}

void ModelAnimator::UpdateBlendingFrame(UINT instance)
{
	//배열로 하면되겟지 다 새로계산할거 아니야. CB_ 도 배열로 한다. setAnimation[id.y] 로 쓴다.
	BlendDesc& desc = blendDesc[instance];

	for (UINT i = 0; i < 3; i++)
	{
		ModelClip* clip = model->ClipByIndex(desc.Clip[i].Clip);

		desc.Clip[i].RunningTime += Time::Delta();

		float time = 1.0f / clip->FrameRate() / desc.Clip[i].Speed;
		if (desc.Clip[i].Time >= 1.0f)
		{
			desc.Clip[i].RunningTime = 0;

			desc.Clip[i].CurrFrame = (desc.Clip[i].CurrFrame + 1) % clip->FrameCount();
			desc.Clip[i].NextFrame = (desc.Clip[i].CurrFrame + 1) % clip->FrameCount();
		}
		desc.Clip[i].Time = desc.Clip[i].RunningTime / time;
	}
}

void ModelAnimator::Render()
{
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
	sBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());
	
	sTransformsSRV->SetResource(transformsSRV);

	//인스턴트 버퍼 랑 컬러 버퍼 세팅
	instanceBuffer->Render();
	instanceColorBuffer->Render();

	for (ModelMesh* mesh : model->Meshes())
		mesh->Render(transforms.size());
}

void ModelAnimator::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
}

void ModelAnimator::ReadMesh(wstring file)
{
	model->ReadMesh(file);
}

void ModelAnimator::ReadClip(wstring file)
{
	model->ReadClip(file);
}

void ModelAnimator::PlayTweenMode(UINT instance, UINT clip, float speed, float takeTime)
{
	tweenDesc[instance].TakeTime = takeTime;
	tweenDesc[instance].Next.Clip = clip;
	tweenDesc[instance].Next.Speed = speed;
}

void ModelAnimator::PlayBlendMode(UINT instance, UINT clip, UINT clip1, UINT clip2)
{
	blendDesc[instance].Mode = 1;

	blendDesc[instance].Clip[0].Clip = clip;
	blendDesc[instance].Clip[1].Clip = clip1;
	blendDesc[instance].Clip[2].Clip = clip2;
}

void ModelAnimator::SetBlendAlpha(UINT instance, float alpha)
{
	alpha = Math::Clamp(alpha, 0.0f, 2.0f);

	blendDesc[instance].Alpha = alpha;
}

void ModelAnimator::SetShader(Shader * shader)
{
	this->shader = shader;

	//sFrameBuffer = shader->AsConstantBuffer("CB_KeyFrames");
	sFrameBuffer = shader->AsConstantBuffer("CB_AnimationFrame");
	sBlendBuffer = shader->AsConstantBuffer("CB_BlendFrame");

	sTransformsSRV = shader->AsSRV("TransformsMap");

	for (ModelMesh* mesh : model->Meshes())
		mesh->SetShader(shader);
}

void ModelAnimator::Pass(UINT pass)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

Transform * ModelAnimator::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	return transform;
}

void ModelAnimator::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
		memcpy(worlds[i], transforms[i]->World(), sizeof(Matrix));

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MAX_MODEL_INSTANCE);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);


	computeWorldBuffer->CopyToInput(worlds);
}

void ModelAnimator::GetAttachBones(UINT instance, Matrix * outMatrix)
{
	memcpy(outMatrix, attachBones[instance], sizeof(Matrix) * MAX_MODEL_TRANSFORMS);
}

void ModelAnimator::CreateTexture()
{
	//Matrix transform[500][250]; //StackOverflow

	clipTransforms = new ClipTransform[model->ClipCount()];
	for (UINT i = 0; i < model->ClipCount(); i++)
		CreateClipTransform(i);


	//CreateTexture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_KEYFRAMES;
		desc.ArraySize = model->ClipCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;


		UINT pageSize = MAX_MODEL_TRANSFORMS * 4 * 16 * MAX_MODEL_KEYFRAMES;
		//void* p = malloc(pageSize * model->ClipCount());
		void* p = VirtualAlloc(NULL, pageSize * model->ClipCount(), MEM_RESERVE, PAGE_READWRITE);

		//MEMORY_BASIC_INFORMATION memory;
		//VirtualQuery(p, &memory, sizeof(MEMORY_BASIC_INFORMATION));

		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			UINT start = c * pageSize;

			for (UINT k = 0; k < MAX_MODEL_KEYFRAMES; k++)
			{
				void* temp = (BYTE *)p + MAX_MODEL_TRANSFORMS * k * sizeof(Matrix) + start;

				VirtualAlloc(temp, MAX_MODEL_TRANSFORMS * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
				memcpy(temp, clipTransforms[c].Transform[k], MAX_MODEL_TRANSFORMS * sizeof(Matrix));
			}
		}//for(c)


		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[model->ClipCount()];
		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			void* temp = (BYTE *)p + c * pageSize;

			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
		}
		Check(D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture));


		SafeDeleteArray(subResource);
		VirtualFree(p, 0, MEM_RELEASE);
	}

	//CreateSRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = model->ClipCount();

		Check(D3D::GetDevice()->CreateShaderResourceView(texture, &desc, &transformsSRV));
	}

	for (ModelMesh* mesh : model->Meshes())
		mesh->TransformsSRV(transformsSRV);
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	Matrix* bones = new Matrix[MAX_MODEL_TRANSFORMS];

	ModelClip* clip = model->ClipByIndex(index);
	for (UINT f = 0; f < clip->FrameCount(); f++)
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{
			ModelBone* bone = model->BoneByIndex(b);


			Matrix parent;
			Matrix inv = bone->Transform();
			D3DXMatrixInverse(&inv, NULL, &inv);

			int parentIndex = bone->ParentIndex();
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];


			Matrix animation;
			ModelKeyframe* frame = clip->Keyframe(bone->Name());
			if (frame != NULL)
			{
				ModelKeyframeData& data = frame->Transforms[f];

				Matrix S, R, T;
				D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
				D3DXMatrixRotationQuaternion(&R, &data.Rotation);
				D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);

				animation = S * R * T;
			}
			else
			{
				D3DXMatrixIdentity(&animation);
			}

			bones[b] = animation * parent;
			clipTransforms[index].Transform[f][b] = inv * bones[b];
		}//for(b)
	}//for(f)
}