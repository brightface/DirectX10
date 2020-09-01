#pragma once

struct VertexColor
{
	D3DXVECTOR3 position;
	D3DXCOLOR color;
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

	D3DXMATRIX world; //4x4 동차행렬이라서 4개데이터 3개로도 표현가능
	D3DXMATRIX view;
	D3DXMATRIX projection;
	/*
		1 0 0 0 x축 방향 마지막이 동차 원소
		0 1 0 0 y축 방향
		0 0 1 0 z축 방향 2로 되면 크기가 2로 늘어난다. 2배로
		0 0 0 1 
		행렬은 항상 단윈행렬로 초기화해야한다.
		공간이 커지면 그 안에 데이터도 같이 커진다.

		공간이 이동하면 안의 데이터도 같이 이동한다.

        y    z
		^   /
		|  /
		| /
		|-------> x
		(0, 0, 0)
       
       OpenGL  - RH좌표계 먼저있었던것
	   DirecrX - LH좌표계 나중에 나온것MS에서 

	*/

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