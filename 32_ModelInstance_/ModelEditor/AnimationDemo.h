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

	ModelAnimator* kachujin;

	Transform* colliderInitTransform;
	ColliderObject** colliderObject;//콜라이더가 트랜스폼 갯수만큼 생겨야하잖아.
	
	Transform* weaponInitTransform;
	ModelRender* weapon;
	
	bool bIntersect = false;
};