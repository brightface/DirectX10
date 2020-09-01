#include "stdafx.h"
#include "D3D11_Pipeline.h"

D3D11_Pipeline::D3D11_Pipeline(Graphics * graphics)
{
	device_context = graphics->GetDeviceContext();
}

D3D11_Pipeline::~D3D11_Pipeline()
{
}

void D3D11_Pipeline::Begin(const D3D11_PipelineState & pipeline_state)
{
}

void D3D11_Pipeline::End()
{
}

void D3D11_Pipeline::SetVertexBuffer(const D3D11_VertexBuffer * buffer)
{
	if (!buffer || !buffer->GetResource())
	{
		assert(false);
		return;
	}

	if (bind_vertex_buffer_id == buffer->GetID())
		return;

	ID3D11Buffer* buffers[]{ buffer->GetResource(), };
	device_context->IASetVertexBuffers(0, 1, buffers, &buffer->GetStride(), &buffer->GetOffset());
	bind_vertex_buffer_id = buffer->GetID();
}

void D3D11_Pipeline::SetIndexBuffer(const D3D11_IndexBuffer * buffer)
{
	if (!buffer || !buffer->GetResource())
	{
		assert(false);
		return;
	}

	if (bind_index_buffer_id == buffer->GetID())
		return;

	device_context->IASetIndexBuffer(buffer->GetResource(), DXGI_FORMAT_R32_UINT, buffer->GetOffset());
	bind_index_buffer_id = buffer->GetID();
}

void D3D11_Pipeline::SetConstantBuffer(const uint & slot, const ShaderScope & scope, const D3D11_ConstantBuffer * buffer)
{
}

void D3D11_Pipeline::SetShaderResource(const uint & slot, const ShaderScope & scope, const D3D11_Texture * resource)
{
}

void D3D11_Pipeline::SetSamplerState(const uint & slot, const ShaderScope & scope, const D3D11_SamplerState * state)
{
}

void D3D11_Pipeline::Draw(const uint & vertex_count, const uint & vertex_offset)
{
	device_context->Draw(vertex_count, vertex_offset);
}

void D3D11_Pipeline::DrawIndexed(const uint & index_count, const uint & index_offset, const uint & vertex_offset)
{
	device_context->DrawIndexed(index_count, index_offset, vertex_offset);
}
