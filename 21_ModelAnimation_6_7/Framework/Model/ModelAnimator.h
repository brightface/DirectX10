#pragma once
//�� ������ �ϴ� �κ��̴�. �̰� ������. �޽� �� ������ // �򰥸���� ��ӱ����� �ص� �Ǵµ� ���ε��� �ϴ°��̴�.

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	void Update();
	void Render();

public:
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);
	void ReadClip(wstring file);

	void PlayClip(UINT clip, float speed = 1.0f,float takeTime=1.0f);
	Transform* GetTransform() { return transform; }
	Model* GetModel() { return model; }

	void SetShader(Shader* shader);
	void Pass(UINT pass);

private:
	void CreateTexture(); //�̺κ��� �����
	void CreateClipTransform(UINT index);

private:
	struct ClipTransform
	{
		Matrix** Transform;
		
		ClipTransform()
		{	//2���� �迭�̴� �����Ҵ��̴� �ڿ� ������ �پ����. ����� ó��
			Transform = new Matrix*[MAX_MODEL_KEYFRAMES];
			//���� �������Ҵ��ؾ��Ѵ�. �տ��� ���������

			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; i++)
				Transform[i] = new Matrix[MAX_MODEL_TRANSFORMS];
		}

		~ClipTransform()
		{
			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; i++)
				SafeDeleteArray(Transform[i]);

			SafeDeleteArray(Transform);
		}
	};
	ClipTransform* clipTransforms;

	ID3D11Texture2D* texture = NULL;
	ID3D11ShaderResourceView* transformsSrv;
	ID3DX11EffectShaderResourceVariable sTransform;

	
private:
	struct KeyframeDesc
	{
		int Clip = 0;

		UINT CurrFrame = 0;//����������
		UINT NextFrame = 0;//����������

		float Time = 0.0f;//����ð�
		float RunningTime = 0.0f;//�������� �÷��̵ǰ��ִ½ð�

		float Speed = 0.1f;//�ӵ�

		Vector2 Padding;//����
	} keyframeDesc; //c����

	struct TweenDesc
	{
		float TakeTime = 1.0f; //�ٲ� �� �ð� //speed 
		float TweenTime = 0.0f; //�ٲ� ��ü �����ð�, ���̴��� �Ѿ Ÿ���ϰ� ����.
		float ChangeTime = 0.0f; //RunningŸ���̶� ����. ���� �Ǵ½ð�.
		float Padding;

		KeyframeDesc Curr;
		KeyframeDesc Next;

		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1; //�����ִϸ��̼��� ���ٴ°� 0�̺��� Ŀ�� �����Ѵٴ���
		}
	} tweenDesc;

	ConstantBuffer* frameBuffer; //�ѱ����
	ID3DX11EffectConstantBuffer* sFrameBuffer;

private:
	Shader* shader;
	Model* model;
	//�ִϸ��̼��� ��� �����Ǵ���, �ָ����� �����δ�. transform
	Transform* transform= NULL; // ���� ��������ϱ� NULL�س����Ѵ�.
};