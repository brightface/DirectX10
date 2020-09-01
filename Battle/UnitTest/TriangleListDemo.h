#pragma once
#include "Systems/IExecute.h"

class TriangleListDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	Shader* shader;

	Vertex vertices[3];
	ID3D11Buffer* vertexBuffer;

	VertexColor vertices2[3];
	ID3D11Buffer* vertexBuffer2;
};