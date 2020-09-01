#include "stdafx.h"
#include "Main.h"
#include "Systems/Window.h"

#include "WaterDemo.h"
#include "RefractionDemo.h"
#include "ReflectionDemo.h"
#include "ScatteringDemo.h"
#include "TerrainLODDemo.h"
#include "TsDistanceDemo.h"
#include "TsRectDemo.h"
#include "TsTriangleDemo.h"
#include "FrustumDemo.h"
#include "ShadowFrameworkDemo.h"
#include "ShadowDemo.h"
#include "DepthBufferDemo.h"
#include "DepthDemo.h"
#include "ProjectorDemo.h"
#include "BloomDemo.h"
#include "GaussianBlurDemo.h"
#include "BlurDemo.h"
#include "MrtDemo.h"
#include "ColorToneDemo.h"
#include "ViewportDemo.h"
#include "RtvDemo.h"
#include "GeometryBillboardDemo.h"
#include "WeatherDemo.h"
#include "BillboardDemo.h"
#include "AreaLightingDemo.h"
#include "NormalMapDemo.h"
#include "LightingDemo.h"

void Main::Initialize()
{
	Push(new WaterDemo());
	//Push(new RefractionDemo());
	//Push(new ReflectionDemo());
	//Push(new ScatteringDemo());
	//Push(new TerrainLODDemo());
	//Push(new TsDistanceDemo());
	//Push(new TsRectDemo());
	//Push(new TsTriangleDemo());
	//Push(new FrustumDemo());
	//Push(new ShadowFrameworkDemo());
	//Push(new ShadowDemo());
	//Push(new DepthBufferDemo());
	//Push(new DepthDemo());
	//Push(new ProjectorDemo());
	//Push(new BloomDemo());
	//Push(new GaussianBlurDemo());
	//Push(new BlurDemo());
	//Push(new MrtDemo());
	//Push(new ColorToneDemo());
	//Push(new ViewportDemo());
	//Push(new RtvDemo());
	//Push(new GeometryBillboardDemo());
	//Push(new WeatherDemo());
	//Push(new BillboardDemo());
	//Push(new AreaLightingDemo());
	//Push(new NormalMapDemo());
	//Push(new LightingDemo());
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
	for (IExecute* exe : executes)
		exe->PreRender();
}

void Main::Render()
{
	for (IExecute* exe : executes)
		exe->Render();
}

void Main::PostRender()
{
	for (IExecute* exe : executes)
		exe->PostRender();
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
	desc.Background = Color(1, 1, 1, 1);
	D3D::SetDesc(desc);

	Main* main = new Main();

	WPARAM wParam = Window::Run(main);
	SafeDelete(main);

	return wParam;
}