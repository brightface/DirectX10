#include "Framework.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(Shader * shader)
	: shader(shader)
{
	model = new Model();

	transform = new Transform(shader);

	frameBuffer = new ConstantBuffer(&tweenDesc, sizeof(TweenDesc));
	blendBuffer = new ConstantBuffer(&blendDesc, sizeof(BlendDesc));
	
	//컴퓨트 쉐이더는 안 바뀔거야, 모델이 아예바뀔일이 없으니까. 쉐이더는 바껴도
	//Create ComputeShader
	{
		computeShader = new Shader(L"25_GetBones.fx");
		//컴퓨트 쉐이더를 구동시킬애
		sComputeWorld = computeShader->AsMatrix("World");
		//setshader에서 한것과같이 똑같이계산할거야.
		sComputeFrameBuffer = computeShader->AsConstantBuffer("CB_AnimationFrame");
		sComputeBlendBuffer = computeShader->AsConstantBuffer("CB_BlendFrame");
		sComputeTransformsSRV = computeShader->AsSRV("TransformsMap");
		
		//setShader  똑같이 컴퓨터 쉐이더로 넘어가야 하잖아.
		
		//계산해서 리턴받을거니까 - 매트릭스는 계산할 본들이 넘어갈거야. 본을 계산.max_model_transform이 본갯수
		computeBoneBuffer = new StructuredBuffer(NULL, sizeof(Matrix), MAX_MODEL_TRANSFORMS, sizeof(Matrix), MAX_MODEL_TRANSFORMS);
		//기준본들어갈거고
		sComputeInputBoneBuffer = computeShader->AsSRV("InputBones");
		//결과 본 
		sComputeOutputBoneBuffer = computeShader->AsUAV("OutputBones");
	}
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(model);
	SafeDelete(transform);

	SafeDeleteArray(clipTransforms);
	SafeRelease(texture);
	SafeRelease(transformsSRV);

	SafeDelete(frameBuffer);
	SafeDelete(blendBuffer);


	SafeDelete(computeShader);
	SafeDelete(computeBoneBuffer);
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
		SetShader(shader, true);

		CreateTexture();


		Matrix bones[MAX_MODEL_TRANSFORMS];
		//기준본은 그냥 넣어주면 돼
		for (UINT i = 0; i < model->BoneCount(); i++)
			bones[i] = model->BoneByIndex(i)->Transform();
		//이렇게 데이터 밀어주면 돼.
		computeBoneBuffer->CopyToInput(bones); //데이터를밀어줄거야. copybone해서 쉐이더로 
	}// 최초 한번만 이렇게 하고 그다음에 컴퓨터 쉐이더 구동시켜야지

	if (blendDesc.Mode == 0)
		UpdateAnimationFrame();
	else
		UpdateBlendingFrame();

	//frameBuffer->Render(); // 이게 쉐이더로 밀어준다는 뜻이잖아.
	//밀어준상태에서 //버퍼를 쉐이더로 밀어준다는 뜻이잖아./갱신이 된 상태에서 밀어준상태에서 컴퓨트 쉐이더로 계산해야지
	//업데이트 끝난후 값을 밀어주고 
	frameBuffer->Render();
	blendBuffer->Render();

	//30초 계산하는 방식.
	frameTime += Time::Delta();
	if (frameTime > (1.0f / frameRate))
	{	
		//월드 보내주잖아.
		sComputeWorld->SetMatrix(transform->World());
		//애니메이션 된행렬
		sComputeFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
		//애니메이션 계산행렬
		sComputeBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());
		sComputeTransformsSRV->SetResource(transformsSRV);
		//연결해주고
		sComputeInputBoneBuffer->SetResource(computeBoneBuffer->SRV());
		sComputeOutputBoneBuffer->SetUnorderedAccessView(computeBoneBuffer->UAV());

		//get한다음에 
		//copy from output 하면 되겠지?
		//쉐이더로 가자
		computeShader->Dispatch(0, 0, 1, 1, 1); //dispatch할때 그룹하나지. 
		
	}
	frameTime = fmod(frameTime, (1.0f / frameRate)); //30분의 1초마다 디스패치 250개 본 컴퓨트 쉐이더 계산할때 1024개 한번에 계산


	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelAnimator::UpdateAnimationFrame()
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


	TweenDesc& desc = tweenDesc;
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

void ModelAnimator::UpdateBlendingFrame()
{
	BlendDesc& desc = blendDesc;

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
	//여기에 원래 frameBuffer->Render(). 있었는데 update()로 넘어감
	//컴퓨트 쉐이더 계산하려고

	//쉐이더 연결해주는것이고 이건
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
	sBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());
	
	sTransformsSRV->SetResource(transformsSRV);

	for (ModelMesh* mesh : model->Meshes())
	{
		mesh->SetTransform(transform);
		mesh->Render();
	}
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

void ModelAnimator::PlayTweenMode(UINT clip, float speed, float takeTime)
{
	tweenDesc.TakeTime = takeTime;
	tweenDesc.Next.Clip = clip;
	tweenDesc.Next.Speed = speed;
}

void ModelAnimator::PlayBlendMode(UINT clip, UINT clip1, UINT clip2)
{
	blendDesc.Mode = 1;

	blendDesc.Clip[0].Clip = clip;
	blendDesc.Clip[1].Clip = clip1;
	blendDesc.Clip[2].Clip = clip2;
}

void ModelAnimator::SetBlendAlpha(float alpha)
{
	alpha = Math::Clamp(alpha, 0.0f, 2.0f);

	blendDesc.Alpha = alpha;
}

void ModelAnimator::SetShader(Shader * shader, bool bFirst)
{
	this->shader = shader;

	if (bFirst == false)
	{
		SafeDelete(transform);
		transform = new Transform(shader);
	}

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

void ModelAnimator::GetAttachBones(Matrix * outMatrix)
{
	computeBoneBuffer->CopyFromOutput(outMatrix);
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