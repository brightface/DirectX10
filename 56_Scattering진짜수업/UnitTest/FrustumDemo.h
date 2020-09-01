#pragma once
#include "Systems/IExecute.h"

class FrustumDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override {}
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void CreateMeshData();

private:
	Material* red;
	Shader* gridShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	PerFrame* perFrame;

	vector<Transform *> transforms;


	Shader* shader;
	Material* floor;
	MeshRender* plane;


	float fov = 0.35f;
	float zFar = 100.0f;

	Fixity* camera;
	Perspective* perspective;
	Frustum* frustum;


	Shader* modelShader;
	ModelRender* model;
};