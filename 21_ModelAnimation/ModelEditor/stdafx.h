#pragma once

#include "Framework.h"
#pragma comment(lib, "../Debug/Framework.lib")

#include "Assimp/Importer.hpp" //보낸파일 읽어들이는거, h와 cpp 합쳐놓은것, 템플릿쓸때많이써
#include "Assimp/scene.h" //불러들여서 모든정보를 가지고 있다.
#include "Assimp/postprocess.h" //모델파일 어떻게 부를지를 결정
#pragma comment(lib, "Assimp/assimp-vc140-mt.lib")  //라이브러리도 하나 링크할거야
//따른데서 어심프를 쓸 이유가 없다. 이프로젝트에서만 추가함. framework에서만 쓰면 돼
