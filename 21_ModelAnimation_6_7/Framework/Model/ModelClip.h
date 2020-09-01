#pragma once
//모델 애니메이션 저장할 것들
//하나하나의 동작이다.
struct ModelKeyframeData
{
	float Frame;

	Vector3 Scale;
	Quaternion Rotation;
	Vector3 Translation;
};

struct ModelKeyframe
{
	wstring BoneName;
	vector<ModelKeyframeData> Transforms;
};

class ModelClip
{
public:
	friend class Model;

private:
	ModelClip();
	~ModelClip();

public:
	float Duration() { return duration; }
	float FrameRate() { return frameRate; }
	UINT FrameCount() { return frameCount; }

	ModelKeyframe* Keyframe(wstring name);

private:
	wstring name;

	float duration; //길이
	float frameRate; //시간비율
	UINT frameCount;

	//맵이 아니라 언오더드 맵이 훨 빨라, 해쉬로 동작. 데이터 작으면 맵이 빨라
	//커지면 어노드가 빨라. O(1)의 시간복잡도, 
	//unordered map, map 이거 차이가 있다. 찾아보기 
	unordered_map<wstring, ModelKeyframe *> keyframeMap;
};