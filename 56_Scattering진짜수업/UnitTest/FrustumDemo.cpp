#include "stdafx.h"
#include "FrustumDemo.h"

void FrustumDemo::Initialize()
{
	gridShader = new Shader(L"33_Lighting.fxo");

	floor = new Material(gridShader);
	floor->DiffuseMap(L"White.png");

	plane = new MeshRender(gridShader, new MeshPlane());
	plane->AddTransform()->Scale(10, 10, 10);
	plane->UpdateTransforms();
	plane->Pass(0);


	camera = new Fixity();
	camera->Position(0, 0, -50);
	perspective = new Perspective(1024, 768, 1, zFar, Math::PI * fov);
	frustum = new Frustum(camera, perspective);
	//frustum = new Frustum(NULL, perspective);

	shader = new Shader(L"16_Mesh.fx");
	perFrame = new PerFrame(shader);

	red = new Material(shader);
	red->DiffuseMap("Red.png");

	for (float z = -50.0f; z <= 50.0f; z += 10)
	{
		for (float y = -50.0f; y <= 50.0f; y += 10)
		{
			for (float x = -50.0f; x <= 50.0f; x += 10)
			{
				Transform* transform = new Transform(shader);
				transform->Position(x, y, z);

				transforms.push_back(transform);
			}
		}
	}

	CreateMeshData();


	modelShader = new Shader(L"33_Lighting.fxo");
	model = new ModelRender(modelShader);
	model->ReadMesh(L"B787/Airplane");
	model->ReadMaterial(L"B787/Airplane");

	for (float z = -100; z <= 100; z += 30)
	{
		for (float y = -100; y <= 100; y += 30)
		{
			for (float x = -100; x <= 100; x += 30)
			{
				Transform* transform = model->AddTransform();
				transform->Position(x, y, z);
				transform->Scale(0.004f, 0.004f, 0.004f);
				transform->Rotation(0, Math::PI * 0.25f, 0);
			}
		}
	}
	model->UpdateTransforms();
}

void FrustumDemo::Update()
{
	ImGui::InputFloat("Far", &zFar, 1.0f);
	ImGui::InputFloat("Fov", &fov, 1e-3f);
	perspective->Set(1024, 768, 1, zFar, Math::PI * fov);

	frustum->Update();

	
	perFrame->Update();
	plane->Update();

	model->Update();
}

void FrustumDemo::Render()
{
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	floor->Render();
	plane->Render();

	perFrame->Render();
	vertexBuffer->Render();
	indexBuffer->Render();

	red->Render();


	UINT drawCount = 0;
	Vector3 position;
	for (Transform* transform : transforms)
	{
		transform->Position(&position);

		if (frustum->CheckPoint(position) == true)
		{
			transform->Update();
			transform->Render();

			shader->DrawIndexed(0, 0, 36);

			drawCount++;
		}
	}

	string str = to_string(drawCount) + " / " + to_string(transforms.size());
	Gui::Get()->RenderText(10, 60, 1, 0, 0, str);


	Plane planes[6];
	frustum->Planes(planes);

	Plane cullPlanes[4];
	cullPlanes[0] = planes[0];
	cullPlanes[1] = planes[1];
	cullPlanes[2] = planes[4];
	cullPlanes[3] = planes[5];

	memcpy(Context::Get()->Culling(), cullPlanes, sizeof(Plane) * 4);

	model->Pass(1);
	model->Render();
}

void FrustumDemo::CreateMeshData()
{
	vector<Mesh::MeshVertex> v;

	float w = 0.5f;
	float h = 0.5f;
	float d = 0.5f;

	//Front
	v.push_back(Mesh::MeshVertex(-w, -h, -d, 0, 1, 0, 0, -1, 1, 0, 0));
	v.push_back(Mesh::MeshVertex(-w, +h, -d, 0, 0, 0, 0, -1, 1, 0, 0));
	v.push_back(Mesh::MeshVertex(+w, +h, -d, 1, 0, 0, 0, -1, 1, 0, 0));
	v.push_back(Mesh::MeshVertex(+w, -h, -d, 1, 1, 0, 0, -1, 1, 0, 0));

	//Back
	v.push_back(Mesh::MeshVertex(-w, -h, +d, 1, 1, 0, 0, 1, -1, 0, 0));
	v.push_back(Mesh::MeshVertex(+w, -h, +d, 0, 1, 0, 0, 1, -1, 0, 0));
	v.push_back(Mesh::MeshVertex(+w, +h, +d, 0, 0, 0, 0, 1, -1, 0, 0));
	v.push_back(Mesh::MeshVertex(-w, +h, +d, 1, 0, 0, 0, 1, -1, 0, 0));

	//Top
	v.push_back(Mesh::MeshVertex(-w, +h, -d, 0, 1, 0, 1, 0, 1, 0, 0));
	v.push_back(Mesh::MeshVertex(-w, +h, +d, 0, 0, 0, 1, 0, 1, 0, 0));
	v.push_back(Mesh::MeshVertex(+w, +h, +d, 1, 0, 0, 1, 0, 1, 0, 0));
	v.push_back(Mesh::MeshVertex(+w, +h, -d, 1, 1, 0, 1, 0, 1, 0, 0));

	//Bottom
	v.push_back(Mesh::MeshVertex(-w, -h, -d, 1, 1, 0, -1, 0, -1, 0, 0));
	v.push_back(Mesh::MeshVertex(+w, -h, -d, 0, 1, 0, -1, 0, -1, 0, 0));
	v.push_back(Mesh::MeshVertex(+w, -h, +d, 0, 0, 0, -1, 0, -1, 0, 0));
	v.push_back(Mesh::MeshVertex(-w, -h, +d, 1, 0, 0, -1, 0, -1, 0, 0));

	//Left
	v.push_back(Mesh::MeshVertex(-w, -h, +d, 0, 1, -1, 0, 0, 0, 0, -1));
	v.push_back(Mesh::MeshVertex(-w, +h, +d, 0, 0, -1, 0, 0, 0, 0, -1));
	v.push_back(Mesh::MeshVertex(-w, +h, -d, 1, 0, -1, 0, 0, 0, 0, -1));
	v.push_back(Mesh::MeshVertex(-w, -h, -d, 1, 1, -1, 0, 0, 0, 0, -1));

	//Right
	v.push_back(Mesh::MeshVertex(+w, -h, -d, 0, 1, 1, 0, 0, 0, 0, 1));
	v.push_back(Mesh::MeshVertex(+w, +h, -d, 0, 0, 1, 0, 0, 0, 0, 1));
	v.push_back(Mesh::MeshVertex(+w, +h, +d, 1, 0, 1, 0, 0, 0, 0, 1));
	v.push_back(Mesh::MeshVertex(+w, -h, +d, 1, 1, 1, 0, 0, 0, 0, 1));


	Mesh::MeshVertex* vertices = new Mesh::MeshVertex[v.size()];
	UINT vertexCount = v.size();

	copy
	(
		v.begin(), v.end(),
		stdext::checked_array_iterator<Mesh::MeshVertex *>(vertices, vertexCount)
	);

	UINT indexCount = 36;
	UINT* indices = new UINT[indexCount]
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};


	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(Mesh::MeshVertex));
	indexBuffer = new IndexBuffer(indices, indexCount);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);
}

