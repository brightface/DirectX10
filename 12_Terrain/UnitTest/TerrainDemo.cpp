#include "stdafx.h"
#include "TerrainDemo.h"

void TerrainDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(13, 0, 0);
	Context::Get()->GetCamera()->Position(54, 10, -17);
	((Freedom *)Context::Get()->GetCamera())->Speed(50, 2);


	shader = new Shader(L"12_Terrain.fx");

	terrain = new Terrain(shader, L"HeightTest256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
}

void TerrainDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void TerrainDemo::Update()
{
	terrain->Update();
}

void TerrainDemo::Render()
{
	terrain->Render();
}