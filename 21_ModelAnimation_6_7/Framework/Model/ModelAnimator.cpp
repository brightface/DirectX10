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
		//�ؽ�ó�� �ѹ��� �����ϴ°ž�.
		CreateTexture();
	}
	//����� �ذ��ϴ¹�



	//keyframeDesc& desc = keyfreameDesc;
	//ModelClip* cli = model->ClipByIndex(desc.Clip);
	//
	//desc.RunningTime += Time::Delta();//���������� -����������
	//float time = 1.0f / clip - FrameRate()/desc.Speed.;
	////������ ����Ʈ
	//if (desc.Time >= 1.0f)
	//{
	//	//30���� 1�ʰ� ������. �׷��� �������������� �Ѱ��ְڴ�.
	//	desc.RunningTime = 0;
	//	desc.CrrFrame = (desc.CurrFrame + 1) % clip->FrameCount();
	//	desc.NextFrame = (desc.CurrFrame + 1) % clp->FrameCount();
	//}
	//desc.Time = desc.RunningTime / Time;

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();

	TweenDesc& desc = tweenDesc;
	ModelClip* clip = model->ClipByIndex(desc.Curr.Clip);

	//���� �ִϸ��̼�
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
	//���ٸ�
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
	//���̴��� �ѱ�� �� �Ѱ������
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
	//���� 500, �� 250�� 
	//������ 500*250*64(��Ʈ�����ϳ��� 64����Ʈ) = 800��(8�ް�)(200���� 2�ް�)
	clipTransforms = new ClipTransform[model->ClipCount()]; //���۸�ŭ �Ҵ��Ұž�
	for (UINT i = 0; i < model->ClipCount(); i++)
		CreateClipTransform(i); // �� �帶�� �� �Լ��� ����Űŵ�// 

	
	//�ؽ�ó����
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
		//void *p = malloc(pageSize * model->ClipCount()); //���������� ũ�⸸ŭ 1MB, MSD 
		void* p = VirtualAlloc(NULL, pageSize * model->ClipCount(), MEM_RESERVE, PAGE_READWRITE); //�Ҵ��ϱ����� ���� ������ �� , ����޸𸮿� ������ �� , ���� �ʴ����� �Ҵ� . 

		//����� �޸𸮹���Ʈ�� �� ����

		//Ŭ������ �����ϴ�.
		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			UINT start = c * pageSize;
			//�ش��帶�� ���� �ּҸ� ����

			//������ ������ 2�� n���� �������
			for (UINT k = 0; k < MAX_MODEL_KEYFRAMES; k++) {
				void* temp = (BYTE*)p + MAX_MODEL_TRANSFORMS * k * sizeof(Matrix) + start;
				//�߶�� 
				VirtualAlloc(temp, MAX_MODEL_TRANSFORMS * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
				//����ȭ ���� ������ �����.

				//Ư���� �ּҸ� �޾Ƽ� ������ ����Ʈ�� �ߴٸ� 
				memcpy(temp, clipTransforms[c].Transform[k], MAX_MODEL_TRANSFORMS * sizeof(Matrix)); //Ŭ�������� �ǳʶٰ� �ٴ����� �ǳʶٸ����.
				//����ü �ϳ��� ���ϳ� k�� ���̰� 
				//cpu �� �ϵ� ����޸𸮿� �־����.
				//gpu�� �־�� ����

			}
		}//CPU ����Ϸ�

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[model->ClipCount()];
		for (UINT c = 0; c<model->ClipCount(); c++)//��纹��
		{
			void*temp(BYTE *)p + c * pageSize;

			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
			//2�� �¼��� �÷Ʒ���

		}
		Check(D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture));

		SafeDeleteArray(subResource);
		VirtualFree(p, 0, MEM_RELEASE); //0�ϸ� ��üũ��
		//�ּҸ� �����°� �ƴ϶� �� �����.
		//�ؽ�ó�� ���̴��� �ѱ���� srv �־���Ѵ�. 

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
//�ִϸ��̼� �߿� �ٽ��߿� �ϳ��� �Լ�
void ModelAnimator::CreateClipTransform(UINT index)
{
	Matrix* bones = new Matrix[MAX_MODEL_TRANSFORMS];

	ModelClip* clip = model->ClipByIndex(index);
	for (UINT f = 0; f < clip->FrameCount(); f++) //������ frame��
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{	//�׾ȿ� ������ �� �ִ�. ������ �ȿ�
			ModelBone* bone = model->BoneByIndex(b);
			//���� �����ðž� ������ �ε�������
			
			Matrix parent;
			Matrix inv = bone->Transform();
			D3DXMatrixInverse(&inv, NULL, &inv);
			//�ι��� ��ų�ž�
			int parentIndex = bone->ParentIndex(); //�θ��ȣ ������
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];
				
			//�ڱ��� ����� ����ķ� �а��̴�.
			Matrix animation;
			//�θ� ���� �����°��̴�.
			//�������� 2���� ������
			//�����ӿ� ���� �ִ°��� ���°�찡 �ִ�.
			
			ModelKeyframe* frame = clip->Keyframe(bone->Name());
			if (frame != NULL)
			{
				ModelKeyframeData& data = frame->Transforms[f];
				//���� �̸����� Ű������ �����°ž�
				//�� Ű������ ������ s r t �� ���� �����س���.
				Matrix S, R, T;

				//������ ���� �ִϸ��̼��� ���� ������ �ִϸ��̼� ������ 
				//��Ŀ��ٰ� ����°ž�

				D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
				
				D3DXMatrixRotationQuaternion(&R, &data.Rotation);
				//�״����� ȸ���̴�. (���� �����س����� ���ʹϿ����� �����س�)
				D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);
				//�״����� �̵�
				animation = S * R * T;//�ִϸ��̼��� �� ����̾�.
				
			
			}
			else
			{	//���� ���°�� �θ�� ����. �հ��� �߸���
				D3DXMatrixIsIdentity(&animation);
			}
			//�̰� ���� ���� �Ҽ� �ִ�. ��ũ ����� ȸ������
			//�̰� �� �̷��� �Ǵ��� �˸� �װ͵� ������.
			bones[b] = animation * parent;
			//ó���� �ִ°�(R) * ����ŭ (R) = ��ġ�� ���Ծ�(G)
				//�ű⿡�� �θ�ŭ �̵�(*G) = G*G 
			/////////////////////////////////////////////
			//�ִϸ��̼� �Ȱ� �θ������� �̵��Ѱž� �ϴ�
			//R*G
			//�ٵ� ���� ���غ��� �ű�� �̵��ؾߵ�
			//INV (BONE->TRANSFORM�� �����) R *G = G
			//�ִϸ��̼��� �θ����� �󸶸�ŭ ���ų� �س���
			//�״����� ���غ��� �����Ѵ�.

			//���غ��� ������������ �θ�� ������߰���. ��ũ ȸ����ų����
			clipTransforms[index].Transform[f][b] = inv * bones[b];
		}//for(b)
	}//for(f)
}