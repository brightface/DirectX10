#include "stdafx.h"
#include "BloomDemo.h"

void BloomDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(1, 36, -85);


	shader = new Shader(L"37_Billboard.fxo");
	

	float width = D3D::Width(), height = D3D::Height();

	renderTarget[0] = new RenderTarget((UINT)width, (UINT)height); //Diffuse
	renderTarget[1] = new RenderTarget((UINT)width, (UINT)height); //Luminosity
	renderTarget[2] = new RenderTarget((UINT)width, (UINT)height); //BlurX
	renderTarget[3] = new RenderTarget((UINT)width, (UINT)height); //BlurY
	renderTarget[4] = new RenderTarget((UINT)width, (UINT)height); //Composite
	renderTarget[5] = new RenderTarget((UINT)width, (UINT)height); //ColorGrading

	depthStencil = new DepthStencil((UINT)width, (UINT)height);
	viewport = new Viewport(width, height);


	render2D = new Render2D();
	render2D->GetTransform()->Position(200, 120, 0);
	render2D->GetTransform()->Scale(355, 200, 1);
	render2D->SRV(renderTarget[0]->SRV());

	postEffect = new PostEffect(L"44_Bloom.fxo");

	
	sky = new CubeSky(L"Environment/GrassCube1024.dds");
	
	AddBillboard();

	Mesh();
	Airplane();
	
	Kachujin();
	Weapon();

	PointLights();
	SpotLights();
}

void BloomDemo::Destroy()
{
	
}

void BloomDemo::Update()
{
	ImGui::InputFloat("Threshold", &threshold, 0.01f);
	postEffect->GetShader()->AsScalar("Threshold")->SetFloat(threshold);

	ImGui::InputInt("BlurCount", (int *)&blurCount, 2);
	blurCount = Math::Clamp<UINT>(blurCount, 1, 33);
	postEffect->GetShader()->AsScalar("BlurCount")->SetInt(blurCount);


	sky->Update();

	cube->Update();
	grid->Update();
	cylinder->Update();
	sphere->Update();

	airplane->Update();
	kachujin->Update();


	Matrix worlds[MAX_MODEL_TRANSFORMS];
	for (UINT i = 0; i < kachujin->GetTransformCount(); i++)
	{
		kachujin->GetAttachTransforms(i, worlds);
		weapon->GetTransform(i)->World(weaponTransform->World() * worlds[40]);
	}
	weapon->UpdateTransforms();
	weapon->Update();

	billboard->Update();

	render2D->Update();
	postEffect->Update();
}

void BloomDemo::PreRender()
{
	viewport->RSSetViewport();
	
	//Main Render
	{
		renderTarget[0]->PreRender(depthStencil);

		sky->Render();

		Pass(0, 1, 2);
		wall->Render();
		sphere->Render();

		brick->Render();
		cylinder->Render();

		stone->Render();
		cube->Render();

		floor->Render();
		grid->Render();

		airplane->Render();
		kachujin->Render();
		weapon->Render();


		billboard->Pass(3);
		billboard->Render();
	}


	Vector2 PixelSize = Vector2(1.0f / D3D::Width(), 1.0f / D3D::Height());
	postEffect->GetShader()->AsVector("PixelSize")->SetFloatVector(PixelSize);

	//Luminosity
	{
		renderTarget[1]->PreRender(depthStencil);

		postEffect->Pass(1);
		postEffect->SRV(renderTarget[0]->SRV());
		postEffect->Render();
	}


	SetBlur();

	//BlurX
	{
		postEffect->GetShader()->AsScalar("Weights")->SetFloatArray(&weightX[0], 0, weightX.size());
		postEffect->GetShader()->AsVector("Offsets")->SetRawValue(&offsetX[0], 0, sizeof(Vector2) * offsetX.size());


		renderTarget[2]->PreRender(depthStencil);
		viewport->RSSetViewport();

		postEffect->SRV(renderTarget[1]->SRV());
		postEffect->Pass(2);
		postEffect->Render();
	}

	//BlurY
	{
		postEffect->GetShader()->AsScalar("Weights")->SetFloatArray(&weightY[0], 0, weightY.size());
		postEffect->GetShader()->AsVector("Offsets")->SetRawValue(&offsetY[0], 0, sizeof(Vector2) * offsetY.size());


		renderTarget[3]->PreRender(depthStencil);
		viewport->RSSetViewport();

		postEffect->SRV(renderTarget[2]->SRV());
		postEffect->Pass(2);
		postEffect->Render();
	}

	//Comsite
	{
		renderTarget[4]->PreRender(depthStencil);
		viewport->RSSetViewport();

		postEffect->GetShader()->AsSRV("LuminosityMap")->SetResource(renderTarget[1]->SRV());
		postEffect->GetShader()->AsSRV("BlurMap")->SetResource(renderTarget[3]->SRV());

		postEffect->Pass(3);
		postEffect->Render();
	}

	//Color Grading
	{
		renderTarget[5]->PreRender(depthStencil);
		viewport->RSSetViewport();

		postEffect->Pass(4);
		postEffect->SRV(renderTarget[4]->SRV());
		postEffect->Render();
	}
}

