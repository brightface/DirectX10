#include "Framework.h"
#include "ModelMesh.h"

ModelBone::ModelBone()
{

}

ModelBone::~ModelBone()
{

}

///////////////////////////////////////////////////////////////////////////////

ModelMesh::ModelMesh()
{
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

ModelMesh::~ModelMesh()
{
	//SafeDelete(transform);
	SafeDelete(perFrame);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);

	for (ModelMeshPart* part : meshParts)
		SafeDelete(part);
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

	//이제 값 빠지지 
	//SafeDelete(transform);
	//transform = new Transform(shader);

	SafeDelete(perFrame);
	perFrame = new PerFrame(shader);

	//이거 해줘야한다 이제
	sBoneBuffer = shader->AsConstantBuffer("CB_Bones");
	//본버퍼 밑에 트랜스폼에 다 넘길수 있도록 해야할거 아니냐
	sTransformsSRV = shader->AsSRV("TransformsMap");

	for (ModelMeshPart* part : meshParts)
		part->SetShader(shader);
}

void ModelMesh::Update()
{
	boneDesc.BoneIndex = boneIndex;

	perFrame->Update();
	//transform->Update(); //필요 없어지지

	for (ModelMeshPart* part : meshParts)
		part->Update();
}

//void ModelMesh::Render()
//{
//	boneBuffer->Render();
//	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());
//
//	perFrame->Render();
//	transform->Render();
//
//	vertexBuffer->Render();
//	indexBuffer->Render();
//
//	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	for (ModelMeshPart* part : meshParts)
//		part->Render();
//}

void ModelMesh::Render(UINT drawCount)
{
	boneBuffer->Render();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	perFrame->Render();

	vertexBuffer->Render();
	indexBuffer->Render();
	sTransformsSRV->SetResource(transformsSRV);

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//랜더에도 드로우 카운터가 있어야한다.
	for (ModelMeshPart* part : meshParts)
		part->Render(drawCount);
}

void ModelMesh::TransformsSRV(ID3D11ShaderResourceView * srv)
{
	transformsSRV = srv;
}

//트랜스폼도 필요없지
//void ModelMesh::Transforms(Matrix * transforms)
//{
//	memcpy(boneDesc.Transforms, transforms, sizeof(Matrix) * MAX_MODEL_TRANSFORMS);
//}
//
//void ModelMesh::SetTransform(Transform * transform)
//{
//	this->transform->Set(transform);
//}