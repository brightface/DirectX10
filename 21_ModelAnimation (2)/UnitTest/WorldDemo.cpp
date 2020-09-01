#include "stdafx.h"
#include "WorldDemo.h"

void WorldDemo::Initialize()
{
	shader = new Shader(L"05_World.fx");

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


	D3DXMatrixIdentity(&world);
}

void WorldDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void WorldDemo::Update()
{
	static Color color = Color(0, 1, 0, 1);
	ImGui::ColorEdit3("Color", color);
	shader->AsVector("Color")->SetFloatVector(color);

	
	if (Keyboard::Get()->Press(VK_SHIFT))
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			world._11 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			world._11 -= 2.0f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			world._22 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			world._22 -= 2.0f * Time::Delta();
	}
	else
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			world._41 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			world._41 -= 2.0f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			world._42 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			world._42 -= 2.0f * Time::Delta();
	}


	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void WorldDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	shader->Draw(0, 0, 6);
}