void BloomDemo::Render()
{
	ImGui::SliderInt("Target Index", (int *)&targetIndex, 0, 5);

	postEffect->Pass(0);
	postEffect->SRV(renderTarget[targetIndex]->SRV());
	postEffect->Render();

	render2D->Render();
}

void BloomDemo::AddBillboard()
{
	billboard = new Billboard(shader);
	billboard->AddTexture(L"Terrain/grass_14.tga");
	billboard->AddTexture(L"Terrain/grass_07.tga");
	billboard->AddTexture(L"Terrain/grass_11.tga");

	for (UINT i = 0; i < 500; i++)
	{
		Vector2 scale = Math::RandomVec2(2, 4);
		Vector2 position = Math::RandomVec2(-60, 60);

		billboard->Add(Vector3(position.x, scale.y * 0.5f, position.y), scale, 0);
	}

	for (UINT i = 0; i < 200; i++)
	{
		Vector2 scale = Math::RandomVec2(2, 4);
		Vector2 position = Math::RandomVec2(-60, 60);

		billboard->Add(Vector3(position.x, scale.y * 0.5f, position.y), scale, 1);
	}

	for (UINT i = 0; i < 400; i++)
	{
		Vector2 scale = Math::RandomVec2(2, 4);
		Vector2 position = Math::RandomVec2(-60, 60);

		billboard->Add(Vector3(position.x, scale.y * 0.5f, position.y), scale, 1);
	}
}

void BloomDemo::Mesh()
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
		transform->Position(0, 5, 0);
		transform->Scale(20, 10, 20);

		grid = new MeshRender(shader, new MeshGrid(5, 5));
		transform = grid->AddTransform();
		transform->Position(0, 0, 0);
		transform->Scale(12, 1, 12);

		cylinder = new MeshRender(shader, new MeshCylinder(0.5f, 3.0f, 20, 20));
		sphere = new MeshRender(shader, new MeshSphere(0.5f, 20, 20));
		for (UINT i = 0; i < 5; i++)
		{
			transform = cylinder->AddTransform();
			transform->Position(-30, 6, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);

			transform = cylinder->AddTransform();
			transform->Position(30, 6, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);


			transform = sphere->AddTransform();
			transform->Position(-30, 15.5f, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);

			transform = sphere->AddTransform();
			transform->Position(30, 15.5f, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);
		}
	}

	sphere->UpdateTransforms();
	cylinder->UpdateTransforms();
	cube->UpdateTransforms();
	grid->UpdateTransforms();

	meshes.push_back(sphere);
	meshes.push_back(cylinder);
	meshes.push_back(cube);
	meshes.push_back(grid);
}

