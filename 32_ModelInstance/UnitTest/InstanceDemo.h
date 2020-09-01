#pragma once
#include "Systems/IExecute.h"

#define InstanceDemo_MAX_COUNT 10000

class InstanceDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override {}
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
	VertexBuffer* colorBuffer;
	IndexBuffer* indexBuffer;

	PerFrame* perFrame;

	Transform* transforms[InstanceDemo_MAX_COUNT];
	Matrix worlds[InstanceDemo_MAX_COUNT];
	Color colors[InstanceDemo_MAX_COUNT];
};