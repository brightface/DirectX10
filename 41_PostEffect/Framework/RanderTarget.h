#pragma once
class RenderTarget
{
public:
private:
	UINT width, height;
	DXGI_FORMAT format;

	ID3D11Textrue* texture;
	ID3D11RenderTagetView* rtv;
	ID3D11ShaderResourceView* srv;

	void SaveTexture(Wstring file);
}