#include "stdafx.h"
#include "TextureBufferDemo.h"
#include "TextureBufferDemo.h"

void TextureBufferDemo::Initialize()
{
	render2D = new Render2D();
	render2D->GetTransform()->Scale(D3D::Width(), D3D::Height(), 1);
	render2D->GetTransform()->Position(D3D::Width() * 0.5f, D3D::Height() * 0.5f, 0.0f);
	
	texture = new Texture(L"Environment/Compute.png");
	textureBuffer = new TextureBuffer(texture->GetTexture());

	shader = new Shader(L"24_Texture.fx");

	shader->AsSRV("Input")->SetResource(textureBuffer->SRV());
	shader->AsUAV("Output")->SetUnorderedAccessView(textureBuffer->UAV());


	render2D->SRV(textureBuffer->OutputSRV());
}

void TextureBufferDemo::Update()
{
	UINT width = textureBuffer->Width();
	UINT height = textureBuffer->Height();
	UINT arraySize = textureBuffer->ArraySize();

	float x = ((float)width / 32.0f) < 1.0f ? 1.0f : ((float)width / 32.0f);
	float y = ((float)height / 32.0f) < 1.0f ? 1.0f : ((float)height / 32.0f);

	if (ImGui::Button("CS - 1"))
		shader->Dispatch(0, 0, (UINT)ceilf(x), (UINT)ceilf(y), arraySize);

	if (ImGui::Button("CS - 2"))
		shader->Dispatch(0, 1, (UINT)ceilf(x), (UINT)ceilf(y), arraySize);

	if (ImGui::Button("CS - 3"))
		shader->Dispatch(0, 2, (UINT)ceilf(x), (UINT)ceilf(y), arraySize);

	render2D->Update();
}

void TextureBufferDemo::Render()
{
	render2D->Render();
}

