#pragma once
class Depthstencil
{
public:
private:

	UINT width, height;
	DXGI_FORMAT format;

	ID3D11Texture2D* Texture() {return };
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* SRV;

	void SaveTexture(Wstring file);
public:
	DepthStencil(UINT width = 0, UINT height = 0, bool bUseStencil = false);
	~DepthStencil();

	ID3D11ShaderResourceView* SRV() { return srv; }
	void SaveTexture(wstring saveFile);

	ID3D11DepthStencilView* DSV() { return dsv; }
}