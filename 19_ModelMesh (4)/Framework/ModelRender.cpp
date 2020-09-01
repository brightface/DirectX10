#include "Framework.h"
#include "ModelRender.h"

ModelRender::ModelRender(Shader * shader) : shader(shader)
{
	model = new Model();
	transform = new Transform(shader);
}

ModelRender::~ModelRender()
{
	SafeDelete(model);
	SafeDelete(transform);
}

void ModelRender::Update()
{
	if (bRead == true)
	{
		bRead = false;

		for (ModelMesh* mesh : model->Meshes())
			mesh->SetShader(shader);
	
		UpdateTransform();
	}
	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelRender::Render()
{
	for (ModelMesh* mesh model->Meshes()) {
		mesh->SelfTransfrom
	}
}

void ModelRender::ReadMesh(wstring file)
{
	bRead = true;
	model->ReadMesh(file);
}

void ModelRender::Pass(UINT pass)
{
	//pass�� �� �޽ÿ� �ִ°� ��ü ���ư��鼭 ���ָ� ��
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

void ModelRender::UpdateTransform()
{
	for (UINT i = 0; i < model->BoneCount(); i++) {
		ModelBone* bone = model->BoneByIndex(i);
		transforms[i] = bone->Transforms();
	}


}
