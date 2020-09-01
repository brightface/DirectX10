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
//정점버퍼는 메시만 가지고 있는거고
//메시와 메시파트의 관계
//메시가 파일전체
//2개의 버텍스버퍼는 같다.
//그 안에서 끊는것이다.

//메시안에 정점버퍼랑 인덱스 버퍼를 줄거야
//시작과 개수 끊어서 그린다.
//메시파트는 전체 정점버퍼 가지고 있는것 아니다.
//메시에 버택스 버퍼 있는 이유가 그것이다.
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
	//주소 다시가져오는거야
	sBoneBuffer = shader->AsConstantBuffer("CB_Bone");
	//바낀 쉐이더로 주소 새로 가져오는거야. 그리고 콜이야.
	for (ModelMeshPart* part : meshParts)
		part->SetShader(shader);
}

//메시가 있고 그안에 메시파트가 있다.
//메시 파트는 월드 누구를 참조? 메시를 참조할것이다.
//거기서 끊어만 그리는거니까.
//메시는 누구를 참조해? 본에 있는 번호의 월드를 가져다 그린다.

//월드 - 본 - 메시 - 메시파트
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
	//메시에 있는거 다 세팅하고 메시 파트 콜할거야.
	vertexBuffer->Render();
	indexBuffer->Render();

	//삼각형 단위로 그려라
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//그 다음에 메시파트 랜더링에 들어가자
	for (ModelMeshPart* part : meshParts)
		part->Render();
}
//본배열 전체를 복사해줄것이다. 본 전체배열 , 모델에서 랜더링할때
void ModelMesh::Transforms(Matrix * transforms)
{
	memcpy(boneDesc.Transforms, transforms, sizeof(Matrix) * MAX_MODEL_TRANSFORMS);
}
//모델이 그려질 전체월드, 부모자식 관계의 메시 말고 , 전체월드에 대한 월드
void ModelMesh::SetTransform(Transform * transform)
{
	//set이 transform 을 넣어준다. 지걸로 갖다 넣어준다.
	this->transform->Set(transform);
}

//다음은 파일 읽고 랜더링 하는것 들어갈거야~!