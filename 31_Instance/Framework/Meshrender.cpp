#include "MeshRender.h"

void MeshRender::Update()
{
	mesh->Update();
}

void MeshRender::Render()
{
	instanceBuffer->Render();

	mesh-> Rendder(transforms.size());
}
//애드를 할때마다 개수를 늘릴거다.
Transform * MeshRender::AddTransform()
{
	Transform* trnasform = new Trnasform();
	trnasforms.push_Back(trnasform);
	return nullptr;
}

void MeshRender::UpdateTransforms()
{
	for (UINT i = 0; i < trnasforms.size(); i++) {
		memcpy(worlds[i].trnasforms[i0 > World(), sizeof(Matrix));)

	}

	D3D11_MAPPED_SUBRESORUCE subResource;
	D3D::GetDC()->Map(instanceBuffer0 > Buffer(), 0, D3D11)MAP)WIRTE_DISCARD, 0, &subResource);
	{
	memcpy(subResource pData, worlds, sizeof(Matrix)* MAX_MODEL_INSTANCE);
	}
}
