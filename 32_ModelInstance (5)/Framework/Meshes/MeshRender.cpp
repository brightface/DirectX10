#include "Framework.h"
#include "MeshRender.h"

//��Ʈ���� �ʱ�ȭ �ϰ� �ν��Ͻ� �޾��ִ°�.
MeshRender::MeshRender(Shader * shader, Mesh * mesh) : mesh(mesh)
{
	Pass(0);
	mesh->SetShader(shader);

	for (UINT i = 0; i < MAX_MESH_INSTANCE; i++)
		D3DXMatrixIdentity(&worlds[i]);
	//�ִ밪�� ��Ƴ��� ����ŭ�� ���ڴ�.
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
	mesh->Update();//�޽ð� ���� ������Ʈ ����
}

void MeshRender::Render()
{
	instanceBuffer->Render();
	//�о������
	mesh->Render(transforms.size());
	//��� ������ - �װ� Ʈ�������� ������. ��ġ���ٰ� �׸�����
}

Transform * MeshRender::AddTransform()
{
	//���带 �Ҷ����� �׸������� �ø��Űŵ�
	//Ʈ���� ���� �׸��Űŵ�
	Transform* transform = new Transform();
	transforms.push_back(transform);

	return transform;
}

void MeshRender::UpdateTransforms()
{
	//Ʈ�������� ���� Ʈ�������� ��Ʈ���� ������ ������ �ðž�.
	for (UINT i = 0; i < transforms.size(); i++)
		memcpy(worlds[i], transforms[i]->World(), sizeof(Matrix));


	//�ν��Ͻ� ���۸� �����Ұ���? write������ d3d11_map_Write_Discard, lock �ϸ� �״��� unlock ���� �ض�.
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		//�׳� ���� �� �Ұ�
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MAX_MESH_INSTANCE);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
	//ť�꽺ī�̰�����.
}
