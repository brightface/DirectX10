#pragma once
#define MAX_MESH_INSTANCE 500
//500�� �ν��Ͻ� �̸� �����

//2008 �̸����� blender �����̶� �ϴ°� ������ - �ִϸ��̼�
//���� ����� �� fbx��. �������� �����ؼ� �;��ϰŵ�
//2014 assimp ������� ����. �װ� ���� ��Ȯ��

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

	//����ƽ�� Ʈ������ ���� = �ν��Ͻ��� ����
	vector<Transform *> transforms;
	Matrix worlds[MAX_MESH_INSTANCE];
	//Ʈ���� ���� �ѹ��� �������簡

	//���⼭ �����ϰ� �ν��Ͻ����� �׸��ž�.
	VertexBuffer* instanceBuffer;
};