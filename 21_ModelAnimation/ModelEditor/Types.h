#pragma once
#include "stdafx.h"

struct asBone
{
	int Index; //���� ��ȣ
	string Name;	

	int Parent; //�θ� 
	Matrix Transform; //�޽ð� ������ ����, �޽ð� �긦 ������ �׸��°ž�
};

struct asMeshPart
{
	string MaterialName;

	UINT StartVertex;
	UINT VertexCount;

	UINT StartIndex;
	UINT IndexCount;
};

struct asMesh
{
	int BoneIndex;

	vector<Model::ModelVertex> Vertices;
	vector<UINT> Indices;

	vector<asMeshPart *> MeshParts;
};

struct asMaterial
{
	string Name;

	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;

	string DiffuseFile;
	string SpecularFile;
	string NormalFile;
};