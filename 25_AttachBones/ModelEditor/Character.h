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

	void othersUpdate();
	void othersRender();


private:
	Shader* shader;

	ColliderObject* colliderObject;
	
	ModelAnimator* kachujin;
	ModelAnimator* others;
	Vector3 direction = Vector3(-1, -1, +1);

	//ColliderObject* colliderObject;
	float othersHP = 10;
	
	Transform* weaponinitTransform;//
	ModelRender* weapon;//

	//¿Ö
	//MeshRender* FullHP;
	Material* FHP;

};
