#pragma once

class ModelBone
{
public:
	friend class Model;
	//모델에서만 접근할수 있게 해놓았다.
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
	//패스번호 바꿔주는것. 자기거 세팅하고 다 자식으로 넘겨줘 즉 메시파트로
	void Pass(UINT val);
	//이것도 위와 마찬가지 쉐이더가 바끼면 주소가 다 바뀐다. 그것때문에 있는것이다.
	void SetShader(Shader* shader);

	void Update();
	void Render();
	
	int BoneIndex() { return boneIndex; }
	class ModelBone* Bone() { return bone; }

	void Transforms(Matrix* transforms);
	void SetTransform(Transform* transform);

private:
	
	struct BoneDesc
	{//최대갯수 정해준거야, 쉐이더로 보낼때는 가변으로 보낼수 없어, 무조건 고정이야
		Matrix Transforms[MAX_MODEL_TRANSFORMS];
		
		//현재 메시가 참조할 본번호 ,쉐이더에서 다 선택해서 그 월드들을 조합해.
		UINT BoneIndex;
		//이럴려면 패딩 필요해, 16바이트씩 묶여야 한다.
		float Padding[3];
	} boneDesc;

private:
	Shader* shader;

	//월드정보 였고
	Transform* transform = NULL;
	//매프레임마다 공통되는것 밀어주는 클래스
	PerFrame* perFrame = NULL;
	
	//참조할 본번호 (메시가 참조해서 그럴거잖아)
	int boneIndex;
	//참조할 개체
	ModelBone* bone;

	VertexBuffer* vertexBuffer;
	UINT vertexCount;
	Model::ModelVertex* vertices;

	IndexBuffer* indexBuffer;
	UINT indexCount;
	UINT* indices;
	//모델 메시는 모델메시파트를 관리하는 목적이 있다. 그래서
	//cBuffer만들고
	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	vector<class ModelMeshPart *> meshParts;
};