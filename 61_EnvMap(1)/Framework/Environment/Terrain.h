#pragma once

class Terrain : public Renderer
{
public:
	friend class TerrainEditor;

public:
	Terrain(Shader* shader, wstring imageFile);
	~Terrain();

	void Update();
	void Render();

	void BaseMap(wstring file);
	void Layer1(wstring file);

public:
	float GetHeight(Vector3& position);
	float GetVerticalRaycast(Vector3& position);
	Vector3 GetRaycastPosition();

private:
	void ReadHeightData();
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalVector();

public:
	struct VertexTerrain
	{
		Vector3 Position = Vector3(0, 0, 0);
		Vector2 Uv = Vector2(0, 0);
		Vector3 Normal = Vector3(0, 0, 0);
		Vector3 Tangent = Vector3(0, 0, 0);
		Color Color = D3DXCOLOR(0, 0, 0, 1);
	};
	
private:
	Material* material;
	UINT width, height;

	wstring imageFile;

	
	VertexTerrain* vertices;
	UINT* indices;

	float* heights;
	
private:
	struct Layer
	{
		float* Data = NULL;
		ID3D11Texture2D* Texture2D = NULL;
		ID3D11ShaderResourceView* SRV = NULL;
		ID3DX11EffectShaderResourceVariable* sSRV;

		Texture* Map = NULL;
		ID3DX11EffectShaderResourceVariable* sMap;

		~Layer()
		{
			SafeDeleteArray(Data);
			SafeRelease(Texture2D);
			SafeRelease(SRV);
			SafeDelete(Map);
		}
	};
	Layer layer1;

	Texture* baseMap = NULL;
};