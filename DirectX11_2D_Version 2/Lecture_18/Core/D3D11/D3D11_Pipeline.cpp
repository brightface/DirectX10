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

void D3D11_Pipeline::SetConstantBuffer(const uint & slot, const uint & scope, const D3D11_ConstantBuffer * buffer)
{
	if (!buffer || !buffer->GetResource())
	{
		assert(false);
		return;
	}

	if (slot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
	{
		assert(false);
		return;
	}

	ID3D11Buffer* constant_buffers[]{ buffer->GetResource(), };

	if (scope & ShaderScope_VS) device_context->VSSetConstantBuffers(slot, 1, constant_buffers);
	if (scope & ShaderScope_PS) device_context->PSSetConstantBuffers(slot, 1, constant_buffers);
}

void D3D11_Pipeline::SetShaderResource(const uint & slot, const uint & scope, const D3D11_Texture * resource)
{
	if (!resource || !resource->GetResource())
	{
		assert(false);
		return;
	}

	if (slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
	{
		assert(false);
		return;
	}

	ID3D11ShaderResourceView* shader_resources[]{ resource->GetResource(), };

	if (scope & ShaderScope_VS) device_context->VSSetShaderResources(slot, 1, shader_resources);
	if (scope & ShaderScope_PS) device_context->PSSetShaderResources(slot, 1, shader_resources);
}

void D3D11_Pipeline::SetSamplerState(const uint & slot, const uint & scope, const D3D11_SamplerState * state)
{
	if (!state || !state->GetResource())
	{
		assert(false);
		return;
	}

	if (slot >= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT)
	{
		assert(false);
		return;
	}

	ID3D11SamplerState* sampler_states[]{ state->GetResource(), };

	if (scope & ShaderScope_VS) device_context->VSSetSamplers(slot, 1, sampler_states);
	if (scope & ShaderScope_PS) device_context->PSSetSamplers(slot, 1, sampler_states);
}

void D3D11_Pipeline::Draw(const uint & vertex_count, const uint & vertex_offset)
{
	device_context->Draw(vertex_count, vertex_offset);
}

void D3D11_Pipeline::DrawIndexed(const uint & index_count, const uint & index_offset, const uint & vertex_offset)
{
	device_context->DrawIndexed(index_count, index_offset, vertex_offset);
}
