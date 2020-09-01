#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard(wstring textureFile)
	:Renderer(L"36_Billboard.fxo")
{
	vertexCount = MAX_BILLBOARD_COUNT * 4;
	vertices = new VertexBillboard[vertexCount];

	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexBillboard), 0, true);


	indexCount = MAX_BILLBOARD_COUNT * 6;
	indices = new UINT[indexCount];

	for (UINT i = 0; i < MAX_BILLBOARD_COUNT; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 1;
		indices[i * 6 + 5] = i * 4 + 3;
	}
};

Billboard::~Billboard()
{
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(texture);
}

void Billboard::Update()
{
	Super::Update();
}

void Billboard::Render()
{
	prevCount = drawCount;

	D3D11_MAPPED_SUBRESORUCE subResource;
	D3D::GetDC()->Map(vertexBuffer-> Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		
	}
}

void Billboard::Add(Vector3 & position, Vector2 & scale)
{
	vertices[drawCount * 4 + 0].Position = position;
	vertices[drawCount * 4 + 1].Position = position;
	vertices[drawCount * 4 + 2].Position = position;
	vertices[drawCount * 4 + 3].Position = position;
	
	vertices[drawCount * 4 + 0].Uv = position;
	vertices[drawCount * 4 + 1].Uv = position;
	vertices[drawCount * 4 + 2].Uv = position;
	vertices[drawCount * 4 + 3].Uv = position;
	
	vertices[drawCount * 4 + 0].Scale = scale;
	vertices[drawCount * 4 + 1].Scale = scale;
	vertices[drawCount * 4 + 2].Scale = scale;
	vertices[drawCount * 4 + 3].Scale = scale;
}

	