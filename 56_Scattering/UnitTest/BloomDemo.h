#pragma once
#include "Systems/IExecute.h"

class BloomDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void Mesh();
	void Airplane();

	void Kachujin();
	void Weapon();

	void Billboards();
	void Weather();

	void PointLights();
	void SpotLights();

	void Pass(UINT mesh, UINT model, UINT anim);

private:
	void GetBlurParameter(vector<float>& weights, vector<Vector2>& offsets, float x, float y);

private:
	Shader* shader;

	RenderTarget* renderTarget[6];
	DepthStencil* depthStencil;
	Viewport* viewport;

	Render2D* render2D;
	PostEffect* postEffect;

	float threshold = 0.05f;
	UINT blurCount = 15;

	vector<float> weightX;
	vector<Vector2> offsetX;

	vector<float> weightY;
	vector<Vector2> offsetY;


	CubeSky* sky;

	Material* floor;
	Material* stone;
	Material* brick;
	Material* wall;

	MeshRender* cube;
	MeshRender* cylinder;
	MeshRender* sphere;
	MeshRender* plane;

	ModelRender* airplane = NULL;

	ModelAnimator* kachujin = NULL;
	ModelRender* weapon = NULL;
	Transform* weaponTransform;

	Billboard* billboard;

	enum class WeatherType
	{
		None = 0, Rain, Snow, Count,
	} weatherType;
	
	Rain* rain;
	Snow* snow;

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;
};