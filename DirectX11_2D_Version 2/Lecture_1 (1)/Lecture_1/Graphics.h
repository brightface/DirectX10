#pragma once

class Graphics final
{
public:
	Graphics();
	~Graphics();

	void Initialize();
	void CreateBackBuffer(const uint& width, const uint& height);

	auto GetDevice() -> ID3D11Device* { return device; }
	auto GetDeviceContext() -> ID3D11DeviceContext* { return device_context; }

	void Begin();
	void End();

private:
	ID3D11Device* device						= nullptr;
	ID3D11DeviceContext* device_context			= nullptr;
	IDXGISwapChain* swap_chain					= nullptr;
	ID3D11RenderTargetView* render_target_view	= nullptr;
	D3D11_VIEWPORT viewport						= { 0 };
	D3DXCOLOR clear_color						= 0xff555566;
};

//Rendering Pipeline
//IA - VS - RS - PS - OM

//DirectX - API
//OpenGL

//GPU - CPU