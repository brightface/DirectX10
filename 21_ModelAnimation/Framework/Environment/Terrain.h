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

public:
	float GetHeight(Vector3& position);
	float GetVerticalRaycast(Vector3& position);

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalVector();
	
private:
	Shader* shader;
	UINT pass = 0;

	UINT width, height;
	
	UINT vertexCount;
	TerrainVertex* vertices;
	VertexBuffer* vertexBuffer;

	UINT indexCount;
	UINT* indices;
	IndexBuffer* indexBuffer;

	Matrix world;

	Texture* heightMap;
	Texture* baseMap = NULL;
};