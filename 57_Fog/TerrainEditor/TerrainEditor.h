#pragma once

class TerrainEditor
{
public:
	TerrainEditor(Shader* shader);
	~TerrainEditor();

	void Update();
	void Render();

	void Pass(UINT val) { terrain->Pass(val); }
	void RaiseHeight(Vector3& center, float intensity);

private:
	struct LineDesc
	{
		Color Color = D3DXCOLOR(1, 1, 1, 1);
		UINT Visible = 1;
		float Thickness = 0.01f;
		float Size = 5.0f;
		float Padding;
	} lineDesc;

	struct BrushDesc
	{
		Color Color = D3DXCOLOR(0, 1, 0, 1);
		Vector3 Location;
		UINT Type = 0;
		UINT Range = 2;
		float Padding[3];
	} brushDesc;

private:
	Shader* shader;
	Terrain* terrain;

	ConstantBuffer* lineBuffer;
	ID3DX11EffectConstantBuffer* sLineBuffer;

	ConstantBuffer* brushBuffer;
	ID3DX11EffectConstantBuffer* sBrushBuffer;
};