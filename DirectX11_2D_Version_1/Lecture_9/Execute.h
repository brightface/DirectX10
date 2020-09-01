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

	//�̱״�� �����ϱ� �������·� ����. UINT���̴�.
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

//���ð���/ <������>
//����/ <����>
//�� /<�þ߰���>
//��������/ <��������> ������ ���ߴ°� 3D�� 2D�� ��ȯ��Ű�°� 
//Ŭ��/ ������ �ڸ��� Ŭ���̶� �θ���.����ȭ��Ų��. -1~1����  // HOMOGENEOUS ��������  ������ǥ �ϳ��� ��ǥ�� �߰��ؼ� 1����ǥ���Ҷ��� 2�������� ǥ��
//1 �̸� ��ġ���� 0�̸� ���⺤�� (������ǥ)
//VS���� ���

//RS���� ���
//����/ -1~1���� //EUCLIDEAN SPACE ǥ�� (�̰��� 1�� ���Ͻ�Ŵ)
//����Ʈ //������ ����ȭ ������? �����Ͷ���¡ �Ѱ� �ٽ� �÷��ش�. 