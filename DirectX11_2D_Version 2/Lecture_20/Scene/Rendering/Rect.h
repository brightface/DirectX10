#pragma once

struct TRANSFORM_DATA
{
	D3DXMATRIX world; //4x4
};

class Rect final
{
public:
	Rect(class Graphics* graphics);
	~Rect();

	void SetPosition(const D3DXVECTOR3& position)	{ this->position = position; }
	void SetScale(const D3DXVECTOR3& scale)			{ this->scale = scale; }

	void Update();
	void Render(D3D11_Pipeline* pipeline);

private:
	D3D11_VertexBuffer* vertex_buffer		= nullptr;
	D3D11_IndexBuffer* index_buffer			= nullptr;
	D3D11_InputLayout* input_layout			= nullptr;
	D3D11_Shader* vertex_shader				= nullptr;
	D3D11_Shader* pixel_shader				= nullptr;
	D3D11_ConstantBuffer* gpu_buffer		= nullptr; // Constant Buffer
	D3D11_RasterizerState* rasterizer_state = nullptr;
	D3D11_Texture* texture					= nullptr;
	D3D11_SamplerState* sampler_state		= nullptr;
	D3D11_BlendState* blend_state			= nullptr;

	D3DXVECTOR3 position					= D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 scale						= D3DXVECTOR3(100, 100, 1);
	D3DXMATRIX world; //4x4
};