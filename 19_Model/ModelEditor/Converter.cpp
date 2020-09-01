#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/BinaryFile.h"

Converter::Converter()
{
	importer = new Assimp::Importer();

}
Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile(wstring file)
{
	this->file = L"../../_Assets/" + file;

	scene = importer->ReadFile
	(
	
		//더블 스트링을 스트링으로 바꾸ㅝ주는거야
		String::ToString(this->file),
		//왼손좌표계, uv계 다 바꾼다.
		aiProcess_ConvertToLeftHanded
		| aiProcess_Triangulate
		| aiProcess_GenUVCoords // 삼각형 형식으로 바뀌는것 uv로 다시 계산 이것ㅇ벗으면 uv못쓴다.
		| aiProcess_GenNormals
		| aiProcess_CalcTangentSpace

	);

	if (scene == NULL) {
		string str = importer->GetErrorString();

		//에러나면 메세지 박스 띄우고
		MessageBoxA(D3D::GetDesc().Handle, str.c_str(), "Assimp Error", MB_OK);
		exit(-1);
	}
}

void Converter::ExportMesh(wstring savePath)
{
	ReadBoneData(scene->mRootNode, -1, -1);

	
	FILE* file;
	fopen_s(&file, "../Meshes.csv", "w");
	//새로 만들고 w 하겠다 파일

	for (asBone* bone : bones)
	{
		string name = bone->Name;
		fprintf(file, "%d,%d, %s\n", bone->index,bone->Parent, bone->Name.c_str());
	}
	
	fprintf(file, "\n");
	for (UINT i = 0; i < meshes.size(); i++)
	{
		fprintf(file, "Mesh Index: : %d\n", i);
		asMesh* mesh = meshes[i];
		for(UINT v=0; v< mesh->Vertices.size(); v++){
			Vector3 p = mesh->Vertices[v].Position;

			fprintf(file, "%f,%f,%f\n", p.x, p.y, p.z);
		}
		fprintf(file, "\n");
	}
	fclose(file); // 파일 닫기
	savePath = L"../../_Model/" + savePath + L".mesh";
}
//본을 그리는데 트리구조로 그리겠다.
void Converter::ReadBoneData(aiNode * node, int index, int parent)
{
	asBone* bone = new asBone();
	bone->index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();
	
	//aiMatrix  4*4
	Matrix transform(node->mTransformation[0]); //연산자 오버라이딩이 되있다.
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent;
	bones.push_back(bone);
	ReadMeshData(node, index);
	//한 노드안에 있는거야 이 노드의 인덱스를 자식에게 넘겨주겠다.
	//이노드가 메시를 안가지고 있는경우가 있다.


	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);


}

void Converter::ReadMeshData(aiNode * node, int index)
{
	//위치만 마킹하려는 본이 있다.
	//HP ID메시가 없어 그릴필요가 ㅇ벗잖아
	
	//메시정보를 저장할 구조체를 만들것이다.

	asMesh* mesh = new asMesh();
	mesh->BoneIndex = index;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];
	
		UINT startVertex = mesh-> Vertices.size();
		UINT startIndex = mesh->Indices.size();

		for (int v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			if (srcMesh->HasTextureCoords(0))
			memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector2));

			mesh->Vertices.push_back(vertex);
		}
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh -> mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++) {
				mesh->Indices.push_back(face.mIndices[k] + startVertex);
			}
		}
		aiMaterial* material = scene->mMaterials[srcMesh-> mMaterialIndex];
		//이름을 넣어주겠다
		asMeshPart* meshPart = new asMeshPart();
		meshPart->MaterialName = material->GetName().C_Str();
		meshPart->StartIndex = startIndex;
		meshPart->StartVertex = startIndex;
		meshPart->VertexCount = srcMesh->mNumVertices;
		meshPart->IndexCount = srcMesh->mNumFaces* srcMesh->mFaces->mNumIndices;

		mesh->MeshParts.push_back(meshPart);
	}// for(i) 
	
	meshes.push_back(mesh);
	
}

void Converter::WriteMeshData(wstring savePath)
{
	Path::CreateFolders(Path::GetDirectoryName(savePath));
	BinaryWriter* w = new BinaryWriter();
	w->Open(savePath);
	//디레토리 이름만 적어야한다.

	//몇개를 써야하는지 안다. 본사이즈 먼저쓴다.
	w->UInt(bones.size());
	for (asBone* bone : bones)
	{
		//문자열을 가변을 만들지 않고 한다.
		w->Int(bone->index);
		w->String(bone->Name);
		w->Int(bone->Parent);
		w->Matrix(bone->Transform);

		SafeDelete(bone);
	}
	w->UInt(meshes.size());
   for (asMesh* meshData : meshes)
   {
      w->String(meshData->Name);
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

