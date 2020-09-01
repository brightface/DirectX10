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

	//constant buffer �� 
	struct Desc
	{
		Matrix View;
		Matrix Projection; //����Ʈ ������ ����������

		Vector2 MapSize; //�ػ�
		float Bias = -0.0006f;

		UINT Quality = 0;
	} desc;

private:
	Shader* shader;
	float width, height;

	//������ ��ġ�� �ƴ϶� ������ ���� ��ġ(����)
	Vector3 position;
	float radius;

	//����Ÿ�� �ʿ����
	RenderTarget* renderTarget; //���̸� �����ַ��¾ִ�.
	//�ػ� ���߷��� �� �־�����̴�.
	DepthStencil* depthStencil; //���� �Ǵ��Ϸ��¾ְ�
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
// pass -> depthstencil -SRV <������ ���̰� ������ �Ӥ����°��ݾ�)