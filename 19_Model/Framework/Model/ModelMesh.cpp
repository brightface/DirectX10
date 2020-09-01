#include "Framework.h"
#include "ModelMesh.h"


ModelBone::ModelBone()
{

}

ModelBone::~ModelBone()
{

}

///////////////////////////////////////////////////////////////////////////////

//�� �޽��� ��Ʈ�� �����ϴ°�
ModelMesh::ModelMesh()
{
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

ModelMesh::~ModelMesh()
{
	SafeDelete(transform);
	SafeDelete(perFrame);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);

	for (ModelMeshPart* part : meshParts)
		SafeDelete(part);
	//�����ϴ°ͱ����� �����Ұž�
}

void ModelMesh::Binding(Model * model)
{
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(Model::ModelVertex));
	indexBuffer = new IndexBuffer(indices, indexCount);

	for (ModelMeshPart* part : meshParts)
		part->Binding(model);
}

void ModelMesh::Pass(UINT val)
{
	for (ModelMeshPart* part : meshParts)
		part->Pass(val);
}

void ModelMesh::SetShader(Shader * shader)
{
	this->shader = shader;

	SafeDelete(transform);
	transform = new Transform(shader);

	SafeDelete(perFrame);
	perFrame = new PerFrame(shader);

	sBoneBuffer = shader->AsConstantBuffer("CB_Bone");
	for (ModelMeshPart* part : meshParts)
		part->SetShader(shader);
	//���̴��� �ٲ�� �����۰� �ٲ�� �ּҰ� �׷��� �� �ٲ��..

}

void ModelMesh::Update()
{
	boneDesc.BoneIndex = boneIndex;

	perFrame->Update();
	transform->Update();

	for (ModelMeshPart* part : MeshParts)
		part->Update();
	//�迭 ��ä�� �Ѱ��ش�.
	boneDesc.Transforms
}

void ModelMesh::Render()
{
	boneBuffer->Render();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	perFrame->Render();

	vertexBuffer->Render();
	indexBuffer->Render();
	//�ڱ�� �����ϰ� ���Ұž�
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (ModelMeshPart* part : meshParts)
		part->Render(instanceCount);
}

void ModelMesh::Trnasforms(Matrix* transforms)
{
	memcpy(boneDesc.Transforms, transforms, sizeof(matrix)*);
}

void ModelMesh::setTransfrom(Transfrom * transform)
{
	this->transform->Set(Transfrom);
}