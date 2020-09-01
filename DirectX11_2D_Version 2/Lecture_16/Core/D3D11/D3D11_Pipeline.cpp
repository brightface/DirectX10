#include "stdafx.h"
#include "D3D11_Pipeline.h"

D3D11_Pipeline::D3D11_Pipeline(Graphcis * graphics)
{
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

void D3D11_Pipeline::SetVertexBuffer(const VertexBuffer * buffer)
{
}

void D3D11_Pipeline::SetIndexBuffer(const IndexBuffer * buffer)
{
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
}

void D3D11_Pipeline::DrawIndexed(const uint & index_count, const uint & index_offset, const uint & vertex_offset)
{
}
