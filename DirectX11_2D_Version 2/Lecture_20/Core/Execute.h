#pragma once

struct CAMERA_DATA
{
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

class Execute final
{
public:
	Execute();
	~Execute();
	
	void Update();
	void Render();

private:
	class Graphics* graphics					= nullptr;
	class Camera* camera						= nullptr;
	class D3D11_ConstantBuffer* camera_buffer	= nullptr;
	class D3D11_Pipeline* pipeline				= nullptr;
	class Rect* rect							= nullptr;
	class Rect* rect2							= nullptr;

};

//Rendering Pipeline
//- IA - VS - RS - PS - OM

//IA : Input Assembler Stage
//- Vertex

//VS : Vertex Shader -> shader : HLSL , GLSL
//- Transform
//Local
//World
//View
//Projection
//Clip	- Homogeneous space - ����

//RS : Rasterizer
//Clipping
//NDC
//Back-face Culling
//Viewport Transform
//Scan Transform



//- NDC
//NDC	- Euclidean space
//Viewport






//(1, 0, 0, 1)

//w -> 0 -> ����
//w -> 1 -> ��ġ


/*

|
|---------------------0------ 4
|
|--------0------------------- 2
|



*/



/*
	DirectX -> API -> GPU


	Rendering Pipeline
	IA -> VS -> RS -> PS -> OM

	IA - Input Assembler
	vertex - ����

	ID3D11Buffer -> vertex buffer
	ID3D11Buffer -> index buffer 
	ID3D11InputLayout
	D3D11_PRIMITIVE_TOPOLOGY

	VS - Vertex Shader
	- Constnat Buffer

	���� ��ȯ

	- Local - World - View - Projection - Clip -
	
	RS - Rasterizer

	- NDC - Viewport
	- rasterizer state

	PS - Pixel Shader
	Shader Resource

	OM - Output Merger
	- render target
	- blend state



	Resource View
	Buffer          Texture
*/