#include "stdafx.h"
#include "Execute.h"
#include "Graphics.h"

Execute::Execute()
{
	graphics = new Graphics();
	graphics->Initialize();
	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));
}

Execute::~Execute()
{
	SAFE_DELETE(graphics);
}

void Execute::Update()
{
}

void Execute::Render()
{
	graphics->Begin();
	{

	}
	graphics->End();
}
