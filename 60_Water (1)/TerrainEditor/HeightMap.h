#pragma once
#include "Systems/IExecute.h"

class HeightMap : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override {}
	virtual void Update() override {}
	virtual void PreRender() override {}
	virtual void Render() override {}
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	enum class Channel
	{
		Red = 0, Green, Blue, Alpha
	};

private:
	void ToPng(wstring heightFile, wstring fileName, Channel channel);
};