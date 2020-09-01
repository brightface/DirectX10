#pragma once

struct VertexColor
{
	D3DXVECTOR3 position;
	D3DXCOLOR color;
};

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
	
	VertexColor* vertices = nullptr;
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
//Clipping 오리는것
//NDC 정규화
//Back-face Culling 뒷면을 앞면으로 바꾼다.그리고 보이지않는면을 제거한다.
//Viewport Transform 뷰포트 늘려주고
//Scan Transform 픽셀쉐이더가 칠해야 할 부분을 정해준다.



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