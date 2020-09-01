#include "Framework.h"
#include "ModelMeshPart.h"

ModelMeshPart::ModelMeshPart()
{

}

ModelMeshPart::~ModelMeshPart()
{
	SafeDelete(material);
}

void ModelMeshPart::Update()
{

}

//void ModelMeshPart::Render()
//{
//	material->Render();
//
//	shader->DrawIndexed(0, pass, indexCount, startIndex);
//}

void ModelMeshPart::Render(UINT drawCount)
{
	material->Render();

	shader->DrawIndexedInstanced(0, pass, indexCount, drawCount, startIndex);
}

void ModelMeshPart::Binding(Model * model)
{
	Material* srcMaterial = model->MaterialByName(materialName);

	material = new Material();
	material->Ambient(srcMaterial->Ambient());
	material->Diffuse(srcMaterial->Diffuse());
	material->Specular(srcMaterial->Specular());
	material->Emissive(srcMaterial->Emissive());

	if (srcMaterial->DiffuseMap() != NULL)
		material->DiffuseMap(srcMaterial->DiffuseMap()->GetFile());

	if (srcMaterial->SpecularMap() != NULL)
		material->SpecularMap(srcMaterial->SpecularMap()->GetFile());

	if (srcMaterial->NormalMap() != NULL)
		material->NormalMap(srcMaterial->NormalMap()->GetFile());
}

void ModelMeshPart::SetShader(Shader * shader)
{
	this->shader = shader;

	material->SetShader(shader);
}
