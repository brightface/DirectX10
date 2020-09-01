#pragma once
#include "Systems/IExecute.h"

class ExportFile : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override {} //안쓸거잖아 {} 해준다.
	virtual void Update() override {}//안쓸거잖아 {} 해준다.
	virtual void PreRender() override {}//안쓸거잖아 {} 해준다.
	virtual void Render() override {}//안쓸거잖아 {} 해준다.
	virtual void PostRender() override {}//안쓸거잖아 {} 해준다.
	virtual void ResizeScreen() override {}//안쓸거잖아 {} 해준다.

private:
	void Tank();
	void Tower();
	void Kachujin();
	void Airplane();
};