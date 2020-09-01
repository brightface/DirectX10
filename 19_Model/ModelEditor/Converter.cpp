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
	
		//���� ��Ʈ���� ��Ʈ������ �ٲ٤��ִ°ž�
		String::ToString(this->file),
		//�޼���ǥ��, uv�� �� �ٲ۴�.
		aiProcess_ConvertToLeftHanded
		| aiProcess_Triangulate
		| aiProcess_GenUVCoords // �ﰢ�� �������� �ٲ�°� uv�� �ٽ� ��� �̰ͤ������� uv������.
		| aiProcess_GenNormals
		| aiProcess_CalcTangentSpace

	);

	if (scene == NULL) {
		string str = importer->GetErrorString();

		//�������� �޼��� �ڽ� ����
		MessageBoxA(D3D::GetDesc().Handle, str.c_str(), "Assimp Error", MB_OK);
		exit(-1);
	}
}

void Converter::ExportMesh(wstring savePath)
{
	ReadBoneData(scene->mRootNode, -1, -1);

	
	FILE* file;
	fopen_s(&file, "../Meshes.csv", "w");
	//���� ����� w �ϰڴ� ����

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
	fclose(file); // ���� �ݱ�
	savePath = L"../../_Model/" + savePath + L".mesh";
}
//���� �׸��µ� Ʈ�������� �׸��ڴ�.
void Converter::ReadBoneData(aiNode * node, int index, int parent)
{
	asBone* bone = new asBone();
	bone->index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();
	
	//aiMatrix  4*4
	Matrix transform(node->mTransformation[0]); //������ �������̵��� ���ִ�.
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent;
	bones.push_back(bone);
	ReadMeshData(node, index);
	//�� ���ȿ� �ִ°ž� �� ����� �ε����� �ڽĿ��� �Ѱ��ְڴ�.
	//�̳�尡 �޽ø� �Ȱ����� �ִ°�찡 �ִ�.


	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);


}

void Converter::ReadMeshData(aiNode * node, int index)
{
	//��ġ�� ��ŷ�Ϸ��� ���� �ִ�.
	//HP ID�޽ð� ���� �׸��ʿ䰡 �����ݾ�
	
	//�޽������� ������ ����ü�� ������̴�.

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
		//�̸��� �־��ְڴ�
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
	//���丮 �̸��� ������Ѵ�.

	//��� ����ϴ��� �ȴ�. �������� ��������.
	w->UInt(bones.size());
	for (asBone* bone : bones)
	{
		//���ڿ��� ������ ������ �ʰ� �Ѵ�.
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

