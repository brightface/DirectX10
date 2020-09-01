#include "Framework.h"
#include "ModelRender.h"

ModelRender::ModelRender(Shader * shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform(shader);
}

ModelRender::~ModelRender()
{
	
	SafeDelete(model);
	for(Transform* transform : transforms)
		SafeDelete(transform);

	SafeDelete(instanceBuffer);
	SafeRelease(transformsSRV);
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
	instanceBuffer->Render();
	instanceColorBuffer->Render();

	for (ModelMesh* mesh : model->Meshes())
	{
		mesh->SetTransform(transforms.size());
		mesh->Render();
	}
}
//������ �ؽ�ó�� �ٲ���� ���̴����ٰ� �о��ٷ��� ���ݾ�
void ModelRender::ReadMesh(wstring file)
{
	model->ReadMesh(file);
}

void ModelRender::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);

}

void ModelRender::Pass(UINT pass)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

void ModelRender::UpdateBoneTransform(UINT instance, ModelBone * bone, Matrix & matrix)
{
	UpdateTransform(instance, model->BoneByIndex(boneindex), transform);
	
	Matrix desMatrix = transform->World();
	//�����θ�
	boneTransforms[instance][bone->Index()] = destMatrix * boneTrnasforms[instance][bone->Index()];

	int tempBoneIndex = bone->Index(); //3���� ����. ��ͷ� �ٲ���Ѵ�.
	for (ModelBone* child : bone->Childs())
	{
		Matrix parent = boneTransforms[instance][bone->Index()];

		Matrix invParent; //�ִϸ�� ���鶧 ������ ���������. ����ĸ�����־���.
		D3DXMatrixinverse(&invParent, NULL, &parent);
		tempBoneIndex++;
		//���ؿ��� �󸶸�ŭ �̵��߰� �׷���  ������ �󸶸�ŭ�̵��޴°�

		Matrix temp = boneTransforms[instance][tempBoneIndex] * invParent;
		boneTrnasforms[instnace][tempBoneIndex] = temp * destMatrix(parent);

		UpdateTrnasfrom(instance, child, transform); 
		
	}
}

void ModelRender::UpdateTransform(ModelBone * bone, Matrix & matrix)
{
	Matrix desMatrix = trnasform->World();

	ModelBone* bone
	if (bone != NULL)
		UpdateBones(bone, matrix);

	for (UINT i = 0; i < model->BoneCount(); i++)
	{
		ModelBone* bone = model->BoneByIndex(i);
		transforms[i] = bone->Transform();
	}

	for (ModelMesh* mesh : model->Meshes())
		mesh->Transforms(transforms);
}

void ModelRender::UpdateBones(ModelBone * bone, Matrix & matrix)
{
	Matrix temp = bone->Transform();
	bone->Transform(temp * matrix);

	for (ModelBone* child : bone->Childs())
		UpdateBones(child, matrix);
}

void ModelRender::CreateTexutre()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = MAX_MODEL_TRANSFORMS * 4;  //��Ʈ������
	desc.Height = MAX_MODEL_INSTANCE;//y�� �ν��Ͻ���
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.Usage = D3D11_USAGE_DYNAMIC; //ȸ�������� �Ҷ�� �ٽ� ������ݾ�.
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;

	Matrix bones[MAX_MODEL_TRANSFORMS];
	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{
			ModelBone* bone = model->BoneByIndex(b);

			Matrix parent;
			int parentIndex = bone->ParentIndex();

			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];

			Matrix matrix = bone->Transform();
			bones[b] = parent;
			boneTransforms[i][b] = matrix * bones[b];
		}//for(b)
	}//for(i)
	//���̴�
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = boneTransforms;
	subResource.SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
	subResource.SysMemSlicePitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix) * MAX_MODEL_INSTANCE;

	Check(D3D::GetDevice()->CreateTexture2D(&desc, &subResource, &texture));
	//���̴�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Format = desc.Format;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));

	for (ModelMesh* mesh : model->Meshes())
		mesh->TransformsSRV(srv);
}