#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

Converter::Converter()
{	//�����͸� �����Ұž� �Ҹ��ڿ����� ����Ű�
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile(wstring file)
{	
	this->file = L"../../_Assets/" + file;
	//������ �⺻�� ���⿡�� ���Ƴ������̴�. 
	//importer�� ���� ������ �ϰ� �Ǹ�

	scene = importer->ReadFile
	(
		//��Ʈ���� ����Ʈ������ �ٲ��ٰž�
		String::ToString(this->file), 
		//ccw�� �ݽð� counter clockwise, cw�� �ð����
		aiProcess_ConvertToLeftHanded //������� �ٲ��ٰų�
		//�� �ɼ��� ���� �������Ҷ�  �ﰢ���������� �ٲ㼭 
		| aiProcess_Triangulate 
		// �ﰢ���������� �ٲ�鼭 uv�� �� �ٲ��ߵ�, �̰͵� �ٽ� ��������. �̰ž����� uv������. 
		| aiProcess_GenUVCoords
		// ai normals �ٽ� ��� �϶�°ž�
		| aiProcess_GenNormals
		// normalmap �Ҷ� ��. ź��Ʈ����
		| aiProcess_CalcTangentSpace
	);

	//�������̸� ���� ���� ���� 
	if (scene == NULL)
	{
		string str = importer->GetErrorString();
		//���� �ƴϸ� �޼���  ��Ʈ���� const_char "" 
		MessageBoxA(D3D::GetDesc().Handle, str.c_str(), "Assimp Error", MB_OK);
		exit(-1); //0�̸����̸� ����������, 0�̻��̸� ��������
	}
}

void Converter::ExportMesh(wstring savePath)
{
	ReadBoneData(scene->mRootNode, -1, -1);
	int a = 0;
	FILE* file;
	fopen_s(&file, "../Meshes.csv", "w");

	for (asBone* bone : bones)
	{
		string name = bone->Name;
		//printf�� �Ȱ���. f ���������� ���°͸� ����
		fprintf(file, "%d,%d,%s\n", bone->Index, bone->Parent, bone->Name.c_str());
	}

	fprintf(file, "\n"); //���ٶ���ְ�
	for (UINT i = 0; i < meshes.size(); i++)
	{
		fprintf(file, "Mesh Index : %d\n", i);
		
		asMesh* mesh = meshes[i];
		for (UINT v = 0; v < mesh->Vertices.size(); v++)
		{
			Vector3 p = mesh->Vertices[v].Position;

			fprintf(file, "%f,%f,%f\n", p.x, p.y, p.z);
		}
		
		fprintf(file, "\n");
	}
	fclose(file);
	//���͸��� xml�� ����
	//������ �׼���
	//2�����Ϸ� �����Ұž�

//	FILE* file;
//   fopen_s(&file, "../Vertices.csv", "w");
//
//   for (asBone* bone : bones)
//   {
//      string name = bone->Name;
//      fprintf(file, "%d,%s\n", bone->Index, bone->Name.c_str());
//   }
//
//   fprintf(file, "\n");
//   for (asMesh* mesh : meshes)
//   {
//      string name = mesh->Name;
//      fprintf(file, "%s\n", name.c_str());
//
//      for (UINT i = 0; i < mesh->Vertices.size(); i++)
//      {
//         Vector3 p = mesh->Vertices[i].Position;
//         Vector4 indices = mesh->Vertices[i].BlendIndices;
//         Vector4 weights = mesh->Vertices[i].BlendWeights;
//
//         fprintf(file, "%f,%f,%f,", p.x, p.y, p.z);
//         fprintf(file, "%f,%f,%f,%f,", indices.x, indices.y, indices.z, indices.w);
//         fprintf(file, "%f,%f,%f,%f\n", weights.x, weights.y, weights.z, weights.w);
//      }
//   }
//   fclose(file);
//
//	savePath = L"../../_Models/" + savePath + L".mesh";
//	WriteMeshData(savePath);
//}
}
void Converter::ReadBoneData(aiNode * node, int index, int parent)
{
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();
	//mTramsformation : aiMatrix 4*4
	//�Ϲ����� �׷��� ���α׷��� ������ ��ǥ���� ���켱�̾�
	//mTransformation �̰� float4*4 , ������ transform���� ������ ����.
	Matrix transform(node->mTransformation[0]);
	//��켱�� ���켱���� �ٲ۴�. 
	D3DXMatrixTranspose(&bone->Transform, &transform);

	//��켱�� ������� 
	//�ϳ��� ��ɾ�� �������� �����͸� ���ÿ� �ްڴ�. �ʴ��� ��ǻ�� �������� ��ɾ� ���ÿ� ó���Ѵ�.
	//4���� ó���� 

	//��켱 ������ ����
	//dix 9 �޼���ǥ�� ��켱�� ����߾�, 9������ �Ѱ谡 �ִ�.
	//dx10 ���ĺ��ʹ� ���켱�� ����� ��������ǥ��(right)

	//����Ƽ �󸮾� �޼���ǥ������, �츮
	//�ϴ���

	//�ٵ� ������ dx ���������� �����Ҷ��� ������ ��ǥ�� �����
	//////////////////////////////////////////////////////////////////////
	//�״��� �� ���� �θ� �����ٰ� �����ٰž�. �θ� ������ ��ŭ ������ ������� �Ѵ�.
	//�ڱ��� �۷ι� ��µ� ����� ������ٰž�, //�޽ð� �� ���带 �����ٰ� �� ������ ��ġ�� �������Ұž�
	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;
	//���� �ڱ��� ��������� �θ� �����ָ� �ȴ�.
	bone->Transform = bone->Transform * matParent;
	
	//meshes �� ���������ݾ�
	
	bones.push_back(bone);
	//�迭�ƴϸ� ����

	//ainode�ȿ� mesh��°� �ִ�. �޽������� ���������� �������ִ°� �ƴ϶� UNIT������ ������ ��ȣ�� �������ִ�.
	//	�̹迭�� �����ؼ� �޽ø� �����ðž�.
	//�� �������� ���ȿ� �ִ�.
	//��ȸ�ϸ鼭 �Ϸ��� �Լ��� �ϳ� �� ����ž�
	ReadMeshData(node, index); 
	//index�� ����ȣ
	/////////////////////////////////

	//�������ϰ� ���� �ε����� �Ѱܼ� ��ȸ�Ұž�
	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);
}

