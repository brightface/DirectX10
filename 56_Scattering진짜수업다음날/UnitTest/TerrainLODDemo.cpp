#include "stdafx.h"
#include "TerrainLODDemo.h"

void TerrainLODDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(21, 0, 0);
	Context::Get()->GetCamera()->Position(126, 35, 39);
	((Freedom *)Context::Get()->GetCamera())->Speed(100);


	shader = new Shader(L"50_Framework.fxo");
	sky = new CubeSky(L"Environment/GrassCube1024.dds", shader);
	
	terrain = new TerrainLOD(L"Terrain/Gray512.dds");
	terrain->BaseMap(L"Terrain/Dirt3.png");
}

void TerrainLODDemo::Destroy()
{

}

void TerrainLODDemo::Update()
{
	sky->Update();
	terrain->Update();
}

void TerrainLODDemo::PreRender()
{
	
}

void TerrainLODDemo::Render()
{
	sky->Pass(3);
	sky->Render();


	static UINT Pass = 0;
	ImGui::InputInt("Pass", (int *)&Pass);
	Pass %= 2;

	terrain->Pass(Pass);
	terrain->Render();
}

void TerrainLODDemo::PostRender()
{
	
}

