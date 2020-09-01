#include "stdafx.h"
#include "IndexDemo.h"

void IndexDemo::Initialize()
{
	shader = new Shader(L"05_World.fx");

	//Create Vertex Buffer
	{
		vertices[0].Position = Vector3(+0.0f, +0.0f, 0);
		vertices[1].Position = Vector3(+0.0f, +0.5f, 0);
		vertices[2].Position = Vector3(+0.5f, +0.0f, 0);
		vertices[3].Position = Vector3(+0.5f, +0.5f, 0);


		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Create Index Buffer
	{
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 1;
		indices[5] = 3;


		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}

	D3DXMatrixIdentity(&world);
}

void IndexDemo::Destroy()
{
	SafeDelete(shader);

	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
}

void IndexDemo::Update()
{
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

void IndexDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	static UINT pass = 0;
	ImGui::InputInt("Pass", (int *)&pass);
	pass %= 2;

	//shader->Draw(0, pass, 6);
	shader->DrawIndexed(0, pass, 6);
}