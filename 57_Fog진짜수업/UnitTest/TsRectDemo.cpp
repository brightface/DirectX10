#include "stdafx.h"
#include "TsRectDemo.h"

void TsRectDemo::Initialize()
{
	shader = new Shader(L"54_Rect.fxo");

	Vertex vertices[4];
	vertices[0].Position = Vector3(-0.9f, -0.9f, 0);
	vertices[1].Position = Vector3(-0.9f, +0.9f, 0);
	vertices[2].Position = Vector3(+0.9f, -0.9f, 0);
	vertices[3].Position = Vector3(+0.9f, +0.9f, 0);

	vertexBuffer = new VertexBuffer(vertices, 4, sizeof(Vertex));
}

void TsRectDemo::Update()
{
	static UINT Edge[4]{ 1, 1, 1, 1 };
	static UINT Inside[2] = { 1 , 1 };

	ImGui::InputInt("Edge - 0", (int *)&Edge[0]);
	ImGui::InputInt("Edge - 1", (int *)&Edge[1]);
	ImGui::InputInt("Edge - 2", (int *)&Edge[2]);
	ImGui::InputInt("Edge - 3", (int *)&Edge[3]);
	ImGui::InputInt("Inside - 0", (int *)&Inside[0]);
	ImGui::InputInt("Inside - 1", (int *)&Inside[1]);

	shader->AsScalar("Edge")->SetIntArray((int *)Edge, 0, 4);
	shader->AsScalar("Inside")->SetIntArray((int *)Inside, 0, 2);
}

void TsRectDemo::Render()
{
	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	shader->Draw(0, 0, 4);
}