void Converter::ReadMeshData(aiNode * node, int index)
{
	if (node->mNumMeshes < 1) return;

	asMesh* mesh = new asMesh();
	mesh->BoneIndex = index;
	//�ϳ��ϳ��� �޽ô� ��ȣ�� �������ִ�.
	//�޽� ������ ������ �ִ°� �ƴ϶�.
	//������ ������? scece�� �ִ� mMeshes ���� ������ ���⿡ �ִ�.
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{	//�޽ö� 1:1��Ī�Ǵ°� �ƴ϶� �޽þȿ��ִ� �޽� ��Ʈ�� 1:1��Ī�Ǵ°��̴�.
		UINT index = node->mMeshes[i];
		//////////////////////////////////�̰� ���� �߿��ϳ�//////
		aiMesh* srcMesh = scene->mMeshes[index];

		//start ���ý��� mesh�� vertices�� �������.
		//������ �Ǿ������� �׾ư��ϱ�.
		UINT startVertex = mesh->Vertices.size();
		UINT startIndex = mesh->Indices.size();
		//������ ������ �ִ°� �� ���Űŵ�(��������)
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));
			//testure��ǥ�� uv ��ǥ�� �ִ��� , �����ϰ� ��ȣ�� �� uv�� �������� �ִ°� �־�
			//texture layor ���� ������ ��� �ٸ��ɷ� �� , ���ӿ����� 0���� ��
			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));
			//��Ấ�͸� ������ �ִ��� ������ ������ �������ָ� �ǰ���
			//�޽��� ������ �߰����ٰž�.
			//�����
			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}
		//triangle face ������ ������ 3�ߴٰ��ϸ� ��
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{//�����ο����� face ��� �ҷ�, triangle���ݾ� ������
			aiFace& face = srcMesh->mFaces[f];
			//���̽� ��� �ϳ��� �ε��� �ϳ��� ����. ������ ���ĵǾ��ִ�. 
			//���� �״�� ������Ű�� ��.

			for (UINT k = 0; k < face.mNumIndices; k++)
				mesh->Indices.push_back(face.mIndices[k] + startVertex);
				//������ ������ŭ ���ؼ� ����� �ذž�.
		}

		//����. scene�� �迭�� �پ��ִ�.mMaterials�迭�� ����Ǿ��ְ� �޽��� ��ȣ�� �����Ѵ�.
		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		//���͸��� ��ȣ���� �̸��� �־��ٰž�. ������ �ٲܼ� �־, �װ� ���ؼ� �̸��� �־��ش�.
		
		//�����ȿ��� �ѹ��� �޽���Ʈ �ϳ��̴�.
		asMeshPart* meshPart = new asMeshPart();
		meshPart->MaterialName = material->GetName().C_Str();
		meshPart->StartVertex = startVertex;
		meshPart->StartIndex = startIndex;
		meshPart->VertexCount = srcMesh->mNumVertices;
		//�޽���Ʈ�Ǹ��� ���� * �鸶�� ������ �ִ� �ε����� ����
		meshPart->IndexCount = srcMesh->mNumFaces * srcMesh->mFaces->mNumIndices;
		//�׷��� �츮�� �ε����� ������ ������.
		
		//�츮�� �޽���Ʈ���ٰ� �����س���
		mesh->MeshParts.push_back(meshPart);
	} //for(i) - �ö󰡺������ݳ�, ��������,

	meshes.push_back(mesh);
}

