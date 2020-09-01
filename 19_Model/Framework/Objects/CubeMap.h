#pragma once

class CubeMap
{
public:
	CubeMap(Shader* shader);
	~CubeMap();

	void Texture(wstring file);

	void Update();
	void Render();

	Transform* GetTransform() { return mesh->GetTransform(); }

private:
	Shader* shader;

	Mesh* mesh;

	ID3D11ShaderResourceView* srv = NULL;
	ID3DX11EffectShaderResourceVariable* sSrv;
};