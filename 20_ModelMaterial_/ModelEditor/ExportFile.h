#pragma once
#include "Systems/IExecute.h"

class ExportFile : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override {} //�Ⱦ����ݾ� {} ���ش�.
	virtual void Update() override {}//�Ⱦ����ݾ� {} ���ش�.
	virtual void PreRender() override {}//�Ⱦ����ݾ� {} ���ش�.
	virtual void Render() override {}//�Ⱦ����ݾ� {} ���ش�.
	virtual void PostRender() override {}//�Ⱦ����ݾ� {} ���ش�.
	virtual void ResizeScreen() override {}//�Ⱦ����ݾ� {} ���ش�.

private:
	void Tank();
	void Tower();
	void Kachujin();
	void Airplane();
};