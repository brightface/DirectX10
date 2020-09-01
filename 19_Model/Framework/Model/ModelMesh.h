#pragma once
class ModelBone
{
	public:
		friend class Model;
		//프렌드를 이용해서 열어준다. 작은구역들
		//namespace를 이용해서 열어줄때도 있다. 큰구역
		//안열면 아래 이용 못해
	private:
		ModelBone();
		~ModelBone();
		//한쪽에만 열어줄때

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
		//transform해줄려면 패딩있어야지?
		float Padding[3];
	}boneDesc;

private:
	Shader* shader;
	Transform* transfrom = NULL;
	//본배열 전체를 복사해줄거거든

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