#include "stdafx.h"
#include "ModelDemo.h"

void ModelDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(24, 0, 0);
	Context::Get()->GetCamera()->Position(-3, 15, -34);


	shader = new Shader(L"19_Model.fx");

	Tank();

	
	gridShader = new Shader(L"16_Mesh.fx");
	sky = new CubeSky(L"Environment/GrassCube1024.dds");

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

	
	sky->Update();
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
	sky->Render();
	grid->Render();

	if (tank != NULL)
		tank->Render();
}
//assimp = fbx아니어도 등등등 다 지원해줘 다 공통적인 형태를 다 만들어놔
//최상위는 SCENE이다. 스튜디오 에다가 캐릭터 하나 얹혀놓고 
//SCENE에는 엄청많은 정보가 들어가 있다.
//카메라는 우리카메라 쓸거야. 우리가 필요한 정보만 가져올거야. 
//SCNEE 안에 MESHES 
//			ROOT NODE
//				NODE안에는 MATRIX랑 정점의 번호가 들어가 있다.

//우리가 원하는 형태로 저장해 쓸거야

//FRAMEWORK 게임 모듈 하나 있을거야
	//GAME PROJECT 
void ModelDemo::Tank()
{
	tank = new ModelRender(shader);
	tank->ReadMesh(L"Tank/Tank");
	tank->ReadMaterial(L"Tank/Tank");
	//tank->GetTransform()
}

//as 모델파일 읽을거고 그 정보 가져와서 
//우리가 원하는 형태로 저장을 해놓을거야 tank.mesh 파일 만들어주기
//여기까지 modelEditor 가 할일

//framework가 이파일을 가지고 랜더링을 할거야
