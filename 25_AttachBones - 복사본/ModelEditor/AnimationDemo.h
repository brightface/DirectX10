#pragma once
#include "Systems/IExecute.h"

#define PLAYERIDLE 0
#define PLAYERATTACK 1
#define ENEMYIDLE 0
#define ENEMYHIT 1
#define ENEMYDIE 2

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
	void Police();
	void Enemy();
private:
	Shader* shader;

	//�浹ü
	//ModelRender* weapon;
	//Transform* weaponInitTransform;
	ColliderObject** colliderObject;

	ColliderObject* colliderObject;
	ModelAnimator* kachujin;
	ModelAnimator* police;
	ModelAnimator* enemy;
	Vector3 direction = Vector3(-1, -1, +1);
};