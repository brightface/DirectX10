#pragma once
#include "Systems/IExecute.h"

class MeshDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override {}
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void Airplane();
	void Tower();
	void Tank();

private:
	Shader* shader;

	ModelRender* airplane = NULL;
	ModelRender* tower = NULL;
	ModelRender* tank = NULL;

	Material* boxMaterial;
	CubeSky* sky;
	MeshPlane* grid;
};