#include "stdafx.h"
#include "CubeMapDemo.h"

void CubeMapDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(1, 36, -85);
	((Freedom *)Context::Get()->GetCamera())->Speed(50, 2);

	
	shader = new Shader(L"16_Mesh.fx");
	sDirection = shader->AsVector("LightDirection");

	sky = new CubeSky(L"Environment/GrassCube1024.dds");

	CreateMesh();


	cubeMapShader = new Shader(L"17_CubeMap0526.fx");
	cubeMap = new CubeMap(cubeMapShader);
	cubeMap->Texture(L"Environment/DesertCube1024.dds");
	cubeMap->Position(0, 20, 0);
	cubeMap->Scale(10, 10, 10);
}

void CubeMapDemo::Update()
{
	ImGui::SliderFloat3("Direction", direction, -1, +1);
	sDirection->SetFloatVector(direction);

	
	sky->Update();

	cube->Update();
	plane->Update();

	for (UINT i = 0; i < 10; i++)
	{
		cylinder[i]->Update();
		sphere[i]->Update();
	}

	cubeMap->Update();
}

void CubeMapDemo::Render()
{
	static bool bWireframe = false;
	ImGui::Checkbox("Wireframe", &bWireframe);

	cube->Pass(bWireframe ? 1 : 0);
	plane->Pass(bWireframe ? 1 : 0);
	
	for (UINT i = 0; i < 10; i++)
	{
		cylinder[i]->Pass(bWireframe ? 1 : 0);
		sphere[i]->Pass(bWireframe ? 1 : 0);
	}


	sky->Render();
	
	cube->Render();
	plane->Render();

	for (UINT i = 0; i < 10; i++)
	{
		cylinder[i]->Render();
		sphere[i]->Render();
	}

	cubeMap->Render();
}

void CubeMapDemo::CreateMesh()
{
	cube = new MeshCube(shader);
	cube->Position(0, 5, 0);
	cube->Scale(20, 10, 20);
	cube->DiffuseMap(L"Stones.png");

	plane = new MeshPlane(shader, 2.5f, 2.5f);
	plane->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");

	for (UINT i = 0; i < 5; i++)
	{
		cylinder[i * 2] = new MeshCylinder(shader, 0.3f, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2]->Position(-30, 6, (float)i * 15.0f - 15.0f);
		cylinder[i * 2]->Scale(5, 5, 5);
		cylinder[i * 2]->DiffuseMap(L"Bricks.png");

		cylinder[i * 2 + 1] = new MeshCylinder(shader, 0.30f, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2 + 1]->Position(30, 6, (float)i * 15.0f - 15.0f);
		cylinder[i * 2 + 1]->Scale(5, 5, 5);
		cylinder[i * 2 + 1]->DiffuseMap(L"Bricks.png");


		sphere[i * 2] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2]->Position(-30, 15.5f, (float)i * 15.0f - 15.0f);
		sphere[i * 2]->Scale(5, 5, 5);
		sphere[i * 2]->DiffuseMap(L"Wall.png");

		sphere[i * 2 + 1] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2 + 1]->Position(30, 15.5f, (float)i * 15.0f - 15.0f);
		sphere[i * 2 + 1]->Scale(5, 5, 5);
		sphere[i * 2 + 1]->DiffuseMap(L"Wall.png");
	}
}
