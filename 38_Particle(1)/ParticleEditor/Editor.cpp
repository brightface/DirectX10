#include "stdafx.h"
#include "Editor.h"
#include "Utilities/.h"
void Editor::Initialize()
{
	particle = new Particle(L"Fire");

	Context::Get
		shader

		sky->Update();

	grid->Update();
	sphere->Update();


	Vector3 position;
	sphere->GetTransform(0)->Position(&position);

	if (Keyboard::Get()->Press('L'))
		position.x += 20 * Time::Delta();
	else if (Keyboard::Get()->Press('J'))
		position.x -= 20 * Time::Delta();

	if (Keyboard::Get()->Press('I'))
		position.z += 20 * Time::Delta();
	else if (Keyboard::Get()->Press('K'))
		position.z -= 20 * Time::Delta();

	if (Keyboard::Get()->Press('O'))
		position.y += 20 * Time::Delta();
	else if (Keyboard::Get()->Press('U'))
		position.y -= 20 * Time::Delta();

	sphere->GetTransform(0)->Position(position);
	sphere->UpdateTransforms();

	void Editor::Mesh()
	{
		//Create Material
		{
			floor = new Material(shader);
			floor->DiffuseMap("Floor.png");
			floor->NormalMap("Floor_Normal.png");
			floor->Specular(1, 1, 1, 20);
			floor->SpecularMap("Floor_Specular.png");

			stone = new Material(shader);
			stone->DiffuseMap("Stones.png");
			stone->NormalMap("Stones_Normal.png");
			stone->Specular(1, 1, 1, 20);
			stone->SpecularMap("Stones_Specular.png");
			stone->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		}


		//Create Mesh
		{
			Transform* transform = NULL;

			grid = new MeshRender(shader, new MeshGrid(5, 5));
			transform = grid->AddTransform();
			transform->Position(0, 0, 0);
			transform->Scale(12, 1, 12);

			sphere = new MeshRender(shader, new MeshSphere(0.5f, 20, 20));
			transform = sphere->AddTransform();
			transform->Position(0, 5, 0);
			transform->Scale(5, 5, 5);
		}

		sphere->UpdateTransforms();
		grid->UpdateTransforms();
	}
}

void Editor::Update()
{
	onGUI();//Imgui들
	particle->Add(Vector3(0, 0, 0));
	particle->Update();
}

void Editor::Render()
{
	particle->Render();
}

void Editor::UpdateParticleList()
{
	particleList.clear();
	Path::GetFiles(&particleList, L"../../_Textures/Particles/", L"*.xml", false);

	for (wstring& file : particleList)
		file = Path::GetFileNameWithoutExtension(file);
}

void Editor::UpdateTextureList()
{
	textureList.clear();

	vector<wstring> files;
	Path::GetFiles(&files, L"../../_Textures/Particles/", L"*.*", false);

	for (wstring file : files)
	{
		wstring ext = Path::GetExtension(file);
		transform(ext.begin(), ext.end(), ext.begin(), toupper);

		file = Path::GetFileName(file);
		//이것만 가져올려고 한거야
		if (ext == L"PNG" || ext == L"TGA" || ext == L"JPG")
			textureList.push_back(file);
	}
}
void Editor::OnGUI()
{
	float width = D3D::Width();
	float height = D3D::Height();

	bool bOpen = true;
	bOpen = ImGui::Begin("Particle", &bOpen);
	ImGui::SetWindowPos(ImVec2(width - windowWidth, 0));
	ImGui::SetWindowSize(ImVec2(windowWidth, height));
	{
		OnGUI_List();
		OnGUI_Settings();
	}
	ImGui::End();
}

void Editor::OnGUI_Write()
{
	ImGui::Separator();

	if (ImGui::Button("WriteParticle"))
	{
		D3DDesc desc = D3D::GetDesc();

		Path::SaveFileDialog
		(
			file, L"Particle file\0*.xml", L"../../_Textures/Particles",
			bind(&Editor::WriteFile, this, placeholders::_1),
			desc.Handle
		);
	}
}

