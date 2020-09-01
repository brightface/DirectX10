#pragma once

#include "Framework.h"
#pragma comment(lib, "../Debug/Framework.lib")

#include "Assimp/Importer.hpp" //모델파일 fbx 파일 읽어오는것.
#include "Assimp/scene.h" //모델정보를 가지고 있다 
#include "Assimp/postprocess.h"//모델파일 어떻게 부를 지를 결정해준다.
#pragma comment(lib, "Assimp/assimp-vc140-mt.lib") //Assimp 관련 라이브러리
