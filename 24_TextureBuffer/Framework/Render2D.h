#pragma once

class Render2D : public Renderer
{
public:
	Render2D(wstring shaderFile);
	~Render2D();


	void Update() override;
	void Render() override; //언리얼 이면 뒤에

	void SRV(ID3D11ShaderResourceView* srv);

	struct Desc
	{
		Matrix View;
		Matrix Projection;
	}desc;

private:

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;
};