#pragma once

struct TRANSFORM_DATA
{
	D3DXMATRIX world; //4x4
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
	class Graphics* graphics = nullptr;
	
	D3D11_Geometry<D3D11_VertexTexture> geometry;
	D3D11_VertexBuffer* vertex_buffer = nullptr;
	D3D11_IndexBuffer* index_buffer = nullptr;
	D3D11_InputLayout* input_layout = nullptr;
	D3D11_Shader* vertex_shader = nullptr;
	D3D11_Shader* pixel_shader = nullptr;
	D3D11_ConstantBuffer* gpu_buffer = nullptr; // Constant Buffer
	D3D11_RasterizerState* rasterizer_state = nullptr;
	D3D11_Texture* texture = nullptr;
	D3D11_SamplerState* sampler_state = nullptr;
	D3D11_BlendState* blend_state = nullptr;

	class Camera* camera = nullptr;
	D3DXMATRIX world; //4x4

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