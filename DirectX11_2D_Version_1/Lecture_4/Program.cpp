#include "stdafx.h"
#include "Window.h"

int APIENTRY WinMain
(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow
)
{
	Window::Create(hInstance, 500, 500);
	Window::Show();

	while (Window::Update())
	{

	}

	Window::Destroy();
	return 0;
}