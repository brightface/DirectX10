#pragma once

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500

class ModelBone;
class ModelMesh;
class ModelClip;
//전방선언 잡아주기
class Model
{
public:
	friend class ModelRender;
	friend class ModelAnimator;

public:
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

	UINT ClipCount() { return clips.size(); }
	vector<ModelClip *>& Clips() { return clips; }
	ModelClip* ClipByIndex(UINT index) { return clips[index]; }
	ModelClip* ClipByName(wstring name);

private:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	void ReadClip(wstring file); //클립 부를애

private:
	void BindBone();
	void BindMesh();

private:
	ModelBone* root;

	vector<ModelBone *> bones;
	vector<ModelMesh *> meshes;
	vector<Material *> materials;
	vector<ModelClip *> clips;
};