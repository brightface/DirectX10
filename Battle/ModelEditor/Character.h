#pragma once
#include "Systems/IExecute.h"
#define PLAYERIDLE 0
#define PLAYERATTACK 1
#define ENEMYIDLE 0
#define ENEMYHIT 1
#define ENEMYDIE 2

class Character : public IExecute
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

	void Player();
	void Enemy();

	void playerUpdate();
	void playerRender();
	void enemyUpdate();
	void enemyRender();


	float playerRot = 270.0f;
	bool isLeft = false;
	int playerState = PLAYERIDLE;
	float attackTime = 0.0f;
	int enemyState = ENEMYIDLE;
	float HitTime = 0.0f;
	float endTimer = 0.0f;

	float enemyHP = 5;

private:
	Shader* shader;

	ModelAnimator* kachujin;
	ModelAnimator* player;
	ModelAnimator* enemy;

	ColliderObject** colliderObject;
	ColliderObject** enemyCollider;

	Transform* weaponInitTransform;
	ModelRender* weapon;

	MeshRender* FullHP;
	Material* FHP;
	MeshRender* myHP;
	Material* MHP;
};