#include "stdafx.h"
#include "EnvMapDemo.h"

void EnvMapDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(21, 0, 0);
	Context::Get()->GetCamera()->Position(126, 35, 39);
	((Freedom *)Context::Get()->GetCamera())->Speed(100);


	shader = new Shader(L"61_EnvCubeMap.fxo");
	shadow = new Shadow(shader, Vector3(128, 0, 128), 65);
	cubeMap = new EnvCubeMap(shader, 256, 256);


	sky = new Sky(shader);
	sky->RealTime(false, Math::PI - 1e-6f);
	
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");


	Mesh();
	Airplane();

	Kachujin();
	Kachujin2();
	Weapon();


	PointLights();
	SpotLights();
}

void EnvMapDemo::Destroy()
{

}

void EnvMapDemo::Update()
{
	ImGui::InputInt("Type", (int *)&cubeMap->Type());
	cubeMap->Type() %= 5;

	ImGui::InputFloat("Alpha", &cubeMap->Alpha(), 0.01f);
	ImGui::InputFloat("RefractAmount", &cubeMap->RefractAmount(), 0.01f);

	ImGui::InputFloat("FresnelAmount", &cubeMap->FresnelAmount(), 0.01f);
	ImGui::InputFloat("FresnelBias", &cubeMap->FresnelBias(), 0.01f);
	ImGui::InputFloat("FresnelScale", &cubeMap->FresnelScale(), 0.01f);


	Vector3 position;
	//sphere2->GetTransform(0)->Position(&position);
	//airplane->GetTransform(0)->Position(&position);
	kachujin2->GetTransform(0)->Position(&position);

	if (Keyboard::Get()->Press('L'))
		position.x += 20 * Time::Delta();
	else if (Keyboard::Get()->Press('J'))
		position.x -= 20 * Time::Delta();

	if (Keyboard::Get()->Press('I'))
		position.z += 20 * Time::Delta();
	else if (Keyboard::Get()->Press('K'))
		position.z -= 20 * Time::Delta();

	if (Keyboard::Get()->Press('O'))
		position.y += 20 * Time::Delta();
	else if (Keyboard::Get()->Press('U'))
		position.y -= 20 * Time::Delta();

	//sphere2->GetTransform(0)->Position(position);
	//sphere2->UpdateTransforms();

	//airplane->GetTransform(0)->Position(position);
	//airplane->UpdateTransforms();

	kachujin2->GetTransform(0)->Position(position);
	kachujin2->UpdateTransforms();


	sky->Update();

	cube->Update();
	cylinder->Update();
	sphere->Update();
	sphere2->Update();

	airplane->Update();
	kachujin->Update();
	kachujin2->Update();


	Matrix worlds[MAX_MODEL_TRANSFORMS];
	for (UINT i = 0; i < kachujin->GetTransformCount(); i++)
	{
		kachujin->GetAttachBones(i, worlds);
		weapon->GetTransform(i)->World(weaponTransform->World() * worlds[40]);
	}
	weapon->UpdateTransforms();
	weapon->Update();

	terrain->Update();
}

void EnvMapDemo::PreRender()
{
	//Depth
	{
		shadow->PreRender();

		Pass(0, 1, 2);

		wall->Render();
		sphere->Render();

		brick->Render();
		cylinder->Render();

		stone->Render();
		cube->Render();

		floor->Render();

		airplane->Render();
		kachujin->Render();
		weapon->Render();
	}

	
	sky->Pass(7);
	sky->PreRender();

	//Env Cube
	{
		//Vector3 p, s;
		//sphere2->GetTransform(0)->Position(&p);
		//sphere2->GetTransform(0)->Scale(&s);
		//cubeMap->PreRender(p, s, 0.1f, 1200, 0.5f);

		//Vector3 p, s;
		//airplane->GetTransform(0)->Position(&p);
		//airplane->GetTransform(0)->Scale(&s);
		//cubeMap->PreRender(p, s, 0.1f, 1200, 0.5f);

		Vector3 p, s;
		kachujin2->GetTransform(0)->Position(&p);
		kachujin2->GetTransform(0)->Scale(&s);
		cubeMap->PreRender(p, s, 0.1f, 1200, 0.5f);


		sky->Pass(7, 24, 25, 26);
		sky->Render();

		Pass(20, 21, 22);

		wall->Render();
		sphere->Render();

		brick->Render();
		cylinder->Render();

		stone->Render();
		cube->Render();

		floor->Render();

		airplane->Render();
		kachujin->Render();
		weapon->Render();

		terrain->Pass(23);
		terrain->Render();
	}
}

