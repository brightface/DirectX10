#pragma once
#include "Systems/IExecute.h"

class UnProjectionDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override {}
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	
	//지면 하나만 그린다음에 

private:
	Shader* shader;


	CubeSky* sky;
	//Terrain* terrain;
};