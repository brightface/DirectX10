#pragma once
#include "Systems/IExecute.h"

class GetHeightDemo : public IExecute
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
	Terrain* terrain;

	//cpp에서 intialize 에서 vertices 선언해주고 나면
	//이 vector3의 포지션이 그 vertice에 영향을 맏는다.
	//왜 그럴까??
	Vector3 position = Vector3(0, 0, 0);

	Shader* triShader;
	ID3D11Buffer* vertexBuffer;
};