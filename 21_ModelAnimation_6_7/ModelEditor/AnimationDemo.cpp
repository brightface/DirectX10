#include "stdafx.h"
#include "AnimationDemo.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(24, 0, 0);
	Context::Get()->GetCamera()->Position(-3, 15, -34);


	shader = new Shader(L"19_Model.fx");

	Kachujin();
}

void AnimationDemo::Update()
{
	static UINT clip = 0;
	//ImGui::InputFloat("Speed", &speed, )
	//if (Keyboard::Get()->Down(VK_SPACE))
	//{
	//	clip++;
	//	clip %= kachujin->GetModel();//
	//}
	//	
	//	ImGui::InputInt("Clip", );
	//if (kachujin != NULL)
	//	kachujin->Update();
}

void AnimationDemo::Render()
{
	if (kachujin != NULL)
		kachujin->Render();
}

void AnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	//ī���� �ؿ� �޽��ϰ� ���͸��� �θ� ����.
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");

	kachujin->GetTransform()->Position(30, 0, 0);
	//��ũ�� �̳� ũ�� ���ͼ� �ٿ����ž�
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
	//ReadClip�߰�
}
