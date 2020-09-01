#pragma once

class Mesh : public Renderer
{
public:
	typedef VertexTextureNormal MeshVertex;

public:
	Mesh(Shader* shader);
	virtual ~Mesh();
	//set shader instance 에서 관리할거야
	void SetShader 

	void Update() override;
	void Render() override;

public:
	void DiffuseMap(wstring file);

protected:
	virtual void Create() = 0;
	Shader* shader;
	UINT pass = 0;

	PerFrame* perFrame = NULL;

	VertexBuffer* vertexBuffer = NULL;
	IndexBuffer* indexBuffer = NULL;
	
	MeshVertex* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;

protected:
	MeshVertex* vertices = NULL;
	UINT* indices = NULL;

private:
	Texture* diffuseMap = NULL;
	ID3DX11EffectShaderResourceVariable* sDiffuseMap;
};