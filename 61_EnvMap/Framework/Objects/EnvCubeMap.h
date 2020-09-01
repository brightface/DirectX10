#pragma once

class EnvCubeMap
{
public:
	EnvCubeMap(Shader* shader, float width, float height);
	~EnvCubeMap();

	void PreRender(Vector3& position, Vector3& scale, float zNear = 0.1f, float zFar = 500.0f, float fov = 0.5f);
	void Render();

	UINT& Type() { return desc.Type; }
	void Type(UINT val) { desc.Type = val; }
	//c버퍼 리턴하는 함수들 밑에는
	float& Alpha() { return desc.Alpha; }
	float& RefractAmount() { return desc.RefractAmount; }

	float& FresnelAmount() { return desc.FresnelAmount; }
	float& FresnelBias() { return desc.FresnelBias; }
	float& FresnelScale() { return desc.FresnelScale; }

	ID3D11ShaderResourceView* SRV() { return srv; }
	Projection* GetProjection() { return projection; }

private:
	struct Desc
	{
		UINT Type =0; //여러가지 타입캐스팅할거거든
		float Alpha = 0.75f; //알파값
		float RefractAmount = 1.52f - 1.0f; //반사정도
		float Padding;

		float FresnelAmount = 1.0f; //플레그넷 정도
		float FresnelBias = 1.0f;
		float FresnelScale = 1.0f;
		float Padding2;

		Matrix Views[6]; //6개가 중요해. 정점을 6면으로 늘려버린다.
		Matrix Projection;
	} desc;

private:
	Shader* shader;

	Vector3 position; //그릴 위치
	float width, height;//해상도

	ID3D11Texture2D* rtvTexture;//랜더타켓뷰
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;

	ID3D11Texture2D* dsvTexture;
	ID3D11DepthStencilView* dsv;//뎁스 트샌실 뷰

	Projection* projection;//오소를쓰는게 이쁘긴할거야. 퍼스펙 티브 하나 써보고 ,먼저 퍼스펙티브 쓸거야.
	Viewport* viewport;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	ID3DX11EffectShaderResourceVariable* sSrv;
};