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
	ModelBone* BoneByName(wstring name);

	UINT MeshCount() { return meshes.size(); }
	vector<ModelMesh *>& Meshes() { return meshes; }
	ModelMesh* MeshByIndex(UINT index) { return meshes[index]; }

	UINT MaterialCount() { return materials.size(); }
	vector<Material *>& Materials() { return materials; }
	Material* MaterialByIndex(UINT index) { return materials[index]; }
	Material* MaterialByName(wstring name);

private:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);

private:
	void BindBone();
	void BindMesh();

private:
	ModelBone* root;

	vector<ModelBone *> bones;
	vector<ModelMesh *> meshes;
	vector<Material *> materials;
};