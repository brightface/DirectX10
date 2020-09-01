#include "stdafx.h"
#include "Viewer.h"

void Viewer::Initialize()
{
	particle = new Particle(L"Fire");

	Context::Get
		shader 

		sky->Update();

	grid->Update();
	sphere->Update();


	Vector3 position;
	sphere->GetTransform(0)->Position(&position);

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

	sphere->GetTransform(0)->Position(position);
	sphere->UpdateTransforms();

	void Viewer::Mesh()
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
		}


		//Create Mesh
		{
			Transform* transform = NULL;

			grid = new MeshRender(shader, new MeshGrid(5, 5));
			transform = grid->AddTransform();
			transform->Position(0, 0, 0);
			transform->Scale(12, 1, 12);

			sphere = new MeshRender(shader, new MeshSphere(0.5f, 20, 20));
			transform = sphere->AddTransform();
			transform->Position(0, 5, 0);
			transform->Scale(5, 5, 5);
		}

		sphere->UpdateTransforms();
		grid->UpdateTransforms();
	}
}

void Viewer::Update()
{
	particle->Add(Vector3(0, 0, 0));
	particle->Update();
}

void Viewer::Render()
{
	particle->Render();
}