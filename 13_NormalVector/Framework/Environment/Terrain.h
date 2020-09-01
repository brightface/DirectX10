#pragma once

class Terrain
{
public:
	typedef VertexTextureNormal TerrainVertex;

public:
	Terrain(Shader* shader, wstring heightFile);
	~Terrain();

	void Update();
	void Render();

	void Pass(UINT val) { pass = val; }

	void BaseMap(wstring file);

private:
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateNormalVector();
	void CreateBuffer();
	
private:
	Shader* shader;
	UINT pass = 0;

	UINT width, height;
	
	UINT vertexCount;
	TerrainVertex* vertices;
	ID3D11Buffer* vertexBuffer;

	UINT indexCount;
	UINT* indices;
	ID3D11Buffer* indexBuffer;

	Matrix world;

	Texture* heightMap;
	Texture* baseMap = NULL;
};