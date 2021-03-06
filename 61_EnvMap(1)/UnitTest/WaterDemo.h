#pragma once
#include "Systems/IExecute.h"

class WaterDemo : public IExecute
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
	void Weapon();

	void PointLights();
	void SpotLights();

	void Pass(UINT mesh, UINT model, UINT anim);
	void SetTransform(Transform* transform);

private:
	Shader* shader;
	Shadow* shadow;
	Water* water;

	Sky* sky;
	Terrain* terrain;

	Material* floor;
	Material* stone;
	Material* brick;
	Material* wall;

	MeshRender* cube;
	MeshRender* cylinder;
	MeshRender* sphere;

	ModelRender* airplane = NULL;

	ModelAnimator* kachujin = NULL;
	ModelRender* weapon = NULL;
	Transform* weaponTransform;

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;
};