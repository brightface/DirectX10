#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard(wstring textureFile)
	: Renderer(L"39_Billboard.fxo")
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//점단위로 보낼거야

	
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
	//텍스처 만들어줘야지 위에 이니셜에
	Super::Render();

	sDiffuseMap->SetResource(texture->SRV());
	
	shader->DrawIndexed(0, Pass(), vertices.size*();//점단위로 넘길것이다. 점개수가 아니라
}

void Billboard::Add(Vector3 & position, Vector2 & scale)
{
	VertexBillboard vertex =
	{
		posiition, scale
	}
	vertices.push_back(vertex);
}
