#include "stdafx.h"
#include "Map.h"

void Map::Initialize()
{
	shader = new Shader(L"30_Animation.fxo");

	sky = new CubeSky(L"Environment/GrassCube1024.dds");

	planeMaterial = new Material(shader);
	planeMaterial->DiffuseMap("Floor.png");

	plane = new MeshRender(shader, new MeshPlane());
	plane->AddTransform()->Scale(3, 3, 3);
	plane->UpdateTransforms();

}

void Map::Update()
{
	sky->Update();
	plane->Update();
}

void Map::Render()
{
	sky->Render();
	planeMaterial->Render();
	plane->Render();
}