#pragma once
#include "stdafx.h"

struct asBone 
{
	int index;
	string Name;
	//번호
	
	int Parent;
	//메시가 참조할 월드
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