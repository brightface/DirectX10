#pragma once
#include "Systems/IExecute.h"

class ObbCollisionDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override{}
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	Shader* shader;

	Material* floor;
	MeshPlane* plane;

	Transform* transform[2];
	Collider* collider[2];
};