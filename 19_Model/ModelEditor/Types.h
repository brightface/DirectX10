#pragma once
#include "stdafx.h"

struct asBone 
{
	int index;
	string Name;
	//��ȣ
	
	int Parent;
	//�޽ð� ������ ����
	Matrix Transform;
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
	string Name;
	int BoneIndex;

	vector<Model::ModelVertex> Vertices;
	vector<UINT> Indices;

	vector<asMeshPart *> MeshParts;
};