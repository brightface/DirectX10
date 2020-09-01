#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

Converter::Converter()
{	//생성자는 임포터만 한다.
	importer = new Assimp::Importer();
}

Converter::~Converter()
{	//소멸자는 임포터를 지운다 어씸프를 지우면 씬도 지워진다.
	SafeDelete(importer);
}

void Converter::ReadFile(wstring file)
{
	this->file = L"../../_Assets/" + file;

	scene = importer->ReadFile
	(
		String::ToString(this->file), //wstring을 string으로
		aiProcess_ConvertToLeftHanded //왼손좌표계로 바꾸기 //행우선 열우선 바꾸기, 원래 오른손좌표계써
		| aiProcess_Triangulate //삼각형형식으로 바뀌면서 uv로 다 바껴야돼
		| aiProcess_GenUVCoords //삼각형방식 uv 틀어져 uv 다시 계산
		| aiProcess_GenNormals //노멀도 다 바뀌고
		| aiProcess_CalcTangentSpace //탄젠트 공간 노멀맵 할때 나온다.
	);

	if (scene == NULL)
	{
		string str = importer->GetErrorString();
		//A가 스트링형이다.
		//유니코드 == 멀티 바이트 코드 차이
		//wstring은 멀티바이트 코드야 1바이트 혹은 2바이트야.
		//한글은 2바이트야. 2바이트 처리
		//아스키코드는 t스트링 1바이트야, 무조건 1바이트야. 아스키코드

		//아스키 코드A
		MessageBoxA(D3D::GetDesc().Handle, str.c_str(), "Assimp Error", MB_OK);
		exit(-1);
	}
}

void Converter::ExportMesh(wstring savePath)
{
	//리드본 하면 메시데이타랑 본데이타 같이 불러진다.
	ReadBoneData(scene->mRootNode, -1, -1);
	ReadSkinData(); //그다음 스킨데이터를 읽늗나.
	
	//여기다가 본정보 저장
	FILE* file;
	fopen_s(&file, "../Meshes.csv", "w");
	//메모리 열었으면 닫아라. 해제하거나. 습관을 들여

	for (asBone* bone : bones)
	{
		string name = bone->Name;
		fprintf(file, "%d,%d,%s\n", bone->Index, bone->Parent, bone->Name.c_str());
	}

	fprintf(file, "\n");
	/*for (UINT i = 0; i < meshes.size(); i++)
	{
		fprintf(file, "Mesh Index : %d\n", i);
		
		asMesh* mesh = meshes[i];
		for (UINT v = 0; v < mesh->Vertices.size(); v++)
		{
			Vector3 p = mesh->Vertices[v].Position;

			fprintf(file, "%f,%f,%f\n", p.x, p.y, p.z);
		}
		
		fprintf(file, "\n");
	}*/
	fclose(file);


	//FILE* file;
	//fopen_s(&file, "../Vertices.csv", "w");

	//for (asBone* bone : bones)
	//{
	//	string name = bone->Name;
	//	fprintf(file, "%d,%s\n", bone->Index, bone->Name.c_str());
	//}

	//fprintf(file, "\n");
	//for (asMesh* mesh : meshes)
	//{
	//	string name = mesh->Name;
	//	fprintf(file, "%s\n", name.c_str());

	//	for (UINT i = 0; i < mesh->Vertices.size(); i++)
	//	{
	//		Vector3 p = mesh->Vertices[i].Position;
	//		Vector4 indices = mesh->Vertices[i].BlendIndices;
	//		Vector4 weights = mesh->Vertices[i].BlendWeights;

	//		fprintf(file, "%f,%f,%f,", p.x, p.y, p.z);
	//		fprintf(file, "%f,%f,%f,%f,", indices.x, indices.y, indices.z, indices.w);
	//		fprintf(file, "%f,%f,%f,%f\n", weights.x, weights.y, weights.z, weights.w);
	//	}
	//}
	//fclose(file);


	savePath = L"../../_Models/" + savePath + L".mesh";
	WriteMeshData(savePath);
}

void Converter::ReadBoneData(aiNode * node, int index, int parent)
{
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();

	Matrix transform(node->mTransformation[0]); //노드의 시작번지가 돼 . 우리의 형태로 변환 
	D3DXMatrixTranspose(&bone->Transform, &transform);
	//전치행렬//트랜스포스

	Matrix matParent;
	if (parent < 0)//-1 이면 나다. 부모니까.
		D3DXMatrixIdentity(&matParent);
	else //아니면
		matParent = bones[parent]->Transform;

	//여기가 핵심이야. 
	bone->Transform = bone->Transform * matParent;
	bones.push_back(bone);

	ReadMeshData(node, index);

	/////////////////여기서 재귀를 타면서 본을 만들면서 부르겠다.
	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);
}

