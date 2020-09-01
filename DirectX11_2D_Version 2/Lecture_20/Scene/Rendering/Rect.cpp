#include "stdafx.h"
#include "Rect.h"

Rect::Rect(Graphics * graphics)
{
	D3D11_Geometry<D3D11_VertexTexture> geometry;
	Geometry_Generator::CreateQuad(geometry);

	vertex_buffer = new D3D11_VertexBuffer(graphics);
	vertex_buffer->Create(geometry.GetVertices());

	index_buffer = new D3D11_IndexBuffer(graphics);
	index_buffer->Create(geometry.GetIndices());

	vertex_shader = new D3D11_Shader(graphics);
	vertex_shader->Create(ShaderScope_VS, "../_Assets/Shader/Texture.hlsl");

	pixel_shader = new D3D11_Shader(graphics);
	pixel_shader->Create(ShaderScope_PS, "../_Assets/Shader/Texture.hlsl");

	input_layout = new D3D11_InputLayout(graphics);
	input_layout->Create(D3D11_VertexTexture::descs, D3D11_VertexTexture::count, vertex_shader->GetShaderBlob());

	gpu_buffer = new D3D11_ConstantBuffer(graphics);
	gpu_buffer->Create<TRANSFORM_DATA>();

	rasterizer_state = new D3D11_RasterizerState(graphics);
	rasterizer_state->Create(D3D11_CULL_BACK, D3D11_FILL_SOLID);

	texture = new D3D11_Texture(graphics);
	texture->Create("../_Assets/Texture/Tree.png");

	sampler_state = new D3D11_SamplerState(graphics);
	sampler_state->Create(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	blend_state = new D3D11_BlendState(graphics);
	blend_state->Create(true);

	//Create World
	{
		D3DXMatrixIdentity(&world);

		//World
		D3DXMATRIX S;
		D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);

		D3DXMATRIX T;
		D3DXMatrixTranslation(&T, position.x, position.y, position.z);

		world = S * T;
	}
}

Rect::~Rect()
{
	SAFE_DELETE(blend_state);
	SAFE_DELETE(sampler_state);
	SAFE_DELETE(texture);
	SAFE_DELETE(rasterizer_state);
	SAFE_DELETE(gpu_buffer);
	SAFE_DELETE(input_layout);
	SAFE_DELETE(pixel_shader);
	SAFE_DELETE(vertex_shader);
	SAFE_DELETE(index_buffer);
	SAFE_DELETE(vertex_buffer);
}

void Rect::Update()
{
	D3DXMATRIX S;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);

	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	world = S * T;

	auto buffer = gpu_buffer->Map<TRANSFORM_DATA>();
	{
		D3DXMatrixTranspose(&buffer->world, &world);
	}
	gpu_buffer->Unmap();
}

void Rect::Render(D3D11_Pipeline * pipeline)
{
	D3D11_PipelineState pipeline_state;
	pipeline_state.input_layout			= input_layout;
	pipeline_state.primitive_topology	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipeline_state.vertex_shader		= vertex_shader;
	pipeline_state.pixel_shader			= pixel_shader;
	pipeline_state.rasterizer_state		= rasterizer_state;
	pipeline_state.blend_state			= blend_state;

	if (pipeline->Begin(pipeline_state))
	{
		pipeline->SetVertexBuffer(vertex_buffer);
		pipeline->SetIndexBuffer(index_buffer);
		pipeline->SetConstantBuffer(1, ShaderScope_VS, gpu_buffer);
		pipeline->SetShaderResource(0, ShaderScope_PS, texture);
		pipeline->SetSamplerState(0, ShaderScope_PS, sampler_state);
		pipeline->DrawIndexed(index_buffer->GetCount(), index_buffer->GetOffset(), vertex_buffer->GetOffset());

		pipeline->End();
	}
}
