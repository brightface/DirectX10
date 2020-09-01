#pragma once

//Window
#include <Windows.h>
#include <assert.h>

//STL
#include <vector>

//DirectX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h> //이게 없어졌지만 우리는 넣겟다.

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib") //쉐이더 써야돼 신버전은 필요없어
#pragma comment(lib, "d3dx10.lib") //쉐이더 써야돼 신버전은 필요없어
#pragma comment(lib, "d3dcompiler.lib")

typedef unsigned int uint;

//Macro Function
#define SAFE_DELETE(p)			{ if (p) { delete (p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p)			{ if (p) { (p)->Release(); (p) = nullptr; } }

//Framework
#include "Settings.h"