#pragma once
//�� �ִϸ��̼� ������ �͵�
//�ϳ��ϳ��� �����̴�.
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

	float duration; //����
	float frameRate; //�ð�����
	UINT frameCount;

	//���� �ƴ϶� ������� ���� �� ����, �ؽ��� ����. ������ ������ ���� ����
	//Ŀ���� ���尡 ����. O(1)�� �ð����⵵, 
	//unordered map, map �̰� ���̰� �ִ�. ã�ƺ��� 
	unordered_map<wstring, ModelKeyframe *> keyframeMap;
};