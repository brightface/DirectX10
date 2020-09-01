#pragma once
#include "Systems/IExecute.h"

class ThreadDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override {}
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override {}
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void Loop();

	void Loop1();
	void Loop2();
	void MultiThread();

	void Mutex();

	void Racer(int& counter);
	void RaceCondition();

	void ExecTimer();
	
private:
	mutex m;
	float progress = 0.0f;

	Timer timer;
	Timer timer2;
};