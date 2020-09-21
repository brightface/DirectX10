#include "stdafx.h"
#include "Character.h"

void Character::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(34, 0, 0);
	Context::Get()->GetCamera()->Position(0, 10, -19);


	shader = new Shader(L"30_Animation.fxo");

	//Kachujin();
	Player();
	Enemy();
}

void Character::Update()
{
	playerUpdate();
	enemyUpdate();
	
	if (enemyState == ENEMYDIE)
	{
		endTimer += Time::Delta();
		if (endTimer >= 3.0f)
		{
			if (MessageBoxA(D3D::GetDesc().Handle, "", "종료", MB_OK))
				exit(1);
		}
	}
}

void Character::Render()
{
	playerRender();
	enemyRender();
}

void Character::Kachujin()
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
		//kachujin->ReadClip(L"Kachujin/Back");
		//kachujin->ReadClip(L"Kachujin/Hit");

		/*for (float x = -50; x <= 50; x += 2.5f)
		{
			Transform* transform = kachujin->AddTransform();
			transform->Position(x, 0, -5);
			transform->Scale(0.01f, 0.01f, 0.01f);
		}
		kachujin->UpdateTransforms();
		*/
	}
	Transform* transform = kachujin->AddTransform();
	transform->Position(-1, 10, -5);
	transform->Scale(0.01f, 0.01f, 0.01f);
	transform->RotationDegree(0, playerRot, 0);
	kachujin->UpdateTransforms();

	UINT count = kachujin->GetTransformCount();

	//Weapon
	{
		weapon = new ModelRender(shader);
		weapon->ReadMesh(L"Weapon/Katana");
		weapon->ReadMaterial(L"Weapon/Katana");

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

void Character::Player()
{
	//Animation
	{
		player = new ModelAnimator(shader);
		player->ReadMesh(L"Kachujin/Mesh");
		player->ReadMaterial(L"Kachujin/Mesh");
		player->ReadClip(L"Kachujin/Idle");
		player->ReadClip(L"Kachujin/Walk");
		player->ReadClip(L"Kachujin/Run");
		player->ReadClip(L"Kachujin/Slash");
		//player->ReadClip(L"Kachujin/HipHop");
		//player->ReadClip(L"Kachujin/Back");
		//player->ReadClip(L"Kachujin/Hit");
		//player->ReadClip(L"Kachujin/Dead");

		/*for (float x = -50; x <= 50; x += 2.5f)
		{
			Transform* transform = player->AddTransform();
			transform->Position(x, 0, -5);
			transform->Scale(0.01f, 0.01f, 0.01f);
		}*/
		Transform* transform = player->AddTransform();
		transform->Position(-1, 0, -5);
		transform->Scale(0.01f, 0.01f, 0.01f);
		
		transform->RotationDegree(0, playerRot, 0);
		player->UpdateTransforms();
	}


	UINT count = player->GetTransformCount();

	//Weapon
	{
		weapon = new ModelRender(shader);
		weapon->ReadMesh(L"Weapon/Katana");
		weapon->ReadMaterial(L"Weapon/Katana");

		for (UINT i = 0; i < count; i++)
			weapon->AddTransform();

		//맨앞x가 위 아래이다.
		weaponInitTransform = new Transform();
		weaponInitTransform->Position(-9, 1, -5.0f);
		weaponInitTransform->Scale(0.4f, 0.4f, 0.5f);
		weaponInitTransform->Rotation(204.25f, 30, 90);
	}

	//Collider
	{
		colliderObject = new ColliderObject*[count];
		for (UINT i = 0; i < count; i++)
		{
			colliderObject[i] = new ColliderObject();

			colliderObject[i]->Init = new Transform();
			
			colliderObject[i]->Init->Position(-25, 10, -5.0f);
			colliderObject[i]->Init->Scale(5, 5, 75);
			colliderObject[i]->Init->Rotation(0, 30, 1);
			
			colliderObject[i]->Transform = new Transform();
			colliderObject[i]->Collider = new Collider(colliderObject[i]->Transform, colliderObject[i]->Init);
		}
	}
}

void Character::Enemy()
{
	//Animation
	{
		enemy = new ModelAnimator(shader);
		enemy->ReadMesh(L"Kachujin/Mesh");
		enemy->ReadMaterial(L"Kachujin/Mesh");
		enemy->ReadClip(L"Kachujin/Idle");
		enemy->ReadClip(L"Kachujin/Walk");
		enemy->ReadClip(L"Kachujin/Run");
		//enemy->ReadClip(L"Kachujin/Slash");
		//enemy->ReadClip(L"Kachujin/HipHop");
		//enemy->ReadClip(L"Kachujin/Back");
		//enemy->ReadClip(L"Kachujin/Hit");
		//enemy->ReadClip(L"Kachujin/Death");
		//enemy->ReadClip(L"Kachujin/Down");

		//for (float x = -50; x <= 50; x += 2.5f)
		//{
		//	Transform* transform = kachujin->AddTransform();
		//	transform->Position(x, 0, -5);
		//	transform->Scale(0.01f, 0.01f, 0.01f);
		//}
		Transform* transform = enemy->AddTransform();
		transform->Position(1, 0, -5);
		transform->Scale(0.01f, 0.01f, 0.01f);
		transform->RotationDegree(0, 90.0f, 0);
		enemy->UpdateTransforms();
	}


	UINT count = enemy->GetTransformCount();

	//Collider
	{
		enemyCollider = new ColliderObject*[count];
		for (UINT i = 0; i < count; i++)
		{
			enemyCollider[i] = new ColliderObject();

			enemyCollider[i]->Init = new Transform();
			enemyCollider[i]->Init->Position(0, 100, 0);
			enemyCollider[i]->Init->Scale(125, 200, 125);

			enemyCollider[i]->Transform = new Transform();
			enemyCollider[i]->Collider = new Collider(enemyCollider[i]->Transform, enemyCollider[i]->Init);
		}
	}

	//HPbar
	{
		FHP = new Material(shader);
		FHP->DiffuseMap("Red.png");

		MHP = new Material(shader);
		MHP->DiffuseMap("Green.png");

		FullHP = new MeshRender(shader, new MeshQuad());		
		myHP = new MeshRender(shader, new MeshQuad());


		for (UINT i = 0; i < count; i++)
		{
			Transform* transform = FullHP->AddTransform();
			transform->Position(1, 2, -4.999f);
			transform->Scale(1.0f, 0.2f, 1.0f);
			transform->Rotation(0, 0, 0);

			Transform* transform2 = myHP->AddTransform();
			transform2->Position(1, 2, -5);
			transform2->Scale(1.0f, 0.2f, 1.0f);
			transform2->Rotation(0, 0, 0);
		}
		FullHP->UpdateTransforms();
		myHP->UpdateTransforms();
	}
}

void Character::playerUpdate()
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
		clip %= 7;

		ImGui::SliderFloat("Speed", &speed, 0.1f, 5.0f);
		ImGui::SliderFloat("TakeTime", &takeTime, 0.1f, 5.0f);

		if (ImGui::Button("Apply"))
			player->PlayTweenMode(instance, clip, speed, takeTime);
	}
	else
	{
		ImGui::InputInt("Instance", (int *)&instance);

		ImGui::SliderFloat("Alpha", &blendAlpha, 0.0f, 2.0f);
		player->SetBlendAlpha(instance, blendAlpha);

		if (ImGui::Button("Apply"))
			player->PlayBlendMode(instance, 0, 1, 2);
	}

	

	if (player != NULL)
	{
		Matrix bones[MAX_MODEL_TRANSFORMS];

		UINT count = player->GetTransformCount();
		for (UINT i = 0; i < count; i++)
		{
			player->GetAttachBones(i, bones);

			Transform* weaponTransform = weapon->GetTransform(i);
			weaponTransform->World(weaponInitTransform->World() * bones[40]);

			colliderObject[i]->Collider->GetTransform()->World(bones[37]);
			colliderObject[i]->Collider->Update();

			Vector3 playerPos = Vector3(player->GetTransform(i)->World()._41, player->GetTransform(i)->World()._42, player->GetTransform(i)->World()._43);

			if (Keyboard::Get()->Press(VK_LEFT) && playerState == PLAYERIDLE)
			{
				if (Keyboard::Get()->Press(VK_LSHIFT))
				{
					isLeft = true;
					if (playerRot > 90.0f)
					{
						playerRot -= 0.5f;
					}
					player->GetTransform(i)->RotationDegree(0, playerRot, 0);
					playerPos.x -= speed * 2 * Time::Delta();
					player->PlayTweenMode(i, 2, speed, takeTime);
				}
				else
				{
					playerPos.x -= speed * Time::Delta();
					player->PlayTweenMode(i, 5, speed, takeTime);
				}
			}
			else if (Keyboard::Get()->Press(VK_RIGHT) && playerState == PLAYERIDLE)
			{
				if (Keyboard::Get()->Press(VK_LSHIFT))
				{
					playerPos.x += speed * 2 * Time::Delta();
					player->PlayTweenMode(i, 2, speed, takeTime);
				}
				else
				{
					playerPos.x += speed * Time::Delta();
					player->PlayTweenMode(i, 1, speed, takeTime);
				}
			}
			else if(playerState == PLAYERIDLE)
			{
				player->PlayTweenMode(i, 0, speed, takeTime);
				if (isLeft)
				{
					isLeft = true;
					if(playerRot < 270.0f)
						playerRot += 0.5f;
					player->GetTransform(i)->RotationDegree(0, playerRot, 0);
				}
			}
			if (Keyboard::Get()->Press(VK_SPACE) && playerState == PLAYERIDLE)
			{
				player->PlayTweenMode(i, 3, speed, takeTime);
				playerState = PLAYERATTACK;
			}
			if (playerState == PLAYERATTACK)
			{
				attackTime += Time::Delta();
				if (attackTime >= 0.7f)
				{
					attackTime = 0.0f;
					playerState = PLAYERIDLE;
					player->PlayTweenMode(i, 1, speed, takeTime);
				}
			}


			player->GetTransform(i)->Position(playerPos);
			player->UpdateTransforms();
		}
		weapon->UpdateTransforms();

		player->Update();
		weapon->Update();
	}
}

