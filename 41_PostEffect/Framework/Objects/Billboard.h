#pragma once
#define MAX_BILLBOARD_COUNT 10000

class Billboard : public Renderer
{
public:
	Billboard(Shader* shader);
	~Billboard();

	void Update();
	void Render();

	void AddTexture(wstring file);
	void Add(Vector3& position, Vector2& scale, UINT mapIndex);

private:
	struct VertexBillboard
	{
		Vector3 Position;
		Vector2 Scale;
		UINT MapIndex;
	};

private:
	vector<VertexBillboard> vertices;

	vector<wstring> textureFiles;
	TextureArray* textureArray = NULL;
	ID3DX11EffectShaderResourceVariable* sMaps;
};