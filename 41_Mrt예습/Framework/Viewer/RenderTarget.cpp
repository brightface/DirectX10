#include "Framework.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height, DXGI_FORMAT format)
	: format(format)
{
	this->width = (width < 1) ? (UINT)D3D::Width() : width;
	this->height = (height < 1) ? (UINT)D3D::Height() : height;


	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = this->width;
	textureDesc.Height = this->height;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc.Count = 1;
	Check(D3D::GetDevice()->CreateTexture2D(&textureDesc, NULL, &texture));


	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	Check(D3D::GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &rtv));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));
}

RenderTarget::~RenderTarget()
{
	SafeRelease(texture);
	SafeRelease(rtv);
	SafeRelease(srv);
}

void RenderTarget::SaveTexture(wstring file)
{
	Check(D3DX11SaveTextureToFile(D3D::GetDC(), texture, D3DX11_IFF_PNG, file.c_str()));
}

void RenderTarget::PreRender(DepthStencil * depthStencil)
{
	D3D::GetDC()->OMSetRenderTargets(1, &rtv, depthStencil->DSV());
	D3D::Get()->Clear(Color(0, 0, 0, 1), rtv, depthStencil->DSV());
}

//랜더타켓 세팅하는대로 가서
void RenderTarget::PreRender(RenderTarget ** targets, UINT count, DepthStencil * depthStencil)
{
	//동적할당 하는것보다 벡터가 편할것같아서 시스템한테 이걸넘겨줘야하니까
	vector<ID3D11RenderTargetView*> rtvs;
	for (UINT i = 0; i < count; i++)
	{	
		//세팅하고 타겟에 있는 RTV 가져오면 되겠지.
		ID3D11RenderTargetView* rtv = targets[i]->RTV();
		rtvs.push_back(rtv); //rtv에 넣어놓고
		//클리어시켜주고 , 세팅은 뒤에 해놓지만 클리어 해놓지만 검은색으로 초기화
		D3D::GetDC()->ClearRenderTargetView(rtv, Color(0, 0, 0, 1));
	}
	//depthStencil 도 클리어 할거야. 이거 똑같이 콜해 Clear , 클리어할떄 뎁스하고 스텐실도 같이 클리어 해야돼,깊이를 처음에 1이라 해놓고,  처음에 0으로 만들어놓겠다는 이야기야.
	D3D::GetDC()->ClearDepthStencilView(depthStencil->DSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	//시작주소 넣는다. 앞에 여러개를 넣을수 있도록 개수를 넣는거야. 배열 모아놨자나 처음에 ,rtv[0] 그담에 depthStencil, 세팅할수 있도록 해놨어
	D3D::GetDC()->OMSetRenderTargets(rtvs.size(), &rtvs[0], depthStencil->DSV());
}
