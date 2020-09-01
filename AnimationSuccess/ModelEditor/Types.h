#pragma once
#include "stdafx.h"
//본
struct asBone
{
	//본 인덱스
	int Index;
	string Name;

	//부모의 본
	int Parent;
	//본의 상대 위치<본의 간격>
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

struct asBlendWeight
{
	Vector4 Indices = Vector4(0, 0, 0, 0);
	Vector4 Weights = Vector4(0, 0, 0, 0);

	void Set(UINT index, UINT boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
			case 0: Indices.x = i; Weights.x = w; break;
			case 1: Indices.y = i; Weights.y = w; break;
			case 2: Indices.z = i; Weights.z = w; break;
			case 3: Indices.w = i; Weights.w = w; break;
		}
	}
};
//이걸 만들거야. 본의 weights 있지. 
struct asBoneWeights
{
private:
	typedef pair<int, float> Pair;
	vector<Pair> BoneWeights;

public:
	void AddWeights(UINT boneIndex, float boneWeights)
	{
		if (boneWeights <= 0.0f) return;

		bool bAdd = false;
		vector<Pair>::iterator it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			if (boneWeights > it->second)
			{
				BoneWeights.insert(it, Pair(boneIndex, boneWeights));
				bAdd = true;
				//추가되었다. 본번호에 가중치를 본웨이츠에 추가되었다. 가장 작은 가중치가 앞에들어가거든.
				break;
			}

			it++;
		} // while(it)
		//해당본번호가 없다면 추가 될수가 없어서 맨뒤에다가 추가를 해줘
		if (bAdd == false)
			BoneWeights.push_back(Pair(boneIndex, boneWeights));
	}

	void GetBlendWeights(asBlendWeight& blendWeights)
	{
		for (UINT i = 0; i < BoneWeights.size(); i++)
		{
			if (i >= 4) return;

			blendWeights.Set(i, BoneWeights[i].first, BoneWeights[i].second);
		}
	}

	void Normalize()
	{	
		float totalWeight = 0.0f;

		int i = 0;
		vector<Pair>::iterator it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			if (i < 4) //4개까지만 쓸거야 그 외는 지울거야. 4개까지 총합이야.
			{
				totalWeight += it->second;
				i++; it++;
			}
			else
				it = BoneWeights.erase(it);
		}
		//총합 나누기. 크기로 나누기
		float scale = 1.0f / totalWeight;

		it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			it->second *= scale;
			it++;
		}//항상 가중치의 총합은 1인거야. 하나가있든 두개가 있든. 공식이야 . 디자인 프로그래머가 그렇게 만들어. 
	}
};

struct asKeyframeData
{
	float Frame;

	Vector3 Scale;
	Quaternion Rotation;
	Vector3 Translation;
};

struct asKeyframe
{
	string BoneName;
	vector<asKeyframeData> Transforms;
};

struct asClip
{
	string Name;

	UINT FrameCount;
	float FrameRate;
	float Duration;

	vector<asKeyframe *> Keyframes;
};

//aniNode의 원본 키프레임 저장
struct asClipNode
{
	aiString Name;
	vector<asKeyframeData> Keyframe;
};