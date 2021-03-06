#pragma once

class Terrain : public Renderer
{
public:
	typedef VertexTextureNormal TerrainVertex;

public:
	Terrain(Shader* shader, wstring heightFile);
	~Terrain();

	void Update();
	void Render();

	void BaseMap(wstring file);

public:
	float GetHeight(Vector3& position);
	float GetVerticalRaycast(Vector3& position);
	Vector3 GetRaycastPosition();

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalVector();
	
private:
	UINT width, height;
	
	TerrainVertex* vertices;
	UINT* indices;

	Texture* heightMap;
	Texture* baseMap = NULL;
};