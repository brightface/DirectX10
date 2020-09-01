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
	//메시는 이름으로 찾을 이유가 없다.

	UINT MeshCount() { return meshes.size(); }
	vector<ModelMesh *>& Meshes() { return meshes; }
	ModelMesh* MeshByIndex(UINT index) { return meshes[index]; }
	ModelMesh* MeshByName(wstring name);

private:
	void ReadMesh(wstring file);//.메시 파일의 경로를 줄거야
private:
	void BindBone();
	void BindMesh();
	void ReadMesh(wstring file);//.메시 파일의 경로를 줄거야
private:
	ModelBone* root;
	vector<ModelBone *> bones; //본들 불러서 넣을거고
	vector<ModelMesh *> meshes; 
	

};