void BloomDemo::Airplane()
{
	airplane = new ModelRender(shader);
	airplane->ReadMesh(L"B787/Airplane");
	airplane->ReadMaterial(L"B787/Airplane");

	Transform* transform = airplane->AddTransform();
	transform->Position(2.0f, 9.91f, 2.0f);
	transform->Scale(0.004f, 0.004f, 0.004f);
	airplane->UpdateTransforms();

	models.push_back(airplane);
}

void BloomDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Sword And Shield Idle");
	kachujin->ReadClip(L"Kachujin/Sword And Shield Walk");
	kachujin->ReadClip(L"Kachujin/Sword And Shield Run");
	kachujin->ReadClip(L"Kachujin/Sword And Shield Slash");
	kachujin->ReadClip(L"Kachujin/Salsa Dancing");


	Transform* transform = NULL;

	transform = kachujin->AddTransform();
	transform->Position(0, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayTweenMode(0, 0, 1.0f);

	transform = kachujin->AddTransform();
	transform->Position(-15, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayTweenMode(1, 1, 1.0f);

	transform = kachujin->AddTransform();
	transform->Position(-30, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayTweenMode(2, 2, 0.75f);

	transform = kachujin->AddTransform();
	transform->Position(15, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayBlendMode(3, 0, 1, 2);
	kachujin->SetBlendAlpha(3, 1.5f);

	transform = kachujin->AddTransform();
	transform->Position(30, 0, -32.5f);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayTweenMode(4, 4, 0.75f);

	kachujin->UpdateTransforms();

	animators.push_back(kachujin);
}

void BloomDemo::Weapon()
{
	weapon = new ModelRender(shader);
	weapon->ReadMesh(L"Weapon/Sword");
	weapon->ReadMaterial(L"Weapon/Sword");

	UINT count = kachujin->GetTransformCount();
	for(UINT i = 0; i < count; i++)
		weapon->AddTransform();

	weapon->UpdateTransforms();
	models.push_back(weapon);

	weaponTransform = new Transform();
	weaponTransform->Position(-2.9f, 1.45f, -6.45f);
	weaponTransform->Scale(0.5f, 0.5f, 0.75f);
	weaponTransform->Rotation(0, 0, 1);
}

void BloomDemo::PointLights()
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

void BloomDemo::SpotLights()
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

void BloomDemo::Pass(UINT mesh, UINT model, UINT anim)
{
	for (MeshRender* temp : meshes)
		temp->Pass(mesh);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

void BloomDemo::SetBlur()
{
	float x = 1.0f / D3D::Width();
	float y = 1.0f / D3D::Height();

	GetBlurParameter(weightX, offsetX, x, 0);
	GetBlurParameter(weightY, offsetY, 0, y);
}

void BloomDemo::GetBlurParameter(vector<float>& weights, vector<Vector2>& offsets, float x, float y)
{
	weights.clear();
	weights.assign(blurCount, float());

	offsets.clear();
	offsets.assign(blurCount, Vector2());

	weights[0] = GetGaussFunction(0); //1
	offsets[0] = Vector2(0, 0);


	float sum = weights[0];
	for (UINT i = 0; i < blurCount / 2; i++)
	{
		float temp = GetGaussFunction((float)(i + 1));

		weights[i * 2 + 1] = temp;
		weights[i * 2 + 2] = temp;
		sum += temp * 2;

		Vector2 temp2 = Vector2(x, y) * (i * 2 + 1.5f);
		offsets[i * 2 + 1] = temp2;
		offsets[i * 2 + 2] = -temp2;
	}

	for (UINT i = 0; i < blurCount; i++)
		weights[i] /= sum;
}

float BloomDemo::GetGaussFunction(float val)
{
	return (float)((1.0 / sqrt(2 * Math::PI * blurCount)) * exp(-(val * val) / (2 * blurCount * blurCount)));
}
