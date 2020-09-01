#include "stdafx.h"
#include "TriangleListDemo.h"

void TriangleListDemo::Initialize()
{
	shader = new Shader(L"03_Triangle.fx");

	//Vertices - 0
	{
		vertices[0].Position = Vector3(-0.5f, +0.0f, 0.0f);
		vertices[1].Position = Vector3(+0.0f, +0.5f, 0.0f);
		vertices[2].Position = Vector3(+0.5f, +0.0f, 0.0f);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}
	
	//Vertices - 1
	{
		vertices2[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices2[1].Position = Vector3(+0.0f, -0.0f, 0.0f);
		vertices2[2].Position = Vector3(+0.5f, -0.5f, 0.0f);

		vertices2[0].Color = Color(1, 0, 0, 0);
		vertices2[1].Color = Color(0, 1, 0, 0);
		vertices2[2].Color = Color(0, 0, 1, 0);


		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(VertexColor) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices2;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer2));
	}
}

void TriangleListDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
	SafeRelease(vertexBuffer2);
}

void TriangleListDemo::Update()
{
	static Color color = Color(0, 1, 0, 1);
	ImGui::ColorEdit3("Color", color);

	shader->AsVector("Color")->SetFloatVector(color);

	static float red = 0.0f;
	ImGui::SliderFloat("Red", &red, 0, 1);
	shader->AsScalar("Red")->SetFloat(red);
}

void TriangleListDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	shader->Draw(0, 0, 3);


	stride = sizeof(VertexColor);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride, &offset);
	shader->Draw(0, 1, 3);
}