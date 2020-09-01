#include "stdafx.h"
#include "MeshDemo.h"

void MeshDemo::Initialize()
{
	shader = new Shader(L"30_Animation.fxo");

	planeMaterial = new Material(shader);//쉐이더 하나 넣어주고
	planeMaterial->DiffuseMap("White.png"); //화이트 하나 넣어주고
	//메시랜더로 바꿨지 메시 플레인이 아니라
	plane = new MeshRender(shader, new MeshPlane());
	plane->AddTransform()->Scale(10, 1, 2); //한번하면 추가하면 추가한 스케일
	plane->UpdateTransforms();//한번만 위치 안바껴 plane은 여기서 한번만 콜해주면 돼

	//박스 메테리얼 마찬가지
	boxMaterial = new Material(shader);
	boxMaterial->DiffuseMap(L"Box.png");

	box = new MeshRender(shader, new MeshCube());
	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = box->AddTransform();

		transform->Scale(0.25f, 0.25f, 0.25f);
		transform->Position(Vector3(x, 0.25f * 0.5f, 0.0f));
		//transform->RotationDegree(0, Math::Random(-180, 180), 0);
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
	}
	box->UpdateTransforms();//다한다음에 한번만 하면돼
}

void MeshDemo::Update()
{
	plane->Update();
	box->Update();
}

void MeshDemo::Render()
{
	planeMaterial->Render();//메테리얼이 먼저 들어가야된다.
	plane->Render();

	boxMaterial->Render();
	box->Render();
}