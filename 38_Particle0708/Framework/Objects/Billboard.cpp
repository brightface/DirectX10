#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard(wstring textureFile)
	: Renderer(L"39_Billboard.fxo")
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//�������� �����ž�

	
}

Billboard::~Billboard()
{

	SafeDelete(texture);
}

void Billboard::Update()
{
	Super::Update();
}

void Billboard::Render()
{
	if (vertexCount != vertices.size())
	{
		vertexCount = vertices.size()
			SafeDelete(vertexBuffer);
		vertexBuffer = newVertexBuffer(&vertices[0], verteices.size() sizeof(VertexBillboard))
		
	}
	//�ؽ�ó ���������� ���� �̴ϼȿ�
	Super::Render();

	sDiffuseMap->SetResource(texture->SRV());
	
	shader->DrawIndexed(0, Pass(), vertices.size*();//�������� �ѱ���̴�. �������� �ƴ϶�
}

void Billboard::Add(Vector3 & position, Vector2 & scale)
{
	VertexBillboard vertex =
	{
		posiition, scale
	}
	vertices.push_back(vertex);
}
