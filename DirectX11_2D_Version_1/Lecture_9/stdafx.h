#pragma once

#ifdef _DEBUG //전처리기 정의에 되어 (속성에) 있어서 쓸수있다. //디버깅모드에만 통한다.
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") //링커 연결시킬떄 쓴다. 또다른 콘솔창띄울수있다.
#endif

//Window
#include <Windows.h>
#include <assert.h>

//STL
#include <vector>

//DirectX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")

typedef unsigned int uint;

//Macro Function
#define SAFE_DELETE(p)			{ if (p) { delete (p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p)			{ if (p) { (p)->Release(); (p) = nullptr; } }

//Framework
#include "Settings.h"