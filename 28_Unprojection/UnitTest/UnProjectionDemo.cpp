#include "stdafx.h"
#include "UnProjectionDemo.h"

void UnProjectionDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(1, 36, -85);
	((Freedom *)Context::Get()->GetCamera())->Speed(50, 2);

	
	shader = new Shader(L"12_Terrain.fx");
	

	sky = new CubeSky(L"Environment/GrassCube1024.dds");
	//terrain = new Terrain(shader, L"Terrain/Gray256.png");
	

}

void UnProjectionDemo::Update()
{
	sky->Update();
	//terrain->Update();
}

void UnProjectionDemo::Render()
{
	sky->Render();
	//terrain->Render();

	
}