void Converter::ReadSkinData()
{
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene  -> mMeshes[i];// ���ϰ� ����Ǿ��ִ� �� �޽þ��� �ȵ�
		if (aiMesh->HasBones() == false) continue;

		asMesh* mesh = meshes[i];

		vector<asBoneWeights> boneWeights;
		boneWeights.assign(mesh->Vertices.size(),asBoneWeights());
		//�̸� ������ ��Ƴ��´�.

		for (UINT b = 0; b< aiMesh->mNumBones; b++)
		{
			aiBone* aiMeshBone = aiMesh-> mBones[b];

			UINT boneindex = 0;
			for (asBone* bone : bones)
			{
				if (bone->Name == (string)aiMeshBone->mName.C_Str())
				{
					boneindex = bone->Index;

					break;
				}
			}//for(bone)
			//�ִ��� ������ �˻��ؼ� ���������Ű� �״�����
			//������ �ǵڤĤ� �ִ´�.
			for (UINT w = 0; w < aiMeshBone->mNumWeights; w++) //�ش���� ����ġ�� �����÷��� �ϴ°��̴�.
			{
				UINT index = aiMeshBone->mWeights[w].mVertexId;
				float weight = aiMeshBone->mWeights[w].mWeight;

				//index - ������ȣ
				//boneindex 0 index�� ������ ������ ���� ����ȣ
				boneWeights[index].AddWeights(boneindex, weight);
			}
		}

		for (UINT w = 0; w < boneWeights.size(); w++)
		{
			boneWeights[w].Normalize();

			asBlendWeight blendWeight;
			boneWeights[w].GetBlendWeights(blendWeight); //float4�� �ѱ��.

			//����ġ�� �����̶� ��ġ�� 

			mesh->Vertices[w].BlendIndices = blendWeight.Indices;
			mesh->Vertices[w].BlendWeights = blendWeight.Weights;
			//�� ������ ����Ʈ

		}
	}
}

void Converter::WriteMeshData(wstring savePath)
{
	//������ ����ڴ�
	//Path::CreateFolders(savePath);
	//������ �����ͼ� ������ �����.
	Path::CreateFolders(Path::GetDirectoryName(savePath));
	//////////////////////////////////////////////////////
	//�̰� �ȵǴµ�??./////////////////////////////
	//_model����ϱ� �� �������///////////////////
	BinaryWriter* w = new BinaryWriter();
	w->Open(savePath);

	w->UInt(bones.size());
	for (asBone* bone : bones)
	{
		w->Int(bone->Index);
		w->String(bone->Name);
		w->Int(bone->Parent);
		w->Matrix(bone->Transform);

		SafeDelete(bone);
	}

	w->UInt(meshes.size());
	for (asMesh* meshData : meshes)
	{
		w->Int(meshData->BoneIndex);

		w->UInt(meshData->Vertices.size());
		w->Byte(&meshData->Vertices[0], sizeof(Model::ModelVertex) * meshData->Vertices.size());

		w->UInt(meshData->Indices.size());
		w->Byte(&meshData->Indices[0], sizeof(UINT) * meshData->Indices.size());


		w->UInt(meshData->MeshParts.size());
		for (asMeshPart* part : meshData->MeshParts)
		{
			w->String(part->MaterialName);

			w->UInt(part->StartVertex);
			w->UInt(part->VertexCount);

			w->UInt(part->StartIndex);
			w->UInt(part->IndexCount);

			SafeDelete(part);
		}

		SafeDelete(meshData);
	}

	w->Close();
	SafeDelete(w);
}

