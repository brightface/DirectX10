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

	//업데이트 본즈- 이거 지웠다.(회전할때)

	void Pass(UINT pass);

	//이거 모델본으로 받는게 아니라 본 인덱스로 받고 매트릭스로 받는게 아니라 트랜스 폼으로 받늗나.
	//void UpdateBoneTransform(UINT instance, ModelBone* bone=NULL, Matrix& matrix = Matrix()); - 텍스처를 만들때 이미 부모자식간에 만들어줬잖아 그래서 이렇게 트랜스폼으로 바뀐다고.
	void UpdateBoneTransform(UINT instance, UINT boneIndex, Transform* transform); //본이 널일수가 없다. 본의 번호로받았을때 
	//재귀할라고 만든다.
	void UpdateBoneTransform(UINT instance, ModelBone* bone, Transform* transform);

private:
	void UpdateBoneTransform();//아무것도 없는애야. 왜만들었까?
	//신나게 본정보를 업데이트 했잖아. gpu로 보내줘야지 텍스처 정보를 보내줘야할거 아니야
	//전체 사이즈만큼 맵해주면 되잖아. 그럴려면

private:
	void CreateTexture();

private:
	Shader* shader;
	Model* model;
	//메시 랜더로 박스 그렸지?
	//모델 랜더와 메시 랜더와 똑같은 구조야. 트랜스폼 먼저 선언
	vector<Transform *> transforms;
	//이러면 인스턴스 하나당 모든 본이 같다고 보잖아. 그래서 나는 2차원 배열로 만들거야 본을 수정할수 있게

	Matrix worlds[MAX_MODEL_INSTANCE];
	VertexBuffer* instanceBuffer;
	//이제 본 한번에 밀어서 계산해야되거든
	//모든 모델이 다같은 행렬가지는거 아니지. 회전한다면
	//모든 모델이 다같은 트랜스폼 가지는거 아니지/본값이 인스턴트 에 따라 바뀌잖아
	//3차열 하나 놓고 1차원 배열 - 4차원배열

	//y를 인스턴스로 만들거야. x는 bone
	//5번땡크에 목이 돌아가게 만들겠다. 가능

	//모델랜더에 넣어준다. 모델 애니메이터에 있던것
	Color colors[MAX_MODEL_INSTANCE];
	VertexBuffer* instanceColorBuffer;

	//그래서 이렇게 본 트랜스폼으로 2차원배열로 바꿔줄거야. 매트릭스 채로 쉐이더로 넘길수 있을까? 
	//한번에 넘길수 있는거 4996개 // 
	Matrix boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_TRANSFORMS];
	//만얼마 나와

	//본 넘길때 텍스처로 넘겨. 비디오 메모리가 허용하면 된다고
	ID3D11Texture2D* texture = NULL;
	ID3D11ShaderResourceView* transformsSRV;
	//위에 올라가 GetTransform 빠진다.
};