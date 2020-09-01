#include "stdafx.h"
#include "AnimationDemo.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(13, 2, 0);
	Context::Get()->GetCamera()->Position(29, 4, -12);


	shader = new Shader(L"21_Animation.fx");

	Kachujin();
}

void AnimationDemo::Update()
{
	static int clip = 0;
	static float speed = 1.0f;
	static float takeTime = 1.0f;

	static bool bBlendMode = false;
	static float blendAlpha = 0.0f;

	ImGui::Checkbox("BlendMode", &bBlendMode);
	if (bBlendMode == false)
	{
		ImGui::InputInt("Clip", &clip);
		clip %= 5;

		ImGui::SliderFloat("Speed", &speed, 0.1f, 5.0f);
		ImGui::SliderFloat("TakeTime", &takeTime, 0.1f, 5.0f);

		if (ImGui::Button("Apply"))
			kachujin->PlayTweenMode(clip, speed, takeTime);
	}
	else
	{
		ImGui::SliderFloat("Alpha", &blendAlpha, 0.0f, 2.0f);
		kachujin->SetBlendAlpha(blendAlpha);

		if (ImGui::Button("Apply"))
			kachujin->PlayBlendMode(0, 1, 2);
	}



	ImGui::SliderFloat3("Direction2", direction, -1, +1);
	shader->AsVector("LightDirection")->SetFloatVector(direction);

	if (kachujin != NULL)
		kachujin->Update();
}

void AnimationDemo::Render()
{
	if (kachujin != NULL)
	{
		static int pass = 0;
		ImGui::InputInt("Animation Pass", &pass);
		pass %= 2;
		kachujin->Pass(pass);

		kachujin->Render();
	}
}

void AnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Walk");
	//kachujin->ReadClip(L"Kachujin/Run");
	//kachujin->ReadClip(L"Kachujin/Slash");
	//kachujin->ReadClip(L"Kachujin/HipHop");

	kachujin->GetTransform()->Position(30, 0, 0);
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}