void Converter::ExportMaterial(wstring savePath, bool bOverwrite)
{
	savePath = L"../../_Textures/" + savePath + L".material";

	if (bOverwrite == false)
	{
		if (Path::ExistFile(savePath) == true)
			return;
	}


	ReadMaterialData();	
	WriteMaterialData(savePath);
}

void Converter::ReadMaterialData()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		asMaterial* material = new asMaterial();

		material->Name = srcMaterial->GetName().C_Str();


		aiColor3D color;

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->Ambient = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->Diffuse = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->Specular = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, material->Specular.a);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->Emissive = Color(color.r, color.g, color.b, 1.0f);


		aiString file;

		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->DiffuseFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SpecularFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->NormalFile = file.C_Str();

		materials.push_back(material);
	}
}

void Converter::WriteMaterialData(wstring savePath)
{
	string folder = String::ToString(Path::GetDirectoryName(savePath));
	string file = String::ToString(Path::GetFileName(savePath));

	Path::CreateFolders(folder);


	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Materials");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);

	for (asMaterial* material : materials)
	{
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		
		Xml::XMLElement* element = NULL;

		element = document->NewElement("Name");
		element->SetText(material->Name.c_str());
		node->LinkEndChild(element);

		
		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->DiffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->SpecularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->NormalFile).c_str());
		node->LinkEndChild(element);


		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->Ambient.r);
		element->SetAttribute("G", material->Ambient.g);
		element->SetAttribute("B", material->Ambient.b);
		element->SetAttribute("A", material->Ambient.a);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->Diffuse.r);
		element->SetAttribute("G", material->Diffuse.g);
		element->SetAttribute("B", material->Diffuse.b);
		element->SetAttribute("A", material->Diffuse.a);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->Specular.r);
		element->SetAttribute("G", material->Specular.g);
		element->SetAttribute("B", material->Specular.b);
		element->SetAttribute("A", material->Specular.a);
		node->LinkEndChild(element);

		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->Emissive.r);
		element->SetAttribute("G", material->Emissive.g);
		element->SetAttribute("B", material->Emissive.b);
		element->SetAttribute("A", material->Emissive.a);
		node->LinkEndChild(element);

		SafeDelete(material);
	}


	document->SaveFile((folder + file).c_str());
	SafeDelete(document);
}

string Converter::WriteTexture(string saveFolder, string file)
{
	if (file.length() < 1) 
		return "";

	string fileName = Path::GetFileName(file);
	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

	string path = "";
	if (texture != NULL)
	{
		path = saveFolder + Path::GetFileNameWithoutExtension(file) + ".png";

		if (texture->mHeight < 1)
		{
			BinaryWriter w;
			w.Open(String::ToWString(path));
			w.Byte(texture->pcData, texture->mWidth);
			w.Close();
		}
		else
		{
			D3D11_TEXTURE2D_DESC destDesc;
			ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
			destDesc.Width = texture->mWidth;
			destDesc.Height = texture->mHeight;
			destDesc.MipLevels = 1;
			destDesc.ArraySize = 1;
			destDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			destDesc.SampleDesc.Count = 1;
			destDesc.SampleDesc.Quality = 0;
			destDesc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = texture->pcData;


			ID3D11Texture2D* dest;

			HRESULT hr;
			hr = D3D::GetDevice()->CreateTexture2D(&destDesc, &subResource, &dest);
			assert(SUCCEEDED(hr));

			D3DX11SaveTextureToFileA(D3D::GetDC(), dest, D3DX11_IFF_PNG, saveFolder.c_str());
		}
	}
	else
	{
		string directory = Path::GetDirectoryName(String::ToString(this->file));
		string origin = directory + file;
		String::Replace(&origin, "\\", "/");

		if (Path::ExistFile(origin) == false)
			return "";


		path = saveFolder + fileName;
		CopyFileA(origin.c_str(), path.c_str(), FALSE);

		String::Replace(&path, "../../_Textures/", "");
	}

	return Path::GetFileName(path);
}
