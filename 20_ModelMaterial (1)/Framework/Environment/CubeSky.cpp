#include "Framework.h"
#include "CubeSky.h"

CubeSky::	(wstring file)
{
	shader = new Shader(L"17_CubeSky.fx");
	sphere = new MeshSphere(shader, 0.5f);
	
	file = L"../../_Textures/" + file;
	Check(D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), file.c_str(), NULL, NULL, &srv, NULL
	));

	sSrv = shader->AsSRV("SkyCubeMap");


	//D3D11_RASTERIZER_DESC;
	//D3D11_DEPTH_STENCIL_DESC;
}

CubeSky::~CubeSky()
{
	SafeDelete(shader);

	SafeDelete(sphere);
	SafeRelease(srv);
}

void CubeSky::Update()
{
	Vector3 position;
	Context::Get()->GetCamera()->Position(&position);

	sphere->GetTransform()->Position(position);
	sphere->Update();
}

void CubeSky::Render()
{
	sSrv->SetResource(srv);
	sphere->Render();
}