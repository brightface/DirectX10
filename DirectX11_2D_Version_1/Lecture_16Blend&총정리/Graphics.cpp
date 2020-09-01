#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
	//dangling pointer 

	SAFE_RELEASE(render_target_view);
	SAFE_RELEASE(device);
	SAFE_RELEASE(device_context);
	SAFE_RELEASE(swap_chain);
}

void Graphics::Initialize()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferCount = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //랜더타겟뷰용으로 쓰겠다
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.OutputWindow = Settings::Get().GetWindowHandle();
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//SSAA
	//MSAA

	std::vector<D3D_FEATURE_LEVEL> feature_levels
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	auto hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		feature_levels.data(),
		1,
		D3D11_SDK_VERSION,
		&desc,
		&swap_chain,
		&device,
		nullptr,
		&device_context
	);
	assert(SUCCEEDED(hr));

}

void Graphics::CreateBackBuffer(const uint & width, const uint & height)
{
	auto hr = swap_chain->ResizeBuffers
	(
		0, 
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0
	);
	assert(SUCCEEDED(hr));

	ID3D11Texture2D* back_buffer = nullptr;
	hr = swap_chain->GetBuffer
	(
		0,
		__uuidof(ID3D11Texture2D),//IID_ID3D11Texture2D,  // UUID  GUID
		reinterpret_cast<void**>(&back_buffer)
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		back_buffer,
		nullptr,
		&render_target_view
	);
	assert(SUCCEEDED(hr));

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width	= static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	SAFE_RELEASE(back_buffer);
}

void Graphics::Begin()
{//내가 어떻게 그릴것인가 도화지 세팅 OM 총 8장가능, rander_targetView==render_target :  도화지
	device_context->OMSetRenderTargets(1, &render_target_view, nullptr);
	//랜더타켓과 같이 바껴 1개 주소
	device_context->RSSetViewports(1, &viewport);
	//세팅하고 혹시 머 있을지 모르니 지운다.
	device_context->ClearRenderTargetView(render_target_view, clear_color);
}
//처음엔 후면 그렸다.
//그다음에 전면버퍼로 보내준다.
void Graphics::End()
{
	//잘만들어졌는지 그리고 앞면으로 보내준다.
	auto hr = swap_chain->Present(1, 0);
	assert(SUCCEEDED(hr));
}

//                  ID3D11Resource

//ID3D11Buffer                       ID3D11Texture1D, ID3D11Texture2D, ID3D11Texture3D

//									//Resource View
                                    //- ID3D11RenderTargetView , ID3D11ShaderResourceView, ID3D11DepthStencilView, ID3D11UnorderedAccessView