#pragma once
#include "Systems/IExecute.h"

class ProjectionDemo : public IExecute
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

	Vector3 direction = Vector3(-1, -1, +1);
	ID3DX11EffectVectorVariable* sDirection;

	CubeSky* sky;

	MeshSphere* marker;

	MeshCube* cube;
	MeshPlane* plane;
	MeshCylinder* cylinder[10];
	MeshSphere* sphere[10];
};