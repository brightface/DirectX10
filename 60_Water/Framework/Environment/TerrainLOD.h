#pragma once

class TerrainLOD : public Renderer
{
public:
	TerrainLOD(wstring imageFile);
	~TerrainLOD();

	void Update();
	void Render();

	float GetWidth();
	float GetHeight();

	void BaseMap(wstring file);

private:
	void ReadHeightData();
	void CalcVertexBounds();
	void CreatePatchVertex();
	void CreatePatchIndex();

private:
	struct VertexTerrainLOD
	{
		Vector3 Position;
		Vector2 Uv;
		Vector2 Bound;
	};

private:
	struct Desc
	{
		Vector2 Distance = Vector2(1, 1000);
		Vector2 TessFactor = Vector2(1, 64);

		float Cellspacing = 2.0f;
		float CellspacingU;
		float CellspacingV;
		float HeightScale = 2.5f;

		Plane CullPlane[6];
	} desc;

private:
	UINT cellsPerPatch = 32;

	wstring imageFile;

	UINT width, height;
	UINT patchWidth, patchHeight;

	float* heights;
	Vector2* bounds = NULL;

	ID3D11Texture2D* heightMap = NULL;
	ID3D11ShaderResourceView* heightMapSRV = NULL;
	ID3DX11EffectShaderResourceVariable* sHeightMapSRV;

	Texture* baseMap = NULL;
	ID3DX11EffectShaderResourceVariable* sBaseMap;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	Fixity* fixity;
	Frustum* frustum;
};