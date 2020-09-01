#include "stdafx.h"
#include "TerrainEditor.h"

TerrainEditor::TerrainEditor(Shader* shader)
	: shader(shader)
{
	terrain = new Terrain(shader, L"Terrain/Gray256.dds");
	terrain->BaseMap(L"Terrain/Dirt3.png");
	terrain->Layer1(L"Terrain/Dirt.png");

	lineBuffer = new ConstantBuffer(&lineDesc, sizeof(LineDesc));
	sLineBuffer = shader->AsConstantBuffer("CB_TerrainLine");

	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BrushDesc));
	sBrushBuffer = shader->AsConstantBuffer("CB_TerrainBrush");
}

TerrainEditor::~TerrainEditor()
{
	SafeDelete(terrain);

	SafeDelete(lineBuffer);
	SafeDelete(brushBuffer);
}

void TerrainEditor::Update()
{
	ImGui::Text("");
	ImGui::Separator();
	ImGui::Text("Guide Line");
	ImGui::Separator();

	static bool bVisible = true;
	ImGui::Checkbox("LineVisible", &bVisible);
	lineDesc.Visible = bVisible ? 1 : 0;

	ImGui::ColorEdit3("LineColor", lineDesc.Color);
	ImGui::InputFloat("Thickness", &lineDesc.Thickness, 0.01f);
	ImGui::InputFloat("LineSize", &lineDesc.Size, 1.0f);

	ImGui::Separator();

	ImGui::ColorEdit3("BrushColor", brushDesc.Color);
	ImGui::InputInt("BrushType", (int *)&brushDesc.Type);
	brushDesc.Type %= 3;

	ImGui::InputInt("BrushRange", (int *)&brushDesc.Range);
	brushDesc.Range = Math::Clamp<UINT>(brushDesc.Range, 1, 10);


	if (brushDesc.Type == 1)
	{
		brushDesc.Location = terrain->GetRaycastPosition();

		if(Mouse::Get()->Press(0))
			RaiseHeight(brushDesc.Location, 5);
	}

	terrain->Update();
}

void TerrainEditor::Render()
{
	lineBuffer->Render();
	sLineBuffer->SetConstantBuffer(lineBuffer->Buffer());

	
	string str = "";
	str += to_string(brushDesc.Location.x);
	str += ", ";
	str += to_string(brushDesc.Location.y);
	str += ", ";
	str += to_string(brushDesc.Location.z);

	Gui::Get()->RenderText(10, 50, 1, 0, 0, str);

	brushBuffer->Render();
	sBrushBuffer->SetConstantBuffer(brushBuffer->Buffer());

	terrain->Render();
}

void TerrainEditor::RaiseHeight(Vector3 & center, float intensity)
{
	UINT left = (UINT)center.x - brushDesc.Range;
	UINT right = (UINT)center.x + brushDesc.Range;
	UINT bottom = (UINT)center.z - brushDesc.Range;
	UINT top = (UINT)center.z + brushDesc.Range;


	if (left < 0) left = 0;
	if (right >= terrain->width) right = terrain->width;
	if (bottom < 0) bottom = 0;
	if (top >= terrain->height) top = terrain->height;

	for (UINT z = bottom; z <= top; z++)
	{
		for (UINT x = left; x <= right; x++)
		{
			UINT index = terrain->width * z + x;

			terrain->vertices[index].Position.y += intensity * Time::Delta();
		}
	}
	terrain->CreateNormalVector();


	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(terrain->vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, terrain->vertices, sizeof(Terrain::VertexTerrain) * terrain->vertexCount);
	}
	D3D::GetDC()->Unmap(terrain->vertexBuffer->Buffer(), 0);
}
