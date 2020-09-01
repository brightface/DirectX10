#include "Framework.h"
#include "RanderTarget.h"
#include "DepthStencil.h"
#inlcude "RenderTarget.h"

RenderTarget(float width = 0, float height = 0, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM)
	
{
	this->width = (width < 1) ? (float)D3D::Width() : width;
	this->height = (height < 1) ? (float)D3D::Height() : height;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = (UINT)this->width;
	textureDesc.Height = (UINT)this->height;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc.Count = 1;
	Check(D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture));
	//쉐이더 리소스뷰에 연결했고 랜더타겟에 연결했고

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	Check(D3D::GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &rtv));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1; //쉐이더 쪽에서는 몇개인지 알아야하거든  민맵핑
	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));
}
~RenderTarget()
{
	SafeRelease(texture);
	SafeRelease(rtv);
	SafeRelease(srv)
}
Check(D3DX11SaveTextureToFile(D3D::GetDC(), texture, D3DX11_IFF_PNG, file.c_str()));
ID3D11RenderTargetView* RTV() { return rtv; }
ID3D11ShaderResourceView* SRV() { return srv; }

void SaveTexture(wstring file);

void PreRender(class DepthStencil* depthStencil->DSV()) {
	D3D::GetDC()->OMSetRenderTargets(1, &rtv);
	D3D::Get()->Clear(0, 0, 0, 1), depthStencil->DSV());
}

void RenderTarget::SaveTexture(Wstring file)
{
	Check(D3DX11SaveTextureToFile(D3D::GetDC(), texture, D3DX11_IFF_PNG, file.c_str()));
}

RenderTarget::DepthStencil(UINT width, UINT height, bool bUseStencil)
{
}

