#pragma once

class ModelBone
{
public:
	friend class Model;
	//�𵨿����� �����Ҽ� �ְ� �س��Ҵ�.
private:
	ModelBone();
	~ModelBone();

public:
	int Index() { return index; }

	int ParentIndex() { return parentIndex; }
	ModelBone* Parent() { return parent; }

	wstring Name() { return name; }

	Matrix& Transform() { return transform; }
	void Transform(Matrix& matrix) { transform = matrix; }

	vector<ModelBone *>& Childs() { return childs; }

private:
	int index;
	wstring name;

	int parentIndex;
	ModelBone* parent;

	Matrix transform;
	vector<ModelBone *> childs;
};

///////////////////////////////////////////////////////////////////////////////

class ModelMesh
{
public:
	friend class Model;

private:
	ModelMesh();
	~ModelMesh();

	void Binding(Model* model);

public:
	//�н���ȣ �ٲ��ִ°�. �ڱ�� �����ϰ� �� �ڽ����� �Ѱ��� �� �޽���Ʈ��
	void Pass(UINT val);
	//�̰͵� ���� �������� ���̴��� �ٳ��� �ּҰ� �� �ٲ��. �װͶ����� �ִ°��̴�.
	void SetShader(Shader* shader);

	void Update();
	void Render();
	
	int BoneIndex() { return boneIndex; }
	class ModelBone* Bone() { return bone; }

	void Transforms(Matrix* transforms);
	void SetTransform(Transform* transform);

private:
	
	struct BoneDesc
	{//�ִ밹�� �����ذž�, ���̴��� �������� �������� ������ ����, ������ �����̾�
		Matrix Transforms[MAX_MODEL_TRANSFORMS];
		
		//���� �޽ð� ������ ����ȣ ,���̴����� �� �����ؼ� �� ������� ������.
		UINT BoneIndex;
		//�̷����� �е� �ʿ���, 16����Ʈ�� ������ �Ѵ�.
		float Padding[3];
	} boneDesc;

private:
	Shader* shader;

	//�������� ����
	Transform* transform = NULL;
	//�������Ӹ��� ����Ǵ°� �о��ִ� Ŭ����
	PerFrame* perFrame = NULL;
	
	//������ ����ȣ (�޽ð� �����ؼ� �׷����ݾ�)
	int boneIndex;
	//������ ��ü
	ModelBone* bone;

	VertexBuffer* vertexBuffer;
	UINT vertexCount;
	Model::ModelVertex* vertices;

	IndexBuffer* indexBuffer;
	UINT indexCount;
	UINT* indices;
	//�� �޽ô� �𵨸޽���Ʈ�� �����ϴ� ������ �ִ�. �׷���
	//cBuffer�����
	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	vector<class ModelMeshPart *> meshParts;
};