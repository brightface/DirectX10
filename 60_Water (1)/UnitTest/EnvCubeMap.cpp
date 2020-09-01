#include "Framework.h"
#include "EnvCubeMap.h"

EnvCubeMap::EnvCubeMap(Shader * shader, UINT width, UINT height)
{
	: shader(shader), position(0, 0, 0), width(width), height(height)
	{
		DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		//Create Texture2D - RTV
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = width;
			desc.Height = height;
			desc.ArraySize = 6;
			desc.Format = rtvFormat;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
			desc.MipLevels = 1;
			desc.SampleDesc.Count = 1;

			Check(D3D::GetDevice()->CreateTexture2D(&desc, NULL, &rtvTexture));
		}

		//Create RTV
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
			desc.Format = rtvFormat; //Env받을때 텍스처 큐브였다.
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY; //반드시 어레이 여야해
			desc.Texture2DArray.ArraySize = 6;

			Check(D3D::GetDevice()->CreateRenderTargetView(rtvTexture, &desc, &rtv));
		}

		//Create SRV
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			desc.Format = rtvFormat;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE; 
			desc.TextureCube.MipLevels = 1;

			Check(D3D::GetDevice()->CreateShaderResourceView(rtvTexture, &desc, &srv));
		}


		DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT;
		//Create Texture - DSV
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = width;
			desc.Height = height;
			desc.ArraySize = 6; //깊이도 6면이 생긴다. 그래야 1:1이 된다.
			desc.Format = dsvFormat;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE; //텍스처 큐브형 만들어//disv 큐브용
			desc.MipLevels = 1;
			desc.SampleDesc.Count = 1;

			Check(D3D::GetDevice()->CreateTexture2D(&desc, NULL, &dsvTexture));
		}

		//CreateDSV
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			desc.Format = dsvFormat;
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.ArraySize = 6;

			Check(D3D::GetDevice()->CreateDepthStencilView(dsvTexture, &desc, &dsv));
		}


		viewport = new Viewport((float)width, (float)height);

		buffer = new ConstantBuffer(&desc, sizeof(Desc));
		sBuffer = shader->AsConstantBuffer("CB_EnvCube");

		sSrv = shader->AsSRV("EnvCubeMap");
}

EnvCubeMap::~EnvCubeMap()
{
}

void EnvCubeMap::PreRender(Vector3 & position, Vector3 & scale, float zNear, float zFar, float fov)
{
	//얘가 핵심이야 6개 면만들어 주면 돼

	this->position = position;

	//Create ViewMatrix
	{
		float x = this->position.x;
		float x = this->position.x;
		float x = this->position.x;
	
		struct LookAt
		{
			Vector3 LookAt;
			Vector3 Up;

		}lookAt[6];
		//표면 에서부터 비출거야.
		lookAt[0] = { Vector3(x + scale.x,y,z),Vector3(0,1,0);
		lookAt[1] = { Vector3(x + scale.x,y,z),Vector3(0,1,0);
		lookAt[2] = { Vector3(x,y + scale.x,y,z),Vector3(0,1,-1);
		lookAt[3] = {
			lookAt[4]=
		//for 이거가지고 뷰행렬 만들면 되겠지.

		//프로젝션 만들고.
			projecction 
			buffer-Render()


	}

}

void EnvCubeMap::Render()
{
}
