#pragma once

class ModelRender
{
public:
	ModelRender(Shader* shader);
	~ModelRender();

	void Update();
	void Render();

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);

	//�̰� ������?
	Transform* GetTransform() { return transform; }
	Model* GetModel() { return model; }

	void Pass(UINT pass);

	void UpdateBoneTransform(UINT instance, ModelBone* boneindex, Matrix& matrix = Matrix());
	void UpdateBoneTransform(UINT instance, ModelBone* bone = NULL, Matrix& matrix = Matrix());

private:
	void UpdateBones(UINT instance, ModelBone* bone, Matrix& matrix);

private:
	void UpdateBoneTransform();
	//�������� ������Ʈ�Գ� ���̾�? gpu�� ��������� �����ָ� ����.
private:
	void CreateTexture();
	
private: 
	

private:
	bool bRead = false;

	Shader* shader;
	Model* model;

	Transform* transform;
	vector<Transform *> transforms;
	Matrix worlds[MAX_MODEL_TRANSFORMS];
	VertexBuffer* instanceBuffer;

	Matrix boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_TRANSFORMS];

	//animation�� texture�� �Ѱ��.

	ID3D11Texture2D* texture = NULL;
	ID3D11ShaderResourceView* srv;
};