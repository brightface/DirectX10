#pragma once

#define MAX_MODEL_TRANSFORMS 250
class ModelBone;
class ModelMesh;
class Model
{
public:
	friend class ModelRender;

	typedef VertexTextureNormalTangentBlend ModelVertex;

private:
	Model();
	~Model();
	

public:
	UINT BoneCount() { return bones.size(); }
	vector<ModelBone *>& Bones() { return bones; }
	ModelBone* BoneByIndex(UINT index) { return bones[index]; }
	//�޽ô� �̸����� ã�� ������ ����.

	UINT MeshCount() { return meshes.size(); }
	vector<ModelMesh *>& Meshes() { return meshes; }
	ModelMesh* MeshByIndex(UINT index) { return meshes[index]; }
	ModelMesh* MeshByName(wstring name);

private:
	void ReadMesh(wstring file);//.�޽� ������ ��θ� �ٰž�
private:
	void BindBone();
	void BindMesh();
	void ReadMesh(wstring file);//.�޽� ������ ��θ� �ٰž�
private:
	ModelBone* root;
	vector<ModelBone *> bones; //���� �ҷ��� �����Ű�
	vector<ModelMesh *> meshes; 
	

};