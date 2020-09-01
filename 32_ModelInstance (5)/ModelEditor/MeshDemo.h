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
	Shader* shader;

	Material* planeMaterial;
	MeshRender* plane; //밑판은 하나야.

	//박스 메테리얼 / 카메라는 맨뒤에 배치
	Material* boxMaterial;
	MeshRender* box;
};