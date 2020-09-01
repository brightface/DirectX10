#include "stdafx.h"
#include "World2Demo.h"

void World2Demo::Initialize()
{
	shader = new Shader(L"05_World.fx");

	//Vertices - 0
	{
		vertices[0].Position = Vector3(+0.0f, +0.0f, 0);
		vertices[1].Position = Vector3(+0.0f, +0.5f, 0);
		vertices[2].Position = Vector3(+0.5f, +0.0f, 0);
		vertices[3].Position = Vector3(+0.5f, +0.0f, 0);
		vertices[4].Position = Vector3(+0.0f, +0.5f, 0);
		vertices[5].Position = Vector3(+0.5f, +0.5f, 0);


		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	
	for (UINT i = 0; i < 10; i++)
	{
		Vector3 scale = Math::RandomVec3(0.5f, 2.0f);
		Vector3 position = Math::RandomVec3(-3.0f, 3.0f);
		position.z = 0.0f;

		Matrix S, T;
		D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
		D3DXMatrixTranslation(&T, position.x, position.y, position.z);

		world[i] = S * T;
		color[i] = Math::RandomColor4();
	}
}

void World2Demo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void World2Demo::Update()
{
	static UINT index = 0;
	ImGui::InputInt("Index", (int *)&index);

	if (Keyboard::Get()->Press(VK_SHIFT))
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			world[index]._11 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			world[index]._11 -= 2.0f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			world[index]._22 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			world[index]._22 -= 2.0f * Time::Delta();
	}
	else
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			world[index]._41 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			world[index]._41 -= 2.0f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			world[index]._42 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			world[index]._42 -= 2.0f * Time::Delta();
	}
	

	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void World2Demo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	static UINT pass = 0;
	ImGui::InputInt("Pass", (int *)&pass);
	pass %= 2;

	for (UINT i = 0; i < 10; i++)
	{
		shader->AsVector("Color")->SetFloatVector(color[i]);
		shader->AsMatrix("World")->SetMatrix(world[i]);
		shader->Draw(0, pass, 6);
	}
}