#include "Framework.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform(shader);

	//frameBuffer = new ConstantBuffer(&keyframeDesc, sizeof(KeyframeDesc));
	sFrameBuffer = shader->AsConstantBuffer("CB_KeyFrame");
	frameBuffer =
}


ModelAnimator::~ModelAnimator()
{
	SafeDelete(model);
	SafeDelete(transform);

	SafeDeleteArray(clipTransforms);
	SafeRelease(texture);
	SafeRelease(srv);

	SafeDelete(frameBuffer);
}

void ModelAnimator::Update()
{
	if (texture == NULL)
	{	
		for (ModelMesh* mesh : model->Meshes())
			mesh->SetShader(shader);
		//텍스처를 한번만 생성하는거야.
		CreateTexture();
	}
	//끊기는 해결하는법



	//keyframeDesc& desc = keyfreameDesc;
	//ModelClip* cli = model->ClipByIndex(desc.Clip);
	//
	//desc.RunningTime += Time::Delta();//이전프레임 -다음프레임
	//float time = 1.0f / clip - FrameRate()/desc.Speed.;
	////프레임 레이트
	//if (desc.Time >= 1.0f)
	//{
	//	//30분의 1초가 지났다. 그래서 다음프레임으로 넘겨주겠다.
	//	desc.RunningTime = 0;
	//	desc.CrrFrame = (desc.CurrFrame + 1) % clip->FrameCount();
	//	desc.NextFrame = (desc.CurrFrame + 1) % clp->FrameCount();
	//}
	//desc.Time = desc.RunningTime / Time;

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();

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
	//없다면
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

void ModelAnimator::Render()
{
	frameBuffer->Render();
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());

	sTransform->SetResource(transformSrv);
	//쉐이더로 넘길거 다 넘겨줬겟지
	for (ModelMesh*, mesh : model->Meshes())
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

void ModelAnimator::PlayClip(UINT clip, float speed = 1.0f, float takeTime = 1.0f)
{
	keyframeDesc.Clip = clip;
	keyframeDesc.Speed = speed;

	tweenDesc.TakeTime = takeTime;
	tweenDesc.Next.Clip = clip;
	tweenDesc.Next.Speed = speed;


}

void ModelAnimator::SetShader(Shader * shader)
{
	SafeDelete(transform);
	transform = new Transform(shader);

	for(ModelMesh* mesh : model->)
	frameBuffer = new 
}

void ModelAnimator::Pass(UINT pass)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

void ModelAnimator::CreateTexture()
{
	//Matrix transform[500][250]; //StackOverflow
	//행이 500, 열 250개 
	//한장이 500*250*64(매트릭스하나당 64바이트) = 800만(8메가)(200만이 2메가)
	clipTransforms = new ClipTransform[model->ClipCount()]; //동작만큼 할당할거야
	for (UINT i = 0; i < model->ClipCount(); i++)
		CreateClipTransform(i); // 각 장마다 이 함수를 만들거거든// 

	
	//텍스처제작
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

		UINT pageSize = MAX_MODEL_TRANSFORMS * 4 * 16 * MAX_MODEL_KEYFRAMES;//sizeof(Matrix)
		//void *p = malloc(pageSize * model->ClipCount()); //스택프레임 크기만큼 1MB, MSD 
		void* p = VirtualAlloc(NULL, pageSize * model->ClipCount(), MEM_RESERVE, PAGE_READWRITE); //할당하기전에 먼저 예약을 해 , 가상메모리에 저장을 해 , 지면 초대형을 할당 . 

		//예약된 메모리바이트수 가 나와

		//클립부터 돌립니다.
		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			UINT start = c * pageSize;
			//해당장마다 시작 주소를 만듬

			//한줄의 사이즈 2의 n승이 만들어져
			for (UINT k = 0; k < MAX_MODEL_KEYFRAMES; k++) {
				void* temp = (BYTE*)p + MAX_MODEL_TRANSFORMS * k * sizeof(Matrix) + start;
				//잘라놔 
				VirtualAlloc(temp, MAX_MODEL_TRANSFORMS * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
				//단편화 문제 문제가 생긴다.

				//특정한 주소를 받아서 썻을때 딜리트를 했다면 
				memcpy(temp, clipTransforms[c].Transform[k], MAX_MODEL_TRANSFORMS * sizeof(Matrix)); //클립단위로 건너뛰고 줄단위로 건너뛰면되지.
				//구조체 하나가 장하나 k가 줄이고 
				//cpu 램 하드 가상메모리에 넣어놨다.
				//gpu에 넣어야 겠지

			}
		}//CPU 복사완료

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[model->ClipCount()];
		for (UINT c = 0; c<model->ClipCount(); c++)//장당복사
		{
			void*temp(BYTE *)p + c * pageSize;

			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
			//2의 승수로 늘렵러ㅕ

		}
		Check(D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture));

		SafeDeleteArray(subResource);
		VirtualFree(p, 0, MEM_RELEASE); //0하면 전체크기
		//주소만 날리는게 아니라 다 지운다.
		//텍스처를 쉐이더로 넘길려면 srv 있어야한다. 

		//CreateSRV
		{
			D3D11_SHADER_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.MipLevels = 1;
			desc.SampleDesc.Count = 1;

		}
		
	}

}
//애니메이션 중에 핵심중에 하나인 함수
void ModelAnimator::CreateClipTransform(UINT index)
{
	Matrix* bones = new Matrix[MAX_MODEL_TRANSFORMS];

	ModelClip* clip = model->ClipByIndex(index);
	for (UINT f = 0; f < clip->FrameCount(); f++) //동작의 frame수
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{	//그안에 본들이 들어가 있다. 프레임 안에
			ModelBone* bone = model->BoneByIndex(b);
			//본을 가져올거야 본바이 인덱스에서
			
			Matrix parent;
			Matrix inv = bone->Transform();
			D3DXMatrixInverse(&inv, NULL, &inv);
			//인버스 시킬거야
			int parentIndex = bone->ParentIndex(); //부모번호 가져와
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];
				
			//자기의 행렬을 역행렬로 둔것이다.
			Matrix animation;
			//부모 따로 가져온것이다.
			//프레임이 2개로 나뉘어
			//프레임에 본이 있는경우랑 없는경우가 있다.
			
			ModelKeyframe* frame = clip->Keyframe(bone->Name());
			if (frame != NULL)
			{
				ModelKeyframeData& data = frame->Transforms[f];
				//본의 이름으로 키프레임 가져온거야
				//모델 키프레임 순서에 s r t 로 순서 저장해놨다.
				Matrix S, R, T;

				//프레임 별로 애니메이션이 본을 움직일 애니메이션 정보를 
				//행렬에다가 만드는거야

				D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
				
				D3DXMatrixRotationQuaternion(&R, &data.Rotation);
				//그다음에 회전이다. (내가 저장해놓은게 쿼터니옴으로 저장해놈)
				D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);
				//그다음에 이동
				animation = S * R * T;//애니메이션이 될 행렬이야.
				
			
			}
			else
			{	//본이 없는경우 부모거 쓴다. 손가락 잘릴때
				D3DXMatrixIsIdentity(&animation);
			}
			//이거 보고 과제 할수 있다. 땡크 제대로 회전시켜
			//이게 왜 이렇게 되는지 알면 그것도 가능해.
			bones[b] = animation * parent;
			//처음에 있는곳(R) * 간만큼 (R) = 위치가 나왔어(G)
				//거기에서 부모만큼 이동(*G) = G*G 
			/////////////////////////////////////////////
			//애니메이션 된게 부모쪽으로 이동한거야 일단
			//R*G
			//근데 이제 기준본이 거기로 이동해야돼
			//INV (BONE->TRANSFORM의 역행렬) R *G = G
			//애니메이션이 부모한테 얼마만큼 갈거냐 해놓고
			//그다음에 기준본이 가야한다.

			//기준본을 뒤집은다음에 부모랑 곱해줘야겠지. 탱크 회전시킬떄도
			clipTransforms[index].Transform[f][b] = inv * bones[b];
		}//for(b)
	}//for(f)
}