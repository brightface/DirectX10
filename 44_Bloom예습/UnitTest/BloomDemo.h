#pragma once
#include "Systems/IExecute.h"

class BloomDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void AddBillboard();

	void Mesh();
	void Airplane();
	
	void Kachujin();
	void Weapon();

	void PointLights();
	void SpotLights();

	void Pass(UINT mesh, UINT model, UINT anim);

private:
	void SetBlur();
	//x방향 y방향 만들어낼거야. 
	void GetBlurParameter(vector<float>& weights, vector<Vector2>& offsets, float x, float y);
	float GetGaussFunction(float val);

private:
	UINT targetIndex = 5;

	float threshold = 0.05f;
	//홀수여야돼. 그래야 정확히 들어가. 3이상 홀수여야돼.
	UINT blurCount = 15;

	//x방향 가중치 
	vector<float> weightX;
	//옆으로 픽셀 늘려갈것.
	vector<Vector2> offsetX;
	//쉐이더에 던진다음에 그다음에 할거야.!
	vector<float> weightY;
	vector<Vector2> offsetY;

private:
	Shader* shader;
	//mrt 한번에 가는게 빨라.
	RenderTarget* renderTarget[6];

	DepthStencil* depthStencil;
	Viewport* viewport;

	//이렇게 넣을게
	Render2D* render2D;
	PostEffect* postEffect;
	
	
	CubeSky* sky;

	Billboard* billboard;

	Material* floor;
	Material* stone;
	Material* brick;
	Material* wall;

	MeshRender* cube;
	MeshRender* cylinder;
	MeshRender* sphere;
	MeshRender* grid;

	ModelRender* airplane = NULL;

	ModelAnimator* kachujin = NULL;	
	ModelRender* weapon = NULL;
	Transform* weaponTransform;

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;
};