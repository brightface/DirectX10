#pragma once

class ModelBone
{
public:
	friend class Model;

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
	void Pass(UINT val);
	void SetShader(Shader* shader);

	void Update();
	//void Render();
	void Render(UINT drawCount); //바귄다 이걸로

	int BoneIndex() { return boneIndex; }
	class ModelBone* Bone() { return bone; }

	//1차원 배열이 아니라 텍스처로 바꼈잖아 필요 없어지지
	//void Transforms(Matrix* transforms);
	//void SetTransform(Transform* transform);

	//이거 생긴다. srv 넘겨줄거 텍스처만들어서
	void TransformsSRV(ID3D11ShaderResourceView* srv);

private:
	struct BoneDesc
	{
		//필요없어. 몇번본인지만 넘어가잖아.
		//Matrix Transforms[MAX_MODEL_TRANSFORMS];

		UINT BoneIndex;
		float Padding[3];
	} boneDesc;

private:
	Shader* shader;

	//필요없다. 인스턴싱 정보로 넘어가니까
	//Transform* transform = NULL;
	PerFrame* perFrame = NULL;

	int boneIndex;
	ModelBone* bone;

	VertexBuffer* vertexBuffer;
	UINT vertexCount;
	Model::ModelVertex* vertices;

	IndexBuffer* indexBuffer;
	UINT indexCount;
	UINT* indices;

	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	vector<class ModelMeshPart *> meshParts;

	//쉐이더로 넘어가야하니까 이것도 들어가야하지 새로
	ID3D11ShaderResourceView* transformsSRV = NULL;
	ID3DX11EffectShaderResourceVariable* sTransformsSRV;
};