#include "stdafx.h"
#include "AnimationDemo.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(19, 2, 0);
	Context::Get()->GetCamera()->Position(1, 7, -19);


	shader = new Shader(L"30_Animation.fxo");

	Kachujin();
}

void AnimationDemo::Update()
{
	static UINT instance = 20;
	static int clip = 0;
	static float speed = 1.0f;
	static float takeTime = 1.0f;

	static bool bBlendMode = false;
	static float blendAlpha = 0.0f;

	ImGui::Checkbox("BlendMode", &bBlendMode);
	if (bBlendMode == false)
	{
		ImGui::InputInt("Instance", (int *)&instance);
		ImGui::InputInt("Clip", &clip);
		clip %= 5;

		ImGui::SliderFloat("Speed", &speed, 0.1f, 5.0f);
		ImGui::SliderFloat("TakeTime", &takeTime, 0.1f, 5.0f);

		if (ImGui::Button("Apply"))
			kachujin->PlayTweenMode(instance, clip, speed, takeTime);
	}
	else
	{
		ImGui::InputInt("Instance", (int *)&instance);

		ImGui::SliderFloat("Alpha", &blendAlpha, 0.0f, 2.0f);
		kachujin->SetBlendAlpha(instance, blendAlpha);

		if (ImGui::Button("Apply"))
			kachujin->PlayBlendMode(instance, 0, 1, 3);
	}


	if (kachujin != NULL)
	{
		Matrix bones[MAX_MODEL_TRANSFORMS];

		UINT count = kachujin->GetTransformCount();
		for (UINT i = 0; i < count; i++)
		{
			kachujin->GetAttachBones(i, bones);

			Transform* weaponTransform = weapon->GetTransform(i);
			weaponTransform->World(weaponInitTransform->World() * bones[40]);

			colliderObject[i]->Collider->GetTransform()->World(bones[40]);
			colliderObject[i]->Collider->Update();
		}
		weapon->UpdateTransforms();
		
		kachujin->Update();
		weapon->Update();
	}
}

void AnimationDemo::Render()
{
	if (kachujin != NULL)
	{
		UINT count = kachujin->GetTransformCount();

		for(UINT i = 0; i < count; i++)
			colliderObject[i]->Collider->Render();

		kachujin->Pass(2);
		kachujin->Render();

		weapon->Pass(1);
		weapon->Render();
	}
}

void AnimationDemo::Kachujin()
{
	//Animation
	{
		kachujin = new ModelAnimator(shader);
		kachujin->ReadMesh(L"Kachujin/Mesh");
		kachujin->ReadMaterial(L"Kachujin/Mesh");
		kachujin->ReadClip(L"Kachujin/Idle");
		kachujin->ReadClip(L"Kachujin/Walk");
		kachujin->ReadClip(L"Kachujin/Run");
		kachujin->ReadClip(L"Kachujin/Slash");
		kachujin->ReadClip(L"Kachujin/HipHop");

		for (float x = -50; x <= 50; x += 2.5f)
		{
			Transform* transform = kachujin->AddTransform();
			transform->Position(x, 0, -5);
			transform->Scale(0.01f, 0.01f, 0.01f);
		}
		kachujin->UpdateTransforms();
	}
	
	
	UINT count = kachujin->GetTransformCount();
	
	//Weapon
	{
		weapon = new ModelRender(shader);
		weapon->ReadMesh(L"Weapon/Sword");
		weapon->ReadMaterial(L"Weapon/Sword");

		for (UINT i = 0; i < count; i++)
			weapon->AddTransform();


		weaponInitTransform = new Transform();
		weaponInitTransform->Position(-2.9f, 1.45f, -6.45f);
		weaponInitTransform->Scale(0.5f, 0.5f, 0.75f);
		weaponInitTransform->Rotation(0, 0, 1);
	}

	//Collider
	{
		colliderObject = new ColliderObject*[count];
		for (UINT i = 0; i < count; i++)
		{
			colliderObject[i] = new ColliderObject();

			colliderObject[i]->Init = new Transform();
			colliderObject[i]->Init->Position(-2.9f, 1.45f, -50.0f);
			colliderObject[i]->Init->Scale(5, 5, 75);

			colliderObject[i]->Transform = new Transform();
			colliderObject[i]->Collider = new Collider(colliderObject[i]->Transform, colliderObject[i]->Init);
		}
	}
}
