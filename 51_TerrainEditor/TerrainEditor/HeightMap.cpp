#include "stdafx.h"
#include "HeightMap.h"

void HeightMap::Initialize()
{
	Gray256();
}

void HeightMap::Gray256()
{
	//ToPng(L"Gray256.png", L"Gray256", Channel::Red); //RGB
	ToPng(L"ireland.jpg", L"Ireland", Channel::Red); //RGB
}

void HeightMap::ToPng(wstring heightFile, wstring fileName, Channel channel)
{
	heightFile = L"Terrain/" + heightFile;

	Texture* texture = new Texture(heightFile);
	ID3D11Texture2D* srcTexture = texture->GetTexture();
	
	D3D11_TEXTURE2D_DESC srcDesc;
	srcTexture->GetDesc(&srcDesc);

	
	ID3D11Texture2D* readTexture;
	D3D11_TEXTURE2D_DESC readDesc;
	ZeroMemory(&readDesc, sizeof(D3D11_TEXTURE2D_DESC));
	readDesc.Width = srcDesc.Width;
	readDesc.Height = srcDesc.Height;
	readDesc.ArraySize = 1;
	readDesc.Format = srcDesc.Format;
	readDesc.MipLevels = 1;
	readDesc.SampleDesc = srcDesc.SampleDesc;
	readDesc.Usage = D3D11_USAGE_STAGING;
	readDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	Check(D3D::GetDevice()->CreateTexture2D(&readDesc, NULL, &readTexture));
	D3D::GetDC()->CopyResource(readTexture, srcTexture);
	
	
	UINT* pixels = new UINT[readDesc.Width * readDesc.Height];

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(readTexture, 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(pixels, subResource.pData, sizeof(UINT) * readDesc.Width * readDesc.Height);
	}
	D3D::GetDC()->Unmap(readTexture, 0);
	
	
	UINT* heights = new UINT[readDesc.Width * readDesc.Height];
	for (UINT i = 0; i < readDesc.Width * readDesc.Height; i++)
	{
		UINT pixel = pixels[i];
		UINT result = 0;

		switch (channel)
		{
			case Channel::Red: result = (0xFF000000 & pixel) >> 24;
			case Channel::Green: result = (0x00FF0000 & pixel) >> 16;
			case Channel::Blue: result = (0x0000FF00 & pixel) >> 8;
			case Channel::Alpha: result = (0x000000FF & pixel) >> 0;
		}

		heights[i] = result << 24;
	}


	ID3D11Texture2D* saveTexture;
	D3D11_TEXTURE2D_DESC saveDesc;
	ZeroMemory(&saveDesc, sizeof(D3D11_TEXTURE2D_DESC));
	saveDesc.Width = readDesc.Width;
	saveDesc.Height = readDesc.Height;
	saveDesc.ArraySize = 1;
	saveDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	saveDesc.MipLevels = 1;
	saveDesc.SampleDesc = readDesc.SampleDesc;
	saveDesc.Usage = D3D11_USAGE_STAGING;
	saveDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA saveSubResource;
	saveSubResource.pSysMem = heights;
	saveSubResource.SysMemPitch = sizeof(UINT) * readDesc.Width;
	saveSubResource.SysMemSlicePitch = sizeof(UINT) * readDesc.Width * readDesc.Height;

	Check(D3D::GetDevice()->CreateTexture2D(&readDesc, &saveSubResource, &saveTexture));


	fileName = L"../../_Textures/Terrain/" + fileName + L".dds";
	Check(D3DX11SaveTextureToFile(D3D::GetDC(), saveTexture, D3DX11_IFF_DDS, fileName.c_str()));
}