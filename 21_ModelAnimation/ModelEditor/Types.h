#pragma once
#include "stdafx.h"

struct asBone
{
	int Index; //본의 번호
	string Name;	

	int Parent; //부모본 
	Matrix Transform; //메시가 참조할 월드, 메시가 얘를 가져다 그리는거야
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