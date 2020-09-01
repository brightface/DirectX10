#include "stdafx.h"
#include "ExportFIle.h"
#include "Converter.h"
void ExportFile::Initialize()
{
	Tank();
}

void ExportFile::Tank()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Tank/Tank.fbx");
	conv->ExportMesh(L"");
	SafeDelete(conv);
}
