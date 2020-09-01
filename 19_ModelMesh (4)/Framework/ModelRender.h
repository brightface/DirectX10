#pragma once
class ModelRender
{
public:
	ModelRender(Shader* shader);
	~ModelRender();

	void Update();
	void Render();

public:
	void ReadMesh(wstring file);// 애를 통해 메시를 읽음

	Transform* GetTransform() {	return transform;}
	Model* GetModel() { return model; }
	//모델 외부에서 읽어올수 있게
	void Pass(UINT pass);
	//패스설정

	void UpdateTransform();
private: 
	bool bRead = false;
	Shader* shader;
	Model* model;

	//모델 전체에 대한 월드
	Transform* transform;

	Matrix treansforms[MAX_MODEL_TRANSFORMS];
};