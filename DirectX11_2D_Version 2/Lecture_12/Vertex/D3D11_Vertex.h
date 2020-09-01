#pragma once

struct D3D11_VertexColor final
{
	D3D11_VertexColor()
		: position(0, 0, 0)
		, color(0, 0, 0, 0)
	{}

	D3D11_VertexColor(const D3DXVECTOR3& position, const D3DXCOLOR& color)
		: position(position)
		, color(color)
	{}

	D3DXVECTOR3 position;
	D3DXCOLOR color;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};

struct D3D11_VertexTexture final
{
	D3D11_VertexTexture()
		: position(0, 0, 0)
		, uv(0, 0)
	{}

	D3D11_VertexTexture(const D3DXVECTOR3& position, const D3DXVECTOR2& uv)
		: position(position)
		, uv(uv)
	{}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv; // texcoord

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};