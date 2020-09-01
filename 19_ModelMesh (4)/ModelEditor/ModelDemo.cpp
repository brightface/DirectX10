#include "stdafx.h"
#include "ModelDemo.h"
void ModelDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(1, 36, -85);


	shader = new Shader(L"19_Model.fx");
	Tank();

	gridShader = new Shader(L"16_Mesh.fx");

	sky = new CubeSky(L"Environment/GrassCube1024.dds");

	grid = new MeshGrid(gridShader, 6, 6);
	grid->GetTransform()->Scale(12, 1, 12);
	grid->DiffuseMap(L"Floor.png");
}

void ModelDemo::Update()
{
	static Vector3 LightDirection = Vector3(-1, -1, +1);
	Imgui::Slati

		//grid Shader에서 넘어가야돼

		static UINT pass = 0; 


	sky->Update();
	grid->Update();
	if (tank != NULL) {
		tank - Update();
	}
}

void ModelDemo::Tank()
{
	tank = new ModelRender(shaer);
	tnak->ReadMesh(L"Tank/Tank");
}