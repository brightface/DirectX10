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
	//x���� y���� �����ž�. 
	void GetBlurParameter(vector<float>& weights, vector<Vector2>& offsets, float x, float y);
	float GetGaussFunction(float val);

private:
	UINT targetIndex = 5;

	float threshold = 0.05f;
	//Ȧ�����ߵ�. �׷��� ��Ȯ�� ��. 3�̻� Ȧ�����ߵ�.
	UINT blurCount = 15;

	//x���� ����ġ 
	vector<float> weightX;
	//������ �ȼ� �÷�����.
	vector<Vector2> offsetX;
	//���̴��� ���������� �״����� �Ұž�.!
	vector<float> weightY;
	vector<Vector2> offsetY;

private:
	Shader* shader;
	//mrt �ѹ��� ���°� ����.
	RenderTarget* renderTarget[6];

	DepthStencil* depthStencil;
	Viewport* viewport;

	//�̷��� ������
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