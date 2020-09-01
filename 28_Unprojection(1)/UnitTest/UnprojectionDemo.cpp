#include "stdafx.h"
#include "UnprojectionDemo.h"

void UnprojectionDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(1, 36, -85);
	((Freedom *)Context::Get()->GetCamera())->Speed(50, 2);

	
	shader = new Shader(L"12_Terrain.fx");

	sky = new CubeSky(L"Environment/GrassCube1024.dds");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");

	terrain ->
}

void UnprojectionDemo::Update()
{
	sky->Update();
	terrain->Update();
}

void UnprojectionDemo::Render()
{
	sky->Render();
	terrain->Render();


	Gui::Get()-> RenderText(0, 100, 1, 0, 0);
	Vector3 position = terrain->GetRaycastPosition();

	string str = "";
	str += to_string(position.x) + ",";
	str += to_string(position.y) + ",";
	str += to_string(position.z);

	Gui::Get()->RenderText(Vector2(10, 60), Color(1, 0, 0, 1), str);
}

