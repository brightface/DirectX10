#pragma once

struct TRANASFROM_DATA
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
	
	VertexTexture* vertices = nullptr;
	ID3D11Buffer* vertex_buffer = nullptr;

	uint* indices = nullptr;
	ID3D11Buffer* index_buffer = nullptr;

	ID3D11InputLayout* input_layout = nullptr;

	ID3D11VertexShader* vertex_shader = nullptr;
	ID3DBlob* vs_blob = nullptr;

	ID3D11PixelShader* pixel_shader = nullptr;
	ID3DBlob* ps_blob = nullptr;

	D3DXMATRIX world; //4x4
	D3DXMATRIX view;
	D3DXMATRIX projection;
	/*
		1 0 0 0
		0 1 0 0
		0 0 1 0
		0 0 0 1


        y    z
		^   /
		|  /
		| /
		|-------> x
		(0, 0, 0)
       
       OpenGL  - RH
	   DirecrX - LH

	*/

	TRANASFROM_DATA cpu_buffer;
	ID3D11Buffer* gpu_buffer = nullptr; // Constant Buffer

	ID3D11RasterizerState* rasterizer_state = nullptr;

	//ID3D11ShaderResourceView* shader_resource[2];
	ID3D11ShaderResourceView* shader_resource = nullptr;

	//Texture
	//ID3D11Texture2D
	//RTV
	//DSV
	//SRV	
	//UAV

	ID3D11SamplerState* sampler_state = nullptr;
	ID3D11BlendState* blend_state = nullptr;
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
//Clip	- Homogeneous space - 동차

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

//w -> 0 -> 방향
//w -> 1 -> 위치


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
	vertex - 정점

	ID3D11Buffer -> vertex buffer
	ID3D11Buffer -> index buffer 
	ID3D11InputLayout
	D3D11_PRIMITIVE_TOPOLOGY

	VS - Vertex Shader
	- Constnat Buffer

	공간 변환

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