void Converter::ReadMeshData(aiNode * node, int index)
{
	if (node->mNumMeshes < 1) return;

	asMesh* mesh = new asMesh();
	mesh->BoneIndex = index;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		
		UINT startVertex = mesh->Vertices.size();
		UINT startIndex = mesh->Indices.size();

		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}

		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
				mesh->Indices.push_back(face.mIndices[k] + startVertex);
		}

		
		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];

		asMeshPart* meshPart = new asMeshPart();
		meshPart->MaterialName = material->GetName().C_Str();
		meshPart->StartVertex = startVertex;
		meshPart->StartIndex = startIndex;
		meshPart->VertexCount = srcMesh->mNumVertices;
		meshPart->IndexCount = srcMesh->mNumFaces * srcMesh->mFaces->mNumIndices;

		mesh->MeshParts.push_back(meshPart);
	} //for(i)

	meshes.push_back(mesh);
}

void Converter::ReadSkinData()
{	//전체메시를 돌거야  메시하고 노드를 매칭시켜 갈거야
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{	
		aiMesh* aiMesh = scene->mMeshes[i];
		//메시하고 본하고 연결되어있는 경우가 아닌경우 그 메시를 쓰면 안돼
		if (aiMesh->HasBones() == false) continue;
		//똑같은걸 저장해놓잖아
		asMesh* mesh = meshes[i];
		//저장할 구조체를 만들어 놓을거야. 그래서 타입으로 가
		vector<asBoneWeights> boneWeights;
		//정점에다 들어갈애들이거든. 이값은 해당메시가 가지고 있는 정점에 개수랑 같겠지.
		boneWeights.assign(mesh->Vertices.size(), asBoneWeights());
		//assign 미리 자료형만큼 크기 잡아놓는것.
		
		//node 개념하고 똑같아. 메시를 출력하기 위해 사용하는것이 노드 ,본이란 것은 애니메이션을 시키기 위해 사용하는것.
		for (UINT b = 0; b < aiMesh->mNumBones; b++)
		{
			aiBone* aiMeshBone = aiMesh->mBones[b];
			//이걸 매칭시킬거야. 매칭은 기존에 멀로할까? 이름을 하겠지. 순서랑 본은 다르게 저장될수 있잖아. 이름으로 매칭시킬거야.
			UINT boneIndex = 0;
			for (asBone* bone : bones)
			{
				//본들을 다 돌려 //fbs가 가지고 있는 본하고 일치시켜서 가져 올려고 하는거야.
				if (bone->Name == (string)aiMeshBone->mName.C_Str())
				{
					boneIndex = bone->Index;
					break;
				}
			}//for(bone)
			//해당 가중치를 가져올려고 하는거야. 해당 메시에 대한
			for (UINT w = 0; w < aiMeshBone->mNumWeights; w++)
			{
				//[w]가 가중치의 번호가 될것이다.
				UINT index = aiMeshBone->mWeights[w].mVertexId;
				float weight = aiMeshBone->mWeights[w].mWeight;
				//해당 본만큼 영향을 준 가중치가 [w]에 번호에 있다.

				//index - 정점 번호
				//boneIndex - index의 정점이 영향을 받을 본 번호
				boneWeights[index].AddWeights(boneIndex, weight); //본번호 대로 저장을할거야
			}//for(w)
		}//for(b)

		//index가 정점번호야
		//boneIndex -index의 정점이 영향을 받을 본번호

		for (UINT w = 0; w < boneWeights.size(); w++)
		{	
			boneWeights[w].Normalize();

			asBlendWeight blendWeight;
			boneWeights[w].GetBlendWeights(blendWeight);

			mesh->Vertices[w].BlendIndices = blendWeight.Indices;
			mesh->Vertices[w].BlendWeights = blendWeight.Weights;
		}
	}
}

void Converter::WriteMeshData(wstring savePath)
{
	Path::CreateFolders(Path::GetDirectoryName(savePath));

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

void Converter::ClipList(vector<wstring>* list)
{
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* anim = scene->mAnimations[i];

		list->push_back(String::ToWString(anim->mName.C_Str()));
	}
}

void Converter::ExportAnimClip(UINT index, wstring savePath)
{
	savePath = L"../../_Models/" + savePath + L".clip";

	asClip* clip = ReadClipData(scene->mAnimations[index]);
	WriteClipData(clip, savePath);
}

