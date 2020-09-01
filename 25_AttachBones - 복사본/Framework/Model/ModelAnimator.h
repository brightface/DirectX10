#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	void Update();

private:
	void UpdateAnimationFrame();
	void UpdateBlendingFrame();

public:
	void Render();

public:
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);
	void ReadClip(wstring file);

	void PlayTweenMode(UINT clip, float speed = 1.0f, float takeTime = 1.0f);
	void PlayBlendMode(UINT clip, UINT clip1, UINT clip2);
	void SetBlendAlpha(float alpha);

	Transform* GetTransform() { return transform; }
	Model* GetModel() { return model; }

	void SetShader(Shader* shader, bool bFirst = false);
	void Pass(UINT pass);

	void GetAttachBones(Matrix* outMatrix);

private:
	void CreateTexture();
	void CreateClipTransform(UINT index);

private:
	struct ClipTransform
	{
		Matrix** Transform;

		ClipTransform()
		{
			Transform = new Matrix*[MAX_MODEL_KEYFRAMES];

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
	ID3D11ShaderResourceView* transformsSRV;
	ID3DX11EffectShaderResourceVariable* sTransformsSRV;
	
private:
	struct KeyframeDesc
	{
		int Clip = 0;

		UINT CurrFrame = 0;
		UINT NextFrame = 0;

		float Time = 0.0f;
		float RunningTime = 0.0f;

		float Speed = 1.0f;

		Vector2 Padding;
	}; //keyframeDesc;

	struct TweenDesc
	{
		float TakeTime = 1.0f; //Speed
		float TweenTime = 0.0f; //Time
		float ChangeTime = 0.0f; //Running
		float Padding;

		KeyframeDesc Curr;
		KeyframeDesc Next;

		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1;
		}
	} tweenDesc;

	ConstantBuffer* frameBuffer;
	ID3DX11EffectConstantBuffer* sFrameBuffer;

private:
	struct BlendDesc
	{
		UINT Mode = 0;
		float Alpha = 0;
		Vector2 Padding;

		KeyframeDesc Clip[3];
	} blendDesc;

	ConstantBuffer* blendBuffer;
	ID3DX11EffectConstantBuffer* sBlendBuffer;

private:
	Shader* shader;
	Model* model;

	Transform* transform = NULL;

private:
	float frameRate = 30.0f;
	float frameTime = 0.0f;

	Shader* computeShader;

	ID3DX11EffectMatrixVariable* sComputeWorld;

	//데이터 본 애니메이션하고 똑같이 넘어가. 
	ID3DX11EffectConstantBuffer* sComputeFrameBuffer;
	ID3DX11EffectConstantBuffer* sComputeBlendBuffer;
	ID3DX11EffectShaderResourceVariable* sComputeTransformsSRV;
	//부모 자식관계 맺어져있는 srv 도 똑같이 들어가

	//계산할 버퍼. computeBoneBuffer 
	//기준본 input, output 버퍼 하나로 통일할게
	StructuredBuffer* computeBoneBuffer;
	ID3DX11EffectShaderResourceVariable* sComputeInputBoneBuffer; //인풋시킬 버퍼
	ID3DX11EffectUnorderedAccessViewVariable* sComputeOutputBoneBuffer; //아웃풋 버퍼
};