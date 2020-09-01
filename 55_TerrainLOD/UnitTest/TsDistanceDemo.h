#pragma once
#include "Systems/IExecute.h"

class TsDistanceDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override {}
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	Shader* shader;
	Transform* transform;
	PerFrame* perFrame;

	VertexBuffer* vertexBuffer;
};