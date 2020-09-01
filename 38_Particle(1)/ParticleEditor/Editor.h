#pragma once
#include "Systems/IExecute.h"

class Editor : public IExecute
{
	Shader* shader;
	CubeSky* sky;

	float windowWidth = 500;

	bool bLoop = false;
	UINT maxParticle = 0;

	vector<wstring> particleList;
	vector<wstring> textureList;

	wstring file = L"";
	ParticleSystem* particleSystem = NULL;

	Material* floor;
	Material* stone;

	MeshRender* sphere;
	MeshRender* grid;
public:
	virtual void Initialize() override;
	virtual void Destroy() override {}
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void Mesh();
	void UpdateParticleList();
	void UpdateTextureList();

	void onGUI(); 
	Particle* particle;

	CubeSky* sky;

	Material* floor;
	Material* stone;
	MeshRender* sphere;
	MeshRender* grid;
};