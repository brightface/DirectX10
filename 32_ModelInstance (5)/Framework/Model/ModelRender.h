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

	Model* GetModel() { return model; }

	Transform* AddTransform();
	Transform* GetTransform(UINT index) { return transforms[index]; }
	void UpdateTransforms();

	//������Ʈ ����- �̰� ������.(ȸ���Ҷ�)

	void Pass(UINT pass);

	//�̰� �𵨺����� �޴°� �ƴ϶� �� �ε����� �ް� ��Ʈ������ �޴°� �ƴ϶� Ʈ���� ������ �ވf��.
	//void UpdateBoneTransform(UINT instance, ModelBone* bone=NULL, Matrix& matrix = Matrix()); - �ؽ�ó�� ���鶧 �̹� �θ��ڽİ��� ��������ݾ� �׷��� �̷��� Ʈ���������� �ٲ�ٰ�.
	void UpdateBoneTransform(UINT instance, UINT boneIndex, Transform* transform); //���� ���ϼ��� ����. ���� ��ȣ�ι޾����� 
	//����Ҷ�� �����.
	void UpdateBoneTransform(UINT instance, ModelBone* bone, Transform* transform);

private:
	void UpdateBoneTransform();//�ƹ��͵� ���¾־�. �ָ������?
	//�ų��� �������� ������Ʈ ���ݾ�. gpu�� ��������� �ؽ�ó ������ ��������Ұ� �ƴϾ�
	//��ü �����ŭ �����ָ� ���ݾ�. �׷�����

private:
	void CreateTexture();

private:
	Shader* shader;
	Model* model;
	//�޽� ������ �ڽ� �׷���?
	//�� ������ �޽� ������ �Ȱ��� ������. Ʈ������ ���� ����
	vector<Transform *> transforms;
	//�̷��� �ν��Ͻ� �ϳ��� ��� ���� ���ٰ� ���ݾ�. �׷��� ���� 2���� �迭�� ����ž� ���� �����Ҽ� �ְ�

	Matrix worlds[MAX_MODEL_INSTANCE];
	VertexBuffer* instanceBuffer;
	//���� �� �ѹ��� �о ����ؾߵǰŵ�
	//��� ���� �ٰ��� ��İ����°� �ƴ���. ȸ���Ѵٸ�
	//��� ���� �ٰ��� Ʈ������ �����°� �ƴ���/������ �ν���Ʈ �� ���� �ٲ��ݾ�
	//3���� �ϳ� ���� 1���� �迭 - 4�����迭

	//y�� �ν��Ͻ��� ����ž�. x�� bone
	//5����ũ�� ���� ���ư��� ����ڴ�. ����

	//�𵨷����� �־��ش�. �� �ִϸ����Ϳ� �ִ���
	Color colors[MAX_MODEL_INSTANCE];
	VertexBuffer* instanceColorBuffer;

	//�׷��� �̷��� �� Ʈ���������� 2�����迭�� �ٲ��ٰž�. ��Ʈ���� ä�� ���̴��� �ѱ�� ������? 
	//�ѹ��� �ѱ�� �ִ°� 4996�� // 
	Matrix boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_TRANSFORMS];
	//���� ����

	//�� �ѱ涧 �ؽ�ó�� �Ѱ�. ���� �޸𸮰� ����ϸ� �ȴٰ�
	ID3D11Texture2D* texture = NULL;
	ID3D11ShaderResourceView* transformsSRV;
	//���� �ö� GetTransform ������.
};