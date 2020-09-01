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

	//이그대로 못들어가니까 버퍼형태로 들어간다. UINT형이다.
	uint* indices = nullptr;
	ID3D11Buffer* index_buffer = nullptr;

	ID3D11InputLayout* input_layout = nullptr;

	ID3D11VertexShader* vertex_shader = nullptr;
	ID3DBlob* vs_blob = nullptr;

	ID3D11PixelShader* pixel_shader = nullptr;
	ID3DBlob* ps_blob = nullptr;
};

//Rendering Pipeline
//- IA - VS - RS - PS - OM

//IA : Input Assembler Stage
//- Vertex

//VS : Vertex Shader -> shader : HLSL , GLSL
//- Transform

//RS : Rasterizer
//- NDC

//로컬공간/ <내공간>
//월드/ <세계>
//뷰 /<시야공간>
//프로젝션/ <투영공간> 차원을 낮추는것 3D를 2D로 변환시키는것 
//클립/ 오리다 자르는 클립이라 부른다.정규화시킨다. -1~1까지  // HOMOGENEOUS 동차공간  동차좌표 하나의 좌표를 추가해서 1차원표시할때도 2차원으로 표시
//1 이면 위치벡터 0이면 방향벡터 (동차좌표)
//VS까지 담당

//RS에서 담당
//엔디씨/ -1~1까지 //EUCLIDEAN SPACE 표준 (이것은 1로 통일시킴)
//뷰포트 //위에서 정규화 시켰지? 레스터라이징 한걸 다시 늘려준다. 