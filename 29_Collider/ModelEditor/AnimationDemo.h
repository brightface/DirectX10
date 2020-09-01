#pragma once
#include "Systems/IExecute.h"

class AnimationDemo : public IExecute
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
	void Kachujin();

private:
	Shader* shader;

	ColliderObject* colliderObject;
	ModelAnimator* kachujin;
	Vector3 direction = Vector3(-1, -1, +1);
	bool bIntersect = false;
};