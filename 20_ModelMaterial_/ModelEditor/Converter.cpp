#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

Converter::Converter()
{	//임포터를 생성할거야 소멸자에서는 지울거고
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile(wstring file)
{	
	this->file = L"../../_Assets/" + file;
	//파일의 기본은 여기에다 몰아넣을것이다. 
	//importer에 리드 파일을 하게 되면

	scene = importer->ReadFile
	(
		//스트링을 더블스트링으로 바꿔줄거야
		String::ToString(this->file), 
		//ccw가 반시계 counter clockwise, cw가 시계방향
		aiProcess_ConvertToLeftHanded //어떤식으로 바꿔줄거냐
		//이 옵션은 모델을 디자인할때  삼각형형식으로 바꿔서 
		| aiProcess_Triangulate 
		// 삼각형형식으로 바뀌면서 uv도 다 바껴야돼, 이것도 다시 계산해줘라. 이거없으면 uv못쓴다. 
		| aiProcess_GenUVCoords
		// ai normals 다시 계산 하라는거야
		| aiProcess_GenNormals
		// normalmap 할때 써. 탄젠트공간
		| aiProcess_CalcTangentSpace
	);

	//정상적이면 씬이 널이 나와 
	if (scene == NULL)
	{
		string str = importer->GetErrorString();
		//널이 아니면 메세지  스트링이 const_char "" 
		MessageBoxA(D3D::GetDesc().Handle, str.c_str(), "Assimp Error", MB_OK);
		exit(-1); //0미만값이면 비정상종료, 0이상이면 정상종료
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
		//printf랑 똑같다. f 파일포인터 쓰는것만 빼고
		fprintf(file, "%d,%d,%s\n", bone->Index, bone->Parent, bone->Name.c_str());
	}

	fprintf(file, "\n"); //한줄띄워주고
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
	//매터리얼 xml로 수정
	//정점은 액셀로
	//2진파일로 저장할거야

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
	//일반적인 그래픽 프로그래밍 오른손 좌표계의 열우선이야
	//mTransformation 이게 float4*4 , 생성자 transform으로 복사해 들어간다.
	Matrix transform(node->mTransformation[0]);
	//행우선은 열우선으로 바꾼다. 
	D3DXMatrixTranspose(&bone->Transform, &transform);

	//행우선을 사용하지 
	//하나의 명령어로 여러개의 데이터를 동시에 받겠다. 초대형 컴퓨터 여러개의 명령어 동시에 처리한다.
	//4개씩 처리후 

	//행우선 실제로 느려
	//dix 9 왼손좌표계 행우선을 사용했어, 9까지는 한계가 있다.
	//dx10 이후부터는 열우선을 사용해 오른손좌표계(right)

	//유니티 얼리얼 왼손좌표계사용해, 우리
	//일단은

	//근데 실제로 dx 직접적으로 개발할때는 오른손 좌표계 사용해
	//////////////////////////////////////////////////////////////////////
	//그다음 내 본을 부모를 가져다가 곱해줄거야. 부모가 움직인 만큼 내본도 움직어야 한다.
	//자기의 글로벌 출력된 월드로 만들어줄거야, //메시가 그 월드를 가져다가 그 월드의 위치를 랜더링할거야
	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;
	//이제 자기의 최종월드는 부모를 곱해주면 된다.
	bone->Transform = bone->Transform * matParent;
	
	//meshes 도 복수형이잖아
	
	bones.push_back(bone);
	//배열아니면 벡터

	//ainode안에 mesh라는게 있다. 메시정보를 직접적으로 가지고있는게 아니라 UNIT형으로 참조할 번호를 가지고있다.
	//	이배열에 접근해서 메시를 가져올거야.
	//이 정보들이 노드안에 있다.
	//순회하면서 하려고 함수를 하나 더 만들거야
	ReadMeshData(node, index); 
	//index는 본번호
	/////////////////////////////////

	//현재노드하고 현재 인덱스를 넘겨서 순회할거야
	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);
}

