#pragma once

class Shadow
{
public:
public:
	Shadow(Shader* shader, Vector3& position, float radius, float width = 1024, float height = 1024);
	~Shadow();

	void PreRender();

	ID3D11ShaderResourceView* SRV() { return renderTarget->SRV(); }

private:
	void CalcViewProjection();

private:

	//constant buffer 용 
	struct Desc
	{
		Matrix View;
		Matrix Projection; //라이트 공간의 뷰포로젝션

		Vector2 MapSize; //해상도
		float Bias = -0.0006f;

		UINT Quality = 0;
	} desc;

private:
	Shader* shader;
	float width, height;

	//조명의 위치가 아니라 조명을 받을 위치(공간)
	Vector3 position;
	float radius;

	//랜더타겟 필요없어
	RenderTarget* renderTarget; //깊이를 보여주려는애다.
	//해상도 맞추려고 다 넣어놓것이다.
	DepthStencil* depthStencil; //깊이 판단하려는애고
	Viewport* viewport;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	ID3DX11EffectShaderResourceVariable* sShadowMap;

	ID3D11SamplerState* pcfSampler;
	ID3DX11EffectSamplerVariable* sPcfSampler;
};

//shadow -> rendertarget->srv
//shadow->depthstencil
//			->texture-> dsv
			//		->stv

// pass -> RTV(SRV)
// pass -> depthstencil -SRV <실제로 깊이가 얘한테 ㅣㅇㅆ는거잖아)