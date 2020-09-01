#pragma once
#include "Systems/IExecute.h"

class InstanceDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void CreateMesh();

private:
	Shader* shader;
	Material* material;

	vector<Mesh::MeshVertex> vertices;
	vector<UINT> indices;

	VertexBuffer* vertexBuffer;
	VertexBuffer* instanceBuffer;
	IndexBuffer* indexBuffer;

	PerFrame* perFrame;

	Transform* transforms[1000];
	Matrix worlds[1000];
};