#include "Framework.h"
#include "ModelMesh.h"


ModelBone::ModelBone()
{

}

ModelBone::~ModelBone()
{

}

///////////////////////////////////////////////////////////////////////////////

//모델 메시의 파트를 관리하는것
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
	//관리하는것까지다 삭제할거야
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
	//쉐이더가 바뀌면 씨버퍼가 바뀌고 주소가 그래서 다 바뀐다..

}

void ModelMesh::Update()
{
	boneDesc.BoneIndex = boneIndex;

	perFrame->Update();
	transform->Update();

	for (ModelMeshPart* part : MeshParts)
		part->Update();
	//배열 통채로 넘겨준다.
	boneDesc.Transforms
}

void ModelMesh::Render()
{
	boneBuffer->Render();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	perFrame->Render();

	vertexBuffer->Render();
	indexBuffer->Render();
	//자기거 세팅하고 콜할거야
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