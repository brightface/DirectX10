#pragma once
class ModelBone
{
	public:
		friend class Model;
		//�����带 �̿��ؼ� �����ش�. ����������
		//namespace�� �̿��ؼ� �����ٶ��� �ִ�. ū����
		//�ȿ��� �Ʒ� �̿� ����
	private:
		ModelBone();
		~ModelBone();
		//���ʿ��� �����ٶ�

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

	void Pass(UINT val);
	void SetShader(Shader* shader);

	void Update();
	void Render();

	int BoneIndex() { return boneIndex; }
	class ModelBone* Bone() { return bone; }
	
	void Transforms(Matrix* transforms);
	void SetTransform(Transform* transform);
private:
	struct BoneDesc
	{
		Matrix Transform[MAX_MODEL_TRANSFORMS];
		//transform���ٷ��� �е��־����?
		float Padding[3];
	}boneDesc;

private:
	Shader* shader;
	Transform* transfrom = NULL;
	//���迭 ��ü�� �������ٰŰŵ�

	PerFrame* perFrame = NULL;

	VertexBuffer* vertexBuffer;
	UINT vertexCount;
	Model::ModelVertex* vertices;

	IndexBuffer* indexBuffer;
	UINT indexCount;
	UINT* indices;
	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	vector<class ModelMeshPart *> meshParts;
};