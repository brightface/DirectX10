#include "stdafx.h"
#include "AnimationDemo.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(13, 2, 0);
	Context::Get()->GetCamera()->Position(29, 4, -12);


	shader = new Shader(L"30_Animation.fx");

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


	if (kachujin != NULL)
	{
		Matrix bones[MAX_MODEL_TRANSFORMS];
		kachujin->GetAttachBones(bones);
		colliderObject->Transform->World(bones[40]);
		colliderObject->Transform->Update();

		weapon->GetTransform()->World(weaponInitTransform->World() * bones[40]);

		kachujin->Update();
		weapon->Update();
	}
}

void AnimationDemo::Render()
{
	if (kachujin != NULL)
	{
		//Matrix bones[MAX_MODEL_TRANSFORMS];
		//kachujin->GetAttachBones(bones);
		//colliderObject->Transform->World(bones[40]);
		//colliderObject->Collider->Render();

		//Obb Raycast
		{
			if (Mouse::Get()->Down(0))
			{
				Vector3 position, direction;
				Context::Get()->GetViewport()->GetMouseRay(&position, &direction);

				float distance = 0.0f;
				bIntersect = colliderObject->Collider->Intersection(position, direction, &distance);

				Gui::Get()->RenderText(10, 100, 1, 0, 0, to_string(distance));
			}

			if (bIntersect == true)
				colliderObject->Collider->Render(Color(1, 0, 0, 1));
			else
				colliderObject->Collider->Render();
		}

		kachujin->Pass(2);
		kachujin->Render();

		weapon->Pass(1);
		weapon->Render();
	}
}

void AnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Walk");
	kachujin->ReadClip(L"Kachujin/Run");
	kachujin->ReadClip(L"Kachujin/Slash");
	kachujin->ReadClip(L"Kachujin/HipHop");

	kachujin->GetTransform()->Position(30, 0, 0);
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);


	weapon = new ModelRender(shader);
	weapon->ReadMesh(L"Weapon/Sword");
	weapon->ReadMaterial(L"Weapon/Sword");

	weaponInitTransform = new Transform();
	weaponInitTransform->Position(-2.9f, 1.45f, -6.45f);
	weaponInitTransform->Scale(0.5f, 0.5f, 0.75f);
	weaponInitTransform->Rotation(0, 0, 1);


	colliderInitTransform = new Transform();
	colliderInitTransform->Position(-2.9f, 1.45f, -50.0f);
	colliderInitTransform->Scale(5, 5, 75);

	colliderObject = new ColliderObject();
	colliderObject->Transform = new Transform();
	colliderObject->Collider = new Collider(colliderObject->Transform, colliderInitTransform);
}
