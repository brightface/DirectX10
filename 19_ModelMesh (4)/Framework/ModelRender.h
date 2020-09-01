#pragma once
class ModelRender
{
public:
	ModelRender(Shader* shader);
	~ModelRender();

	void Update();
	void Render();

public:
	void ReadMesh(wstring file);// �ָ� ���� �޽ø� ����

	Transform* GetTransform() {	return transform;}
	Model* GetModel() { return model; }
	//�� �ܺο��� �о�ü� �ְ�
	void Pass(UINT pass);
	//�н�����

	void UpdateTransform();
private: 
	bool bRead = false;
	Shader* shader;
	Model* model;

	//�� ��ü�� ���� ����
	Transform* transform;

	Matrix treansforms[MAX_MODEL_TRANSFORMS];
};