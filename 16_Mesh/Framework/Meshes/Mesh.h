#pragma once

class Mesh
{
public:
	typedef VertexTextureNormal MeshVertex;

public:
	Mesh(Shader* shader);
	virtual ~Mesh(); //상속줄거니까 virtual 붙여야지

	void Update();
	void Render();

public:
	void Pass(UINT val) { pass = val; } //패스설정하는것

	void Position(float x, float y, float z);
	void Position(Vector3& vec);
	void Position(Vector3* vec);

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& vec);
	void Rotation(Vector3* vec);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& vec);
	void RotationDegree(Vector3* vec);

	void Scale(float x, float y, float z);
	void Scale(Vector3& vec);
	void Scale(Vector3* vec);

	Matrix World() { return world; }

	//월드를 이용해 리턴해주면 된다.
	Vector3 Forward(); //z
	Vector3 Up(); //y
	Vector3 Right(); //x 방향
	//텍스처설정할것
	void DiffuseMap(wstring file);

protected:
	//순수가상함수는 생성자에서 콜할수 없다.
	virtual void Create() = 0;
	void CreateBuffer();

private:
	void UpdateWorld();

protected:
	MeshVertex* vertices = NULL;
	UINT* indices = NULL;

	UINT vertexCount;
	UINT indexCount;

private:
	Shader* shader;
	UINT pass = 0;

	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 rotation = Vector3(0, 0, 0);
	//이 3가지가 필요한 월드
	Matrix world;


	ID3D11Buffer* vertexBuffer = NULL;
	ID3D11Buffer* indexBuffer = NULL;

	ID3DX11EffectMatrixVariable* sWorld, *sView, *sProjection;
	//텍스처 받아야하는것
	Texture* diffuseMap = NULL;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;
};