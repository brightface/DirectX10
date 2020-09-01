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
	if (kachujin != NULL)
		kachujin->Update();
}

void AnimationDemo::Render()
{
	if (kachujin != NULL)
		kachujin->Render();
}

void AnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Slash");

	kachujin->GetTransform()->Position(30, 0, 0);
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}
