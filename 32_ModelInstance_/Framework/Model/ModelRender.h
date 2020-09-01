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

	//이거 빠지지?
	Transform* GetTransform() { return transform; }
	Model* GetModel() { return model; }

	void Pass(UINT pass);

	void UpdateBoneTransform(UINT instance, ModelBone* boneindex, Matrix& matrix = Matrix());
	void UpdateBoneTransform(UINT instance, ModelBone* bone = NULL, Matrix& matrix = Matrix());

private:
	void UpdateBones(UINT instance, ModelBone* bone, Matrix& matrix);

private:
	void UpdateBoneTransform();
	//본정보를 업데이트함녀 끝이야? gpu로 보내줘야지 맵해주면 되지.
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

	//animation때 texture로 넘겼다.

	ID3D11Texture2D* texture = NULL;
	ID3D11ShaderResourceView* srv;
};