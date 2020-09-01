#pragma once
//모델 랜더링 하는 부분이다. 이게 빡세다. 메시 모델 랜더링 // 헷갈릴까봐 상속구조로 해도 되는데 따로따로 하는것이다.

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
	void CreateTexture(); //이부분이 힘들어
	void CreateClipTransform(UINT index);

private:
	struct ClipTransform
	{
		Matrix** Transform;
		
		ClipTransform()
		{	//2차원 배열이다 동적할당이니 뒤에 포인터 붙어야지. 상수도 처리
			Transform = new Matrix*[MAX_MODEL_KEYFRAMES];
			//이제 열단위할당해야한다. 앞에건 행단위였고

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

		UINT CurrFrame = 0;//현재프레임
		UINT NextFrame = 0;//다음프레임

		float Time = 0.0f;//현재시간
		float RunningTime = 0.0f;//프레임이 플레이되고있는시간

		float Speed = 0.1f;//속도

		Vector2 Padding;//대입
	} keyframeDesc; //c버퍼

	struct TweenDesc
	{
		float TakeTime = 1.0f; //바뀔 총 시간 //speed 
		float TweenTime = 0.0f; //바뀔 전체 최종시간, 쉐이더로 넘어갈 타임하고 같아.
		float ChangeTime = 0.0f; //Running타임이랑 같다. 증가 되는시간.
		float Padding;

		KeyframeDesc Curr;
		KeyframeDesc Next;

		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1; //다음애니메이션이 없다는것 0이보다 커야 존재한다느것
		}
	} tweenDesc;

	ConstantBuffer* frameBuffer; //넘길버퍼
	ID3DX11EffectConstantBuffer* sFrameBuffer;

private:
	Shader* shader;
	Model* model;
	//애니메이션이 어디에 투영되는지, 애를통해 움직인다. transform
	Transform* transform= NULL; // 새로 만들어지니까 NULL해놔야한다.
};