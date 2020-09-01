#include "Framework.h"
#include "CubeSky.h"

CubeSky::CubeSky(wstring file)
{
	shader = new Shader(L"17_CubeSky.fx");
	sphere = new MeshRender(shader, new MeshSphere(0.5f));
	sphere->AddTransform(); //�ϳ��� �׸����ݾ�.

	
	file = L"../../_Textures/" + file;
	Check(D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), file.c_str(), NULL, NULL, &srv, NULL
	));

	sSrv = shader->AsSRV("SkyCubeMap");
	sphere->Pass(0);

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

	sphere->GetTransform(0)->Position(position);
	sphere->UpdateTransforms(); //�Ź�������Ʈ���� ��ġ�� �ٲ���� �̰ɹٲ��־��
	sphere->Update();
}

void CubeSky::Render()
{
	sSrv->SetResource(srv);
	sphere->Render();
}