asClip * Converter::ReadClipData(aiAnimation * animation)
{
	asClip* clip = new asClip();
	clip->Name = animation->mName.C_Str();
	clip->FrameRate = (float)animation->mTicksPerSecond;
	clip->FrameCount = (UINT)animation->mDuration + 1;

	vector<asClipNode> aniNodeInfos;
	for (UINT i = 0; i < animation->mNumChannels; i++)
	{
		//scene->mRootNode : 씬의 루트(본)노드 - ModelBone
		//aniNode : 애니메이션의 (본)노드
		aiNodeAnim* aniNode = animation->mChannels[i];

		asClipNode aniNodeInfo;
		aniNodeInfo.Name = aniNode->mNodeName;

		UINT keyCount = max(aniNode->mNumPositionKeys, aniNode->mNumRotationKeys);
		keyCount = max(keyCount, aniNode->mNumScalingKeys);


		asKeyframeData frameData;
		for (UINT k = 0; k < keyCount; k++)
		{
			bool bFound = false;
			UINT t = aniNodeInfo.Keyframe.size();

			if (fabsf((float)aniNode->mPositionKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiVectorKey key = aniNode->mPositionKeys[k];
				memcpy_s(&frameData.Translation, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));
				frameData.Frame = (float)aniNode->mPositionKeys[k].mTime;

				bFound = true;
			}

			if (fabsf((float)aniNode->mRotationKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiQuatKey key = aniNode->mRotationKeys[k];

				frameData.Rotation.x = key.mValue.x;
				frameData.Rotation.y = key.mValue.y;
				frameData.Rotation.z = key.mValue.z;
				frameData.Rotation.w = key.mValue.w;

				frameData.Frame = (float)aniNode->mRotationKeys[k].mTime;

				bFound = true;
			}

			if (fabsf((float)aniNode->mScalingKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiVectorKey key = aniNode->mScalingKeys[k];
				memcpy_s(&frameData.Scale, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));
				frameData.Frame = (float)aniNode->mScalingKeys[k].mTime;

				bFound = true;
			}

			if (bFound == true)
				aniNodeInfo.Keyframe.push_back(frameData);
		}

		if (aniNodeInfo.Keyframe.size() < clip->FrameCount)
		{
			UINT count = clip->FrameCount - aniNodeInfo.Keyframe.size();
			asKeyframeData keyframe = aniNodeInfo.Keyframe.back();

			for (UINT n = 0; n < count; n++)
				aniNodeInfo.Keyframe.push_back(keyframe);
		}

		clip->Duration = max(clip->Duration, aniNodeInfo.Keyframe.back().Frame);

		aniNodeInfos.push_back(aniNodeInfo);
	}

	ReadKeyframeData(clip, scene->mRootNode, aniNodeInfos);

	return clip;
}

void Converter::ReadKeyframeData(asClip * clip, aiNode * node, vector<asClipNode>& aniNodeInfos)
{
	asKeyframe* keyframe = new asKeyframe();
	keyframe->BoneName = node->mName.C_Str();
	
	for (UINT i = 0; i < clip->FrameCount; i++)
	{
		asClipNode* asClipNode = NULL;
		for (UINT n = 0; n < aniNodeInfos.size(); n++)
		{
			if (aniNodeInfos[n].Name == node->mName)
			{
				asClipNode = &aniNodeInfos[n];

				break;
			}
		}//for(n)


		asKeyframeData frameData;
		if (asClipNode == NULL)
		{
			Matrix transform(node->mTransformation[0]);
			D3DXMatrixTranspose(&transform, &transform);

			D3DXMatrixDecompose(&frameData.Scale, &frameData.Rotation, &frameData.Translation, &transform);
			frameData.Frame = (float)i;
		}
		else
		{
			frameData = asClipNode->Keyframe[i];
		}

		keyframe->Transforms.push_back(frameData);
	}

	clip->Keyframes.push_back(keyframe);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadKeyframeData(clip, node->mChildren[i], aniNodeInfos);
}

void Converter::WriteClipData(asClip * clip, wstring savePath)
{
	Path::CreateFolders(Path::GetDirectoryName(savePath));

	BinaryWriter* w = new BinaryWriter();
	w->Open(savePath);

	w->String(clip->Name);
	w->Float(clip->Duration);
	w->Float(clip->FrameRate);
	w->UInt(clip->FrameCount);

	w->UInt(clip->Keyframes.size());
	for (asKeyframe* keyframe : clip->Keyframes)
	{
		w->String(keyframe->BoneName);

		w->UInt(keyframe->Transforms.size());
		w->Byte(&keyframe->Transforms[0], sizeof(asKeyframeData) * keyframe->Transforms.size());

		SafeDelete(keyframe);
	}

	w->Close();
	SafeDelete(w);
}
