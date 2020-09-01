#pragma once
class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(wstring file);

public:
	void ExportMesh(wstring savePath);

private:
	wstring file;
	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int index);
	void WriteMeshData(wstring savePath);

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<struct asBone *> bones;
	vector<struct asMesh *> meshes;
};