void Converter::ReadMeshData(aiNode * node, int index)
{
	if (node->mNumMeshes < 1) return;

	asMesh* mesh = new asMesh();
	mesh->BoneIndex = index;
	//하나하나의 메시는 번호를 가지고있다.
	//메시 정보를 가지고 있는게 아니라.
	//누구를 참조할? scece에 있는 mMeshes 실제 정보는 여기에 있다.
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{	//메시랑 1:1매칭되는게 아니라 메시안에있는 메시 파트랑 1:1매칭되는것이다.
		UINT index = node->mMeshes[i];
		//////////////////////////////////이게 존나 중요하네//////
		aiMesh* srcMesh = scene->mMeshes[index];

		//start 버택스는 mesh의 vertices의 사이즈다.
		//다음점 되었을때도 쌓아가니까.
		UINT startVertex = mesh->Vertices.size();
		UINT startIndex = mesh->Indices.size();
		//정점을 가지고 있는것 다 돌거거든(정점갯수)
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));
			//testure좌표계 uv 좌표계 있느냐 , 유일하게 번호가 들어가 uv가 여러개가 있는게 있어
			//texture layor 문신 같은거 사실 다른걸로 써 , 게임에서는 0번만 써
			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));
			//노멜벡터를 가지고 있느냐 가지고 있으면 복사해주면 되겟지
			//메시의 정점에 추가해줄거야.
			//만든걸
			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}
		//triangle face 나오면 나누기 3했다고하면 돼
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{//디자인에서는 face 라고 불러, triangle이잖아 디자인
			aiFace& face = srcMesh->mFaces[f];
			//페이스 요소 하나당 인덱스 하나와 같아. 순대대로 정렬되어있다. 
			//받은 그대로 누적시키면 돼.

			for (UINT k = 0; k < face.mNumIndices; k++)
				mesh->Indices.push_back(face.mIndices[k] + startVertex);
				//정점의 개수만큼 더해서 만들어 준거야.
		}

		//재질. scene의 배열로 붙어있다.mMaterials배열로 저장되어있고 메시의 번호로 참조한다.
		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		//메터리얼 번호말고 이름을 넣어줄거야. 순서를 바꿀수 있어서, 그게 편해서 이름을 넣어준다.
		
		//포문안에서 한번이 메시파트 하나이다.
		asMeshPart* meshPart = new asMeshPart();
		meshPart->MaterialName = material->GetName().C_Str();
		meshPart->StartVertex = startVertex;
		meshPart->StartIndex = startIndex;
		meshPart->VertexCount = srcMesh->mNumVertices;
		//메시파트의면의 개수 * 면마다 가지고 있는 인덱스의 개수
		meshPart->IndexCount = srcMesh->mNumFaces * srcMesh->mFaces->mNumIndices;
		//그래야 우리의 인덱스의 개수가 나오지.
		
		//우리의 메시파트에다가 저장해놓고
		mesh->MeshParts.push_back(meshPart);
	} //for(i) - 올라가봐야하잖나, 귀찮으니,

	meshes.push_back(mesh);
}

void Converter::ReadSkinData()
{
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene  -> mMeshes[i];// 본하고 연결되어있는 건 메시쓰면 안돼
		if (aiMesh->HasBones() == false) continue;

		asMesh* mesh = meshes[i];

		vector<asBoneWeights> boneWeights;
		boneWeights.assign(mesh->Vertices.size(),asBoneWeights());
		//미리 사이즈 잡아놓는다.

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
			//있는지 없는지 검사해서 끼워넣은거고 그다음에
			//없으면 맨뒤ㅔㅇ 넣는다.
			for (UINT w = 0; w < aiMeshBone->mNumWeights; w++) //해당넘의 가중치를 가져올려고 하는것이다.
			{
				UINT index = aiMeshBone->mWeights[w].mVertexId;
				float weight = aiMeshBone->mWeights[w].mWeight;

				//index - 정점번호
				//boneindex 0 index의 정점이 영향을 받을 본번호
				boneWeights[index].AddWeights(boneindex, weight);
			}
		}

		for (UINT w = 0; w < boneWeights.size(); w++)
		{
			boneWeights[w].Normalize();

			asBlendWeight blendWeight;
			boneWeights[w].GetBlendWeights(blendWeight); //float4로 넘긴다.

			//가중치가 정점이랑 일치해 

			mesh->Vertices[w].BlendIndices = blendWeight.Indices;
			mesh->Vertices[w].BlendWeights = blendWeight.Weights;
			//각 정점의 웨이트

		}
	}
}

void Converter::WriteMeshData(wstring savePath)
{
	//폴더를 만들겠다
	//Path::CreateFolders(savePath);
	//폴더명만 가져와서 폴더를 만들어.
	Path::CreateFolders(Path::GetDirectoryName(savePath));
	//////////////////////////////////////////////////////
	//이게 안되는데??./////////////////////////////
	//_model지우니까 안 만들어져///////////////////
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
