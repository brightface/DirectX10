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

	Vector3 position; //그릴 위치
	UINT width, height;

	ID3D11Texture2D* rtvTexture; //랜더타켓뷰
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;

	ID3D11Texture2D* dsvTexture;
	ID3D11DepthStencilView* dsv; //뎁스 트샌실 뷰

	Projection* projection; //오소를쓰는게 이쁘긴할거야. 퍼스펙 티브 하나 써보고 
	Viewport* viewport;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	ID3DX11EffectShaderResourceVariable* sSrv;
};