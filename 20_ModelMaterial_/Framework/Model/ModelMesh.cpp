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
	SafeDelete(transform);
	SafeDelete(perFrame);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);

	for (ModelMeshPart* part : meshParts)
		SafeDelete(part);
}
//�������۴� �޽ø� ������ �ִ°Ű�
//�޽ÿ� �޽���Ʈ�� ����
//�޽ð� ������ü
//2���� ���ؽ����۴� ����.
//�� �ȿ��� ���°��̴�.

//�޽þȿ� �������۶� �ε��� ���۸� �ٰž�
//���۰� ���� ��� �׸���.
//�޽���Ʈ�� ��ü �������� ������ �ִ°� �ƴϴ�.
//�޽ÿ� ���ý� ���� �ִ� ������ �װ��̴�.
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
	//�ּ� �ٽð������°ž�
	sBoneBuffer = shader->AsConstantBuffer("CB_Bone");
	//�ٳ� ���̴��� �ּ� ���� �������°ž�. �׸��� ���̾�.
	for (ModelMeshPart* part : meshParts)
		part->SetShader(shader);
}

//�޽ð� �ְ� �׾ȿ� �޽���Ʈ�� �ִ�.
//�޽� ��Ʈ�� ���� ������ ����? �޽ø� �����Ұ��̴�.
//�ű⼭ ��� �׸��°Ŵϱ�.
//�޽ô� ������ ������? ���� �ִ� ��ȣ�� ���带 ������ �׸���.

//���� - �� - �޽� - �޽���Ʈ
void ModelMesh::Update()
{
	boneDesc.BoneIndex = boneIndex;

	perFrame->Update();
	transform->Update();

	for (ModelMeshPart* part : meshParts)
		part->Update();
}

void ModelMesh::Render()
{
	boneBuffer->Render();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	perFrame->Render();
	transform->Render();
	//�޽ÿ� �ִ°� �� �����ϰ� �޽� ��Ʈ ���Ұž�.
	vertexBuffer->Render();
	indexBuffer->Render();

	//�ﰢ�� ������ �׷���
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�� ������ �޽���Ʈ �������� ����
	for (ModelMeshPart* part : meshParts)
		part->Render();
}
//���迭 ��ü�� �������ٰ��̴�. �� ��ü�迭 , �𵨿��� �������Ҷ�
void ModelMesh::Transforms(Matrix * transforms)
{
	memcpy(boneDesc.Transforms, transforms, sizeof(Matrix) * MAX_MODEL_TRANSFORMS);
}
//���� �׷��� ��ü����, �θ��ڽ� ������ �޽� ���� , ��ü���忡 ���� ����
void ModelMesh::SetTransform(Transform * transform)
{
	//set�� transform �� �־��ش�. ���ɷ� ���� �־��ش�.
	this->transform->Set(transform);
}

//������ ���� �а� ������ �ϴ°� ���ž�~!