void Character::playerRender()
{
	if (player != NULL)
	{
		UINT count = player->GetTransformCount();

		for (UINT i = 0; i < count; i++)
			colliderObject[i]->Collider->Render();

		player->Pass(2);
		player->Render();

		weapon->Pass(1);
		weapon->Render();
	}
}

void Character::enemyUpdate()
{

	//enemy->PlayTweenMode(instance, clip, speed, takeTime);
	//enemy->SetBlendAlpha(instance, blendAlpha);
	//enemy->PlayBlendMode(instance, 0, 1, 2);
	//static float rot = 90.0f;
	//ImGui::SliderFloat("Rotation", &rot, 0.1f, 200.0f);
	
	if (enemy != NULL)
	{
		Matrix bones[MAX_MODEL_TRANSFORMS];

		UINT count = enemy->GetTransformCount();
		for (UINT i = 0; i < count; i++)
		{
			enemy->GetAttachBones(i, bones);

			//enemy->GetTransform(i)->Rotation(0, rot, 0);
			

			if (enemyCollider[i]->Collider->Intersection(colliderObject[0]->Collider) && playerState == PLAYERATTACK)
			{
				if (enemyState == ENEMYIDLE)
				{
					enemyState = ENEMYHIT;
					enemy[i].PlayTweenMode(i, 6, 0.5f, 1);
					myHP->GetTransform(i)->Scale(1.0f - ((6 - enemyHP) / 5), 0.2f, 1.0f - ((6 - enemyHP) / 5));
					myHP->GetTransform(i)->Position(1 - ((6 - enemyHP) / 9.5), 2, -5);
					myHP->UpdateTransforms();
					enemyHP -= 1;
				}
			}
			else if(enemyState == ENEMYIDLE)
			{
				enemy[i].PlayTweenMode(i, 0, 1, 1);
			}
			else if (enemyState == ENEMYDIE)
			{
				enemy[i].PlayTweenMode(i, 8, 1, 1);
			}
			if (enemyHP <= 0 && enemyState != ENEMYDIE)
			{
				HitTime += Time::Delta();
				if (HitTime >= 1)
				{
					HitTime = 0;
					enemyState = ENEMYDIE;
				}
				enemy[i].PlayTweenMode(i, 7, 1, 1);				
			}
			if (enemyState == ENEMYHIT)
			{
				HitTime += Time::Delta();
				if (HitTime >= 1)
				{
					HitTime = 0;
					enemyState = ENEMYIDLE;
				}
			}
			enemyCollider[i]->Collider->GetTransform()->World(bones[0]);
			enemyCollider[i]->Collider->Update();
			//FullHP->GetTransform(i)->Scale(0,0,0);
		}
		enemy->Update();
		FullHP->Update();
		myHP->Update();
	}
}

void Character::enemyRender()
{
	if (enemy != NULL)
	{
		UINT count = enemy->GetTransformCount();

		for (UINT i = 0; i < count; i++)
			enemyCollider[i]->Collider->Render();

		enemy->Pass(2);
		enemy->Render();

		FHP->Render();
		FullHP->Render();

		MHP->Render();
		myHP->Render();
	}
}
