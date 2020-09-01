#pragma once
class EnvCubeMap {
public:
	EnvCubeMap(Shader* shader, UINT width, UINT height);
	~EnvCubeMap();

	void PreRender(Vector3& position, Vector3& scale, float zNear = 0.1f, float zFar = 500.0f, float fov = 0.5f);
	void Render();

	UINT& Type() { return desc.Type; }
	void Type(UINT val) { desc.Type = val; }

	float& Alpha() { return desc.Alpha; }
	float& RefractAmount() { return desc.RefractAmount; }

	float& FresnelAmount() { return desc.FresnelAmount; }
	float& FresnelBias() { return desc.FresnelBias; }
	float& FresnelScale() { return desc.FresnelScale; }

	ID3D11ShaderResourceView* SRV() { return srv; }
	Projection* GetProjection() { return projection; }
	Shader* shader;

	Vector3 position; //�׸� ��ġ
	UINT width, height;

	ID3D11Texture2D* rtvTexture; //����Ÿ�Ϻ�
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;

	ID3D11Texture2D* dsvTexture;
	ID3D11DepthStencilView* dsv; //���� Ʈ���� ��

	Projection* projection; //���Ҹ����°� �̻ڱ��Ұž�. �۽��� Ƽ�� �ϳ� �Ẹ�� 
	Viewport* viewport;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	ID3DX11EffectShaderResourceVariable* sSrv;
};