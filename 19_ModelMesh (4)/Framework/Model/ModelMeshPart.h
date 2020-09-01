#pragma once

class ModelMeshPart
{
public:
	friend class Model;
	friend class ModelMesh;

private:
	//거기에서만 접근할것
	ModelMeshPart();
	~ModelMeshPart();

	void Update();
	void Render();

	void Binding(Model* model);
	void SetShader(Shader* shader);
	void Pass(UINT val) { pass = val; }

	

private:
	Shader* shader;
	UINT pass = 0;

	//Material* material = NULL;
	wstring materialName;

	UINT startVertex;
	UINT vertexCount;

	UINT startIndex;
	UINT indexCount;

};