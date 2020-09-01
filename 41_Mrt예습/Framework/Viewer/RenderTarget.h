#pragma once

class RenderTarget
{
public:
	RenderTarget(UINT width = 0, UINT height = 0, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
	~RenderTarget();

	ID3D11RenderTargetView* RTV() { return rtv; }
	ID3D11ShaderResourceView* SRV() { return srv; }

	void SaveTexture(wstring file);

	void PreRender(class DepthStencil* depthStencil);
	static void PreRender(RenderTarget** targets, UINT count, class DepthStencil* depthStencil);
	//하나만 설정할수 있게 되어있잖아. 이거 스테닉이야, 이거 여러개 받을수 있도록 할수 있또록 배여로
private:
	UINT width, height;
	DXGI_FORMAT format;

	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;
};