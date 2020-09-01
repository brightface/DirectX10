#include "stdafx.h"
#include "MapEditor.h"

void MapEditor::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(21, 0, 0);
	Context::Get()->GetCamera()->Position(126, 35, 39);
	((Freedom *)Context::Get()->GetCamera())->Speed(100);


	shader = new Shader(L"50_Framework.fxo");
	shadow = new Shadow(shader, Vector3(128, 0, 128), 65);

	sky = new CubeSky(L"Environment/GrassCube1024.dds", shader);

	
	editor = new TerrainEditor(shader);
}

void MapEditor::Update()
{
	sky->Update();
	editor->Update();
}

void MapEditor::PreRender()
{
	shadow->PreRender();
}

void MapEditor::Render()
{
	sky->Pass(3);
	sky->Render();

	editor->Pass(7);
	editor->Render();
}

