#include "stdafx.h"
#include "ProjectionDemo.h"

void ProjectionDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(1, 36, -85);
	((Freedom *)Context::Get()->GetCamera())->Speed(50, 2);

	
	shader = new Shader(L"16_Mesh.fx");
	sDirection = shader->AsVector("LightDirection");

	sky = new CubeSky(L"Environment/GrassCube1024.dds");

	CreateMesh();


	marker = new MeshSphere(shader, 0.5f);
	marker->GetTransform()->Position(0, 15, 0);
}

void ProjectionDemo::Update()
{
	static float x = 0;
	static float y = 0;
	static UINT width = (UINT)D3D::Width();
	static UINT height = (UINT)D3D::Height();

	ImGui::InputFloat("X", &x, 10.0f);
	ImGui::InputFloat("Y", &y, 10.0f);
	ImGui::InputInt("Width", (int *)&width, 10);
	ImGui::InputInt("Height", (int *)&height, 10);

	Viewport* viewport = Context::Get()->GetViewport();
	viewport->Set(width, height, x, y);


	ImGui::SliderFloat3("Direction", direction, -1, +1);
	sDirection->SetFloatVector(direction);


	Vector3 position;
	marker->GetTransform()->Position(&position);

	if (Keyboard::Get()->Press(VK_RIGHT))
		position.x += 20 * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LEFT))
		position.x -= 20 * Time::Delta();

	if (Keyboard::Get()->Press(VK_UP))
		position.z += 20 * Time::Delta();
	else if (Keyboard::Get()->Press(VK_DOWN))
		position.z -= 20 * Time::Delta();

	marker->GetTransform()->Position(position);

	
	sky->Update();

	cube->Update();
	plane->Update();

	for (UINT i = 0; i < 10; i++)
	{
		cylinder[i]->Update();
		sphere[i]->Update();
	}

	marker->Update();
}

void ProjectionDemo::Render()
{
	sky->Render();
	
	cube->Render();
	plane->Render();

	for (UINT i = 0; i < 10; i++)
	{
		cylinder[i]->Render();
		sphere[i]->Render();
	}

	Vector3 position;
	marker->GetTransform()->Position(&position);

	Matrix W, V, P;
	D3DXMatrixIdentity(&W);

	Vector3 outPosition;
	V = Context::Get()->View();
	P = Context::Get()->Projection();
	Context::Get()->GetViewport()->Project(&outPosition, position, W, V, P);

	outPosition.y -= 40.0f;
	Gui::Get()->RenderText(outPosition.x, outPosition.y, 0, 1, 0, "Marker");

	marker->Render();
}

void ProjectionDemo::CreateMesh()
{
	cube = new MeshCube(shader);
	cube->GetTransform()->Position(0, 5, 0);
	cube->GetTransform()->Scale(20, 10, 20);
	cube->DiffuseMap(L"Stones.png");

	plane = new MeshPlane(shader, 2.5f, 2.5f);
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");

	for (UINT i = 0; i < 5; i++)
	{
		cylinder[i * 2] = new MeshCylinder(shader, 0.3f, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2]->GetTransform()->Position(-30, 6, (float)i * 15.0f - 15.0f);
		cylinder[i * 2]->GetTransform()->Scale(5, 5, 5);
		cylinder[i * 2]->DiffuseMap(L"Bricks.png");

		cylinder[i * 2 + 1] = new MeshCylinder(shader, 0.30f, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2 + 1]->GetTransform()->Position(30, 6, (float)i * 15.0f - 15.0f);
		cylinder[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		cylinder[i * 2 + 1]->DiffuseMap(L"Bricks.png");


		sphere[i * 2] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2]->GetTransform()->Position(-30, 15.5f, (float)i * 15.0f - 15.0f);
		sphere[i * 2]->GetTransform()->Scale(5, 5, 5);
		sphere[i * 2]->DiffuseMap(L"Wall.png");

		sphere[i * 2 + 1] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2 + 1]->GetTransform()->Position(30, 15.5f, (float)i * 15.0f - 15.0f);
		sphere[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		sphere[i * 2 + 1]->DiffuseMap(L"Wall.png");
	}
}
