#include "stdafx.h"
#include "TsTriangleDemo.h"

void TsTriangleDemo::Initialize()
{
	shader = new Shader(L"54_Triangle.fxo");

	Vertex vertices[3];
	vertices[0].Position = Vector3(-0.9f, -0.9f, 0.0f);
	vertices[1].Position = Vector3(+0.0f, +0.9f, 0.0f);
	vertices[2].Position = Vector3(+0.9f, -0.9f, 0.0f);

	vertexBuffer = new VertexBuffer(vertices, 3, sizeof(Vertex));
}

void TsTriangleDemo::Update()
{
	static UINT Edge[3]{ 1, 1, 1 };
	static UINT Inside = 1;

	ImGui::InputInt("Edge - 0", (int *)&Edge[0]);
	ImGui::InputInt("Edge - 1", (int *)&Edge[1]);
	ImGui::InputInt("Edge - 2", (int *)&Edge[2]);
	ImGui::InputInt("Inside", (int *)&Inside);

	shader->AsScalar("Edge")->SetIntArray((int *)Edge, 0, 3);
	shader->AsScalar("Inside")->SetInt(Inside);
}

void TsTriangleDemo::Render()
{
	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	shader->Draw(0, 0, 3);
}