void EnvMapDemo::Render()
{
	sky->Pass(7);
	sky->Render();

	Pass(3, 4, 5);

	wall->Render();
	sphere->Render();

	brick->Render();
	cylinder->Render();

	stone->Render();
	cube->Render();

	floor->Render();

	//cubeMap->Render();
	//airplane->Pass(28);
	airplane->Render();
	
	
	kachujin->Render();
	weapon->Render();

	cubeMap->Render();
	kachujin2->Pass(29);
	kachujin2->Render();

	terrain->Pass(6);
	terrain->Render();

	
	//cubeMap->Render();
	//sphere2->Pass(27);
	//sphere2->Render();
}

void EnvMapDemo::PostRender()
{
	
}

void EnvMapDemo::Mesh()
{
	//Create Material
	{
		floor = new Material(shader);
		floor->DiffuseMap("Floor.png");
		floor->NormalMap("Floor_Normal.png");
		floor->Specular(1, 1, 1, 20);
		floor->SpecularMap("Floor_Specular.png");

		stone = new Material(shader);
		stone->DiffuseMap("Stones.png");
		stone->NormalMap("Stones_Normal.png");
		stone->Specular(1, 1, 1, 20);
		stone->SpecularMap("Stones_Specular.png");
		stone->Emissive(0.15f, 0.15f, 0.15f, 0.3f);

		brick = new Material(shader);
		brick->DiffuseMap("Bricks.png");
		brick->NormalMap("Bricks_Normal.png");
		brick->Specular(1, 0.3f, 0.3f, 20);
		brick->SpecularMap("Bricks_Specular.png");
		brick->Emissive(0.15f, 0.15f, 0.15f, 0.3f);

		wall = new Material(shader);
		wall->DiffuseMap("Wall.png");
		wall->NormalMap("Wall_Normal.png");
		wall->Specular(1, 1, 1, 20);
		wall->SpecularMap("Wall_Specular.png");
		wall->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
	}


	//Create Mesh
	{
		Transform* transform = NULL;

		cube = new MeshRender(shader, new MeshCube());
		transform = cube->AddTransform();
		transform->Position(128, 0, 128);
		transform->Scale(20, 10, 20);
		SetTransform(transform);

		cylinder = new MeshRender(shader, new MeshCylinder(0.5f, 3.0f, 20, 20));
		sphere = new MeshRender(shader, new MeshSphere(0.5f, 20, 20));
		for (UINT i = 0; i < 5; i++)
		{
			transform = cylinder->AddTransform();
			transform->Position(128 - 30, 6, 128 - 15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);
			SetTransform(transform);

			transform = cylinder->AddTransform();
			transform->Position(128 + 30, 6, 128 - 15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);
			SetTransform(transform);


			transform = sphere->AddTransform();
			transform->Position(128 - 30, 15.5f, 128 - 15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);
			SetTransform(transform);

			transform = sphere->AddTransform();
			transform->Position(128 + 30, 15.5f, 128 - 15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);
			SetTransform(transform);
		}


		sphere2 = new MeshRender(shader, new MeshSphere(0.5f));
		transform = sphere2->AddTransform();
		transform->Position(138, 10, 138);
		transform->Scale(5, 5, 5);
	}

	sphere->UpdateTransforms();
	cylinder->UpdateTransforms();
	cube->UpdateTransforms();
	sphere2->UpdateTransforms();

	meshes.push_back(sphere);
	meshes.push_back(cylinder);
	meshes.push_back(cube);
	meshes.push_back(sphere2);
}

void EnvMapDemo::Airplane()
{
	airplane = new ModelRender(shader);
	airplane->ReadMesh(L"B787/Airplane");
	airplane->ReadMaterial(L"B787/Airplane");

	Transform* transform = airplane->AddTransform();
	transform->Position(128 + 2.0f, 9.91f, 128 + 2.0f);
	transform->Scale(0.004f, 0.004f, 0.004f);
	SetTransform(transform);
	airplane->UpdateTransforms();

	models.push_back(airplane);
}

void EnvMapDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Walk");
	kachujin->ReadClip(L"Kachujin/Run");
	kachujin->ReadClip(L"Kachujin/Slash");
	kachujin->ReadClip(L"Kachujin/HipHop");


	Transform* transform = NULL;

	transform = kachujin->AddTransform();
	transform->Position(128 + 0, 0, 128 - 30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	SetTransform(transform);
	kachujin->PlayTweenMode(0, 0, 1.0f);

	transform = kachujin->AddTransform();
	transform->Position(128 - 15, 0, 128 - 30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	SetTransform(transform);
	kachujin->PlayTweenMode(1, 1, 1.0f);

	transform = kachujin->AddTransform();
	transform->Position(128 - 30, 0, 128 - 30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	SetTransform(transform);
	kachujin->PlayTweenMode(2, 2, 0.75f);

	transform = kachujin->AddTransform();
	transform->Position(128 + 15, 0, 128 - 30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	SetTransform(transform);
	kachujin->PlayBlendMode(3, 0, 1, 2);
	kachujin->SetBlendAlpha(3, 1.5f);

	transform = kachujin->AddTransform();
	transform->Position(128 + 30, 0, 128 - 32.5f);
	transform->Scale(0.075f, 0.075f, 0.075f);
	SetTransform(transform);
	kachujin->PlayTweenMode(4, 4, 0.75f);

	kachujin->UpdateTransforms();

	animators.push_back(kachujin);
}

void EnvMapDemo::Weapon()
{
	weapon = new ModelRender(shader);
	weapon->ReadMesh(L"Weapon/Sword");
	weapon->ReadMaterial(L"Weapon/Sword");

	UINT count = kachujin->GetTransformCount();
	for (UINT i = 0; i < count; i++)
		weapon->AddTransform();

	weapon->UpdateTransforms();
	models.push_back(weapon);

	weaponTransform = new Transform();
	weaponTransform->Position(-2.9f, 1.45f, -6.45f);
	weaponTransform->Scale(0.5f, 0.5f, 0.75f);
	weaponTransform->Rotation(0, 0, 1);
}

void EnvMapDemo::Kachujin2()
{
	kachujin2 = new ModelAnimator(shader);
	kachujin2->ReadMesh(L"Kachujin/Mesh");
	kachujin2->ReadMaterial(L"Kachujin/Mesh");
	kachujin2->ReadClip(L"Kachujin/Idle");
	kachujin2->ReadClip(L"Kachujin/Walk");
	kachujin2->ReadClip(L"Kachujin/Run");
	kachujin2->ReadClip(L"Kachujin/Slash");
	kachujin2->ReadClip(L"Kachujin/HipHop");


	Transform* transform = NULL;

	transform = kachujin2->AddTransform();
	transform->Position(128 - 30, 0, 128 - 50);
	transform->Scale(0.075f, 0.075f, 0.075f);
	SetTransform(transform);
	kachujin2->PlayTweenMode(2, 2, 0.75f);
	kachujin2->UpdateTransforms();

	animators.push_back(kachujin2);
}

void EnvMapDemo::PointLights()
{
	PointLight light;
	light =
	{
		Color(0.0f, 0.0f, 0.0f, 1.0f), //Ambient
		Color(0.0f, 0.0f, 1.0f, 1.0f), //Diffuse
		Color(0.0f, 0.0f, 0.7f, 1.0f), //Specular
		Color(0.0f, 0.0f, 0.7f, 1.0f), //Emissive
		Vector3(-30, 10, -30), 5.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);

	light =
	{
		Color(0.0f, 0.0f, 0.0f, 1.0f),
		Color(1.0f, 0.0f, 0.0f, 1.0f),
		Color(0.6f, 0.2f, 0.0f, 1.0f),
		Color(0.6f, 0.3f, 0.0f, 1.0f),
		Vector3(15, 10, -30), 10.0f, 0.3f
	};
	Lighting::Get()->AddPointLight(light);

	light =
	{
		Color(0.0f, 0.0f, 0.0f, 1.0f), //Ambient
		Color(0.0f, 1.0f, 0.0f, 1.0f), //Diffuse
		Color(0.0f, 0.7f, 0.0f, 1.0f), //Specular
		Color(0.0f, 0.7f, 0.0f, 1.0f), //Emissive
		Vector3(-5, 1, -17.5f), 5.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);

	light =
	{
		Color(0.0f, 0.0f, 0.0f, 1.0f),
		Color(0.0f, 0.0f, 1.0f, 1.0f),
		Color(0.0f, 0.0f, 0.7f, 1.0f),
		Color(0.0f, 0.0f, 0.7f, 1.0f),
		Vector3(-10, 1, -17.5f), 5.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);
}

void EnvMapDemo::SpotLights()
{
	SpotLight light;
	light =
	{
		Color(0.3f, 1.0f, 0.0f, 1.0f),
		Color(0.7f, 1.0f, 0.0f, 1.0f),
		Color(0.3f, 1.0f, 0.0f, 1.0f),
		Color(0.3f, 1.0f, 0.0f, 1.0f),
		Vector3(-15, 20, -30), 25.0f,
		Vector3(0, -1, 0), 30.0f, 0.9f
	};
	Lighting::Get()->AddSpotLight(light);

	light =
	{
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Vector3(0, 20, -30), 30.0f,
		Vector3(0, -1, 0), 40.0f, 0.9f
	};
	Lighting::Get()->AddSpotLight(light);
}

void EnvMapDemo::Pass(UINT mesh, UINT model, UINT anim)
{
	for (MeshRender* temp : meshes)
		temp->Pass(mesh);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

void EnvMapDemo::SetTransform(Transform * transform)
{
	Vector3 position;
	transform->Position(&position);

	Vector3 scale;
	transform->Scale(&scale);

	float y = terrain->GetHeight(position);
	position.y += y + scale.y * 0.5f;
	transform->Position(position);
}
