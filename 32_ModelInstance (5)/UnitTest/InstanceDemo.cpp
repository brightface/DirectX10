#include "stdafx.h"
#include "InstanceDemo.h"

void InstanceDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -17);

	shader = new Shader(L"31_Instance.fx");

	perFrame = new PerFrame(shader);

	material = new Material(shader);
	material->DiffuseMap(L"Box.png");


	//No Instancing
	//for (UINT i = 0; i < InstanceDemo_MAX_COUNT; i++)
	//{
	//	transforms[i] = new Transform(shader);

	//	transforms[i]->Position(Math::RandomVec3(-30, 30));
	//	transforms[i]->Scale(Math::RandomVec3(1.0f, 2.5f));
	//	transforms[i]->RotationDegree(Math::RandomVec3(-180, 180));
	//}

	//Instancing 1
	//for (UINT i = 0; i < InstanceDemo_MAX_COUNT; i++)
	//{
	//	transforms[i] = new Transform();

	//	transforms[i]->Position(Math::RandomVec3(-30, 30));
	//	transforms[i]->Scale(Math::RandomVec3(1.0f, 2.5f));
	//	transforms[i]->RotationDegree(Math::RandomVec3(-180, 180));

	//	worlds[i] = transforms[i]->World();
	//	colors[i] = Math::RandomColor4();
	//}

	//Instancing 2
	for (UINT i = 0; i < InstanceDemo_MAX_COUNT; i++)
	{
		transforms[i] = new Transform();

		worlds[i] = transforms[i]->World();
		colors[i] = Math::RandomColor4();
	}

	CreateMesh();

	instanceBuffer = new VertexBuffer(worlds, InstanceDemo_MAX_COUNT, sizeof(Matrix), 1);
	colorBuffer = new VertexBuffer(colors, InstanceDemo_MAX_COUNT, sizeof(Color), 2);
}

void InstanceDemo::Update()
{
	perFrame->Update();

	//for (int i = 0; i < 1000; i++)
		//transforms[i]->Update();
}

void InstanceDemo::Render()
{
	perFrame->Render();

	material->Render();

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//for (int i = 0; i < InstanceDemo_MAX_COUNT; i++)
	//{
	//	transforms[i]->Render();
	//	shader->DrawIndexed(0, 0, indices.size());
	//}

	instanceBuffer->Render();
	colorBuffer->Render();
	shader->DrawIndexedInstanced(0, 0, indices.size(), InstanceDemo_MAX_COUNT);
}

void InstanceDemo::CreateMesh()
{
	float w, h, d;
	w = h = d = 0.5f;

	//Front
	vertices.push_back(Mesh::MeshVertex(-w, -h, -d, 0, 1, 0, 0, -1));
	vertices.push_back(Mesh::MeshVertex(-w, +h, -d, 0, 0, 0, 0, -1));
	vertices.push_back(Mesh::MeshVertex(+w, +h, -d, 1, 0, 0, 0, -1));
	vertices.push_back(Mesh::MeshVertex(+w, -h, -d, 1, 1, 0, 0, -1));

	//Back
	vertices.push_back(Mesh::MeshVertex(-w, -h, +d, 1, 1, 0, 0, 1));
	vertices.push_back(Mesh::MeshVertex(+w, -h, +d, 0, 1, 0, 0, 1));
	vertices.push_back(Mesh::MeshVertex(+w, +h, +d, 0, 0, 0, 0, 1));
	vertices.push_back(Mesh::MeshVertex(-w, +h, +d, 1, 0, 0, 0, 1));

	//Top
	vertices.push_back(Mesh::MeshVertex(-w, +h, -d, 0, 1, 0, 1, 0));
	vertices.push_back(Mesh::MeshVertex(-w, +h, +d, 0, 0, 0, 1, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, +d, 1, 0, 0, 1, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, -d, 1, 1, 0, 1, 0));

	//Bottom
	vertices.push_back(Mesh::MeshVertex(-w, -h, -d, 1, 1, 0, -1, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, -d, 0, 1, 0, -1, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, +d, 0, 0, 0, -1, 0));
	vertices.push_back(Mesh::MeshVertex(-w, -h, +d, 1, 0, 0, -1, 0));

	//Left
	vertices.push_back(Mesh::MeshVertex(-w, -h, +d, 0, 1, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, +h, +d, 0, 0, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, +h, -d, 1, 0, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, -h, -d, 1, 1, -1, 0, 0));

	//Right
	vertices.push_back(Mesh::MeshVertex(+w, -h, -d, 0, 1, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, -d, 0, 0, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, +d, 1, 0, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, +d, 1, 1, 1, 0, 0));


	indices =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	vertexBuffer = new VertexBuffer(&vertices[0], vertices.size(), sizeof(Mesh::MeshVertex));
	indexBuffer = new IndexBuffer(&indices[0], indices.size());
}

