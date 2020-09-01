#include "stdafx.h"
#include "VertexMoveDemo.h"

void VertexMoveDemo::Initialize()
{
	shader = new Shader(L"04_Rect.fx");

	vertices[0].Position = Vector3(+0.0f, +0.0f, 0.0f);
	vertices[1].Position = Vector3(+0.0f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, +0.0f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, +0.0f, 0.0f);
	vertices[4].Position = Vector3(+0.0f, +0.5f, 0.0f);
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
}

void VertexMoveDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void VertexMoveDemo::Update()
{
	static Color color = Color(0, 1, 0, 1);
	ImGui::ColorEdit3("Color", color);
	shader->AsVector("Color")->SetFloatVector(color);

	
	if (Keyboard::Get()->Press(VK_RIGHT))
		vertices[0].Position.x += 2.0f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LEFT))
		vertices[0].Position.x -= 2.0f * Time::Delta();

	static float y = 0.0f;
	ImGui::InputFloat("Y", &y, 0.01f);
	vertices[0].Position.y = y;

	D3D::GetDC()->UpdateSubresource(vertexBuffer, 0, NULL, vertices, sizeof(Vertex) * 6, 0);
}

void VertexMoveDemo::Render()
{
	static bool bOpen = true;

	if (Keyboard::Get()->Down(VK_SPACE))
		bOpen = !bOpen;

	if (bOpen == true)
		ImGui::ShowDemoWindow(&bOpen);

	static string text = "";
	if (Keyboard::Get()->Down(VK_RIGHT))
		text += "R, ";
	else if (Keyboard::Get()->Down(VK_LEFT))
		text += "L, ";

	Gui::Get()->RenderText(0, 0, 1, 0, 0, text);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	static UINT pass = 0;
	ImGui::InputInt("Pass", (int *)&pass);
	pass %= 2;

	shader->Draw(0, pass, 6);
}