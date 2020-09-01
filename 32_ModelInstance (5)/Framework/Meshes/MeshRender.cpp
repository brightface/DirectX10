#include "Framework.h"
#include "MeshRender.h"

//매트릭스 초기화 하고 인스턴스 받아주는것.
MeshRender::MeshRender(Shader * shader, Mesh * mesh) : mesh(mesh)
{
	Pass(0);
	mesh->SetShader(shader);

	for (UINT i = 0; i < MAX_MESH_INSTANCE; i++)
		D3DXMatrixIdentity(&worlds[i]);
	//최대값을 잡아놓고 쓸만큼만 쓰겠다.
	instanceBuffer = new VertexBuffer(worlds, MAX_MESH_INSTANCE, sizeof(Matrix), 1, true);
}

MeshRender::~MeshRender()
{
	for (Transform* transform : transforms)
		SafeDelete(transform);

	SafeDelete(instanceBuffer);
	SafeDelete(mesh);
}

void MeshRender::Update()
{	
	mesh->Update();//메시가 가진 업데이트 수행
}

void MeshRender::Render()
{
	instanceBuffer->Render();
	//밀어줘야지
	mesh->Render(transforms.size());
	//몇개를 콜할지 - 그게 트랜스폼의 개수다. 위치에다가 그릴개수
}

Transform * MeshRender::AddTransform()
{
	//에드를 할때마다 그릴개수를 늘릴거거든
	//트랜스 폼을 그릴거거든
	Transform* transform = new Transform();
	transforms.push_back(transform);

	return transform;
}

void MeshRender::UpdateTransforms()
{
	//트랜스폼을 가진 트랜스폼을 매트릭스 쪽으로 복사해 올거야.
	for (UINT i = 0; i < transforms.size(); i++)
		memcpy(worlds[i], transforms[i]->World(), sizeof(Matrix));


	//인스턴스 버퍼를 갱신할거지? write용으로 d3d11_map_Write_Discard, lock 하면 그다음 unlock 부터 해라.
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		//그냥 나는 다 할게
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MAX_MESH_INSTANCE);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
	//큐브스카이같은것.
}
