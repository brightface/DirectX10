#include "stdafx.h"
#include "TerrainDemo.h"

void TerrainDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(6, 0, 0);
	Context::Get()->GetCamera()->Position(110, 50, -110);
	((Freedom *)Context::Get()->GetCamera())->Speed(50, 2);


	shader = new Shader(L"13_Terrain.fx");

	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
}

void TerrainDemo::Destroy()
{
	SafeDelete(shader);
	
	SafeDelete(terrain);
}

void TerrainDemo::Update()
{
	static UINT Pass = 0;
	ImGui::InputInt("Pass", (int *)&Pass);
	Pass %= 2;
	terrain->Pass() = Pass;

	terrain->Update();
}

void TerrainDemo::Render()
{
	terrain->Render();
}