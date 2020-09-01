#pragma once
#include "Systems/IExecute.h"

class EnvMapDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}

private:
	void Mesh();
	void Airplane();

	void Kachujin();
	void Kachujin2();
	void Weapon();

	void PointLights();
	void SpotLights();

	void Pass(UINT mesh, UINT model, UINT anim);
	void SetTransform(Transform* transform);

private:
	Shader* shader;
	Shadow* shadow;
	EnvCubeMap* cubeMap;
	

	Sky* sky;
	Terrain* terrain;

	Material* floor;
	Material* stone;
	Material* brick;
	Material* wall;

	MeshRender* cube;
	MeshRender* cylinder;
	MeshRender* sphere;
	MeshRender* sphere2;

	ModelRender* airplane = NULL;

	ModelAnimator* kachujin = NULL;
	ModelAnimator* kachujin2 = NULL;
	ModelRender* weapon = NULL;
	Transform* weaponTransform;

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;
};