void Editor::WriteFile(wstring file)
{
	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Particle");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);


	Xml::XMLElement* node = NULL;

	node = document->NewElement("BlendState");
	node->SetText((int)Particle->GetData().Type);
	root->LinkEndChild(node);


	string textureFile = String::ToString(Particle->GetData().TextureFile);
	String::Replace(&textureFile, "Particles/", "");

	node = document->NewElement("Loop");
	node->SetText(Particle->GetData().bLoop);
	root->LinkEndChild(node);

	node = document->NewElement("TextureFile");
	node->SetText(textureFile.c_str());
	root->LinkEndChild(node);


	node = document->NewElement("MaxParticles");
	node->SetText(Particle->GetData().MaxParticles);
	root->LinkEndChild(node);


	node = document->NewElement("ReadyTime");
	node->SetText(Particle->GetData().ReadyTime);
	root->LinkEndChild(node);

	node = document->NewElement("ReadyRandomTime");
	node->SetText(Particle->GetData().ReadyRandomTime);
	root->LinkEndChild(node);

	node = document->NewElement("StartVelocity");
	node->SetText(Particle->GetData().StartVelocity);
	root->LinkEndChild(node);

	node = document->NewElement("EndVelocity");
	node->SetText(Particle->GetData().EndVelocity);
	root->LinkEndChild(node);


	node = document->NewElement("MinHorizontalVelocity");
	node->SetText(Particle->GetData().MinHorizontalVelocity);
	root->LinkEndChild(node);

	node = document->NewElement("MaxHorizontalVelocity");
	node->SetText(Particle->GetData().MaxHorizontalVelocity);
	root->LinkEndChild(node);

	node = document->NewElement("MinVerticalVelocity");
	node->SetText(Particle->GetData().MinVerticalVelocity);
	root->LinkEndChild(node);

	node = document->NewElement("MaxVerticalVelocity");
	node->SetText(Particle->GetData().MaxVerticalVelocity);
	root->LinkEndChild(node);


	node = document->NewElement("Gravity");
	node->SetAttribute("X", Particle->GetData().Gravity.x);
	node->SetAttribute("Y", Particle->GetData().Gravity.y);
	node->SetAttribute("Z", Particle->GetData().Gravity.z);
	root->LinkEndChild(node);


	node = document->NewElement("MinColor");
	node->SetAttribute("R", Particle->GetData().MinColor.r);
	node->SetAttribute("G", Particle->GetData().MinColor.g);
	node->SetAttribute("B", Particle->GetData().MinColor.b);
	node->SetAttribute("A", Particle->GetData().MinColor.a);
	root->LinkEndChild(node);

	node = document->NewElement("MaxColor");
	node->SetAttribute("R", Particle->GetData().MaxColor.r);
	node->SetAttribute("G", Particle->GetData().MaxColor.g);
	node->SetAttribute("B", Particle->GetData().MaxColor.b);
	node->SetAttribute("A", Particle->GetData().MaxColor.a);
	root->LinkEndChild(node);


	node = document->NewElement("MinRotateSpeed");
	node->SetText(Particle->GetData().MinRotateSpeed);
	root->LinkEndChild(node);

	node = document->NewElement("MaxRotateSpeed");
	node->SetText(Particle->GetData().MaxRotateSpeed);
	root->LinkEndChild(node);

	node = document->NewElement("MinStartSize");
	node->SetText((int)Particle->GetData().MinStartSize);
	root->LinkEndChild(node);

	node = document->NewElement("MaxStartSize");
	node->SetText((int)Particle->GetData().MaxStartSize);
	root->LinkEndChild(node);

	node = document->NewElement("MinEndSize");
	node->SetText((int)Particle->GetData().MinEndSize);
	root->LinkEndChild(node);

	node = document->NewElement("MaxEndSize");
	node->SetText((int)Particle->GetData().MaxEndSize);
	root->LinkEndChild(node);

	wstring folder = Path::GetDirectoryName(file);
	wstring fileName = Path::GetFileNameWithoutExtension(file);

	document->SaveFile(String::ToString(folder + fileName + L".xml").c_str());
	SafeDelete(document);

	UpdateParticleList();
}