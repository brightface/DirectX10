#include "stdafx.h"
#include "ModelDemo.h"

void ModelDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(24, 0, 0);
	Context::Get()->GetCamera()->Position(-3, 15, -34);


	shader = new Shader(L"19_Model.fx");
	Tank();
	/////////////////////////////////////////////////////
	//1. 모델이 어떻게 바닥에 그려지나
	//2. 메시는 왜 안채워지나
	//3. 어떤식으로 그려져서 옆으로 부딪치나
	//4. 포탄이 어떻게 나가는가?


	////////////////////////////////////////////////////

	//바닥
	gridShader = new Shader(L"16_Mesh.fx");
	//sky = new CubeSky(L"Environment/GrassCube1024.dds");

	grid = new MeshPlane(gridShader, 6, 6);
	grid->GetTransform()->Scale(12, 1, 12);
	grid->DiffuseMap(L"Floor.png");
}

void ModelDemo::Update()
{
	static Vector3 LightDirection = Vector3(-1, -1, +1);
	ImGui::SliderFloat3("LightDirection", LightDirection, -1, +1);
	shader->AsVector("LightDirection")->SetFloatVector(LightDirection);
	gridShader->AsVector("LightDirection")->SetFloatVector(LightDirection);

	static UINT pass = 0;
	ImGui::InputInt("Pass", (int *)&pass);
	pass %= 2;
	tank->Pass(pass);

	
	//sky->Update();
	grid->Update();

	if (tank != NULL)
	{
		ModelBone* bone = tank->GetModel()->BoneByIndex(10);
		//탱크 포신 돌린다.
		float rotation = sinf(Time::Get()->Running() + 90) * Math::PI * Time::Delta();
		
		Transform transform;
		transform.Rotation(0, rotation, 0);

		tank->UpdateTransform(bone, transform.World());
		tank->Update();
	}
}

void ModelDemo::Render()
{
	//sky->Render();
	grid->Render();

	if (tank != NULL)
		tank->Render();
}

void ModelDemo::Tank()
{
	tank = new ModelRender(shader);
	tank->ReadMesh(L"Tank/Tank");
}
