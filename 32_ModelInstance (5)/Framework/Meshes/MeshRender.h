#pragma once
#define MAX_MESH_INSTANCE 500
//500개 인스턴싱 이면 충분해

//2008 미만버전 blender 리딩이랑 하는거 읽혀둬 - 애니메이션
//에셋 스토어 다 fbx야. 블랜더에서 보정해서 와야하거든
//2014 assimp 맞춰놓고 쓴다. 그게 가장 정확해

class MeshRender
{
public:
	MeshRender(Shader* shader, Mesh* mesh);
	~MeshRender();

	Mesh* GetMesh() { return mesh; }

	void Update();
	void Render();

	void Pass(UINT val) { mesh->Pass(val); }

	Transform* AddTransform();
	Transform* GetTransform(UINT index) { return transforms[index]; }
	void UpdateTransforms();

private:
	Mesh* mesh;

	//스테틱이 트랜스폼 개수 = 인스턴싱의 개수
	vector<Transform *> transforms;
	Matrix worlds[MAX_MESH_INSTANCE];
	//트랜스 폼이 한번에 가져오든가

	//여기서 셋팅하고 인스턴싱으로 그릴거야.
	VertexBuffer* instanceBuffer;
};