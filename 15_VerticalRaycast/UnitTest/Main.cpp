#include "stdafx.h"
#include "Main.h"
#include "Systems/Window.h"

#include "VerticalRaycastDemo.h"
#include "GetHeightDemo.h"
#include "TerrainDemo.h"
#include "TextureSamplerDemo.h"
#include "TextureDemo.h"
#include "CubeDemo.h"
#include "GridDemo.h"
#include "IndexDemo.h"
#include "World2Demo.h"
#include "WorldDemo.h"
#include "VertexMoveDemo.h"
#include "RectDemo.h"
#include "TriangleListDemo.h"
#include "VertexLineDemo.h"
#include "VertexLine2Demo.h"

void Main::Initialize()
{
	//Push(new VerticalRaycastDemo());
	//Push(new GetHeightDemo());
	Push(new TerrainDemo());
	//Push(new TextureSamplerDemo());
	//Push(new TextureDemo());
	//Push(new CubeDemo());
	//Push(new GridDemo());
	//Push(new IndexDemo());
	//Push(new World2Demo());
	//Push(new WorldDemo());
	//Push(new VertexMoveDemo());
	//Push(new RectDemo());
	//Push(new TriangleListDemo());
	//Push(new VertexLine2Demo());
	//Push(new VertexLineDemo());
}

void Main::Destroy()
{
	for (IExecute* exe : executes)
	{
		exe->Destroy();
		SafeDelete(exe);
	}
}

void Main::Update()
{
	for (IExecute* exe : executes)
		exe->Update();
}

void Main::PreRender()
{
}

void Main::Render()
{
	for (IExecute* exe : executes)
		exe->Render();
}

void Main::PostRender()
{
}

void Main::ResizeScreen()
{
}

void Main::Push(IExecute * execute)
{
	executes.push_back(execute);

	execute->Initialize();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	D3DDesc desc;
	desc.AppName = L"D3D Game";
	desc.Instance = instance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Handle = NULL;
	desc.Width = 1280;
	desc.Height = 720;
	desc.Background = Color(0.3f, 0.3f, 0.3f, 1.0f);
	D3D::SetDesc(desc);

	Main* main = new Main();

	WPARAM wParam = Window::Run(main);
	SafeDelete(main);

	return wParam;
}