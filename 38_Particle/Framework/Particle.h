#pragma once

class Particle : public Renderer
{
public:
	Particle(wstring file);
	~Particle();
public:
	void Reset();
private:
	void ReadFile(wstring file);

private:
	struct VertexParticle
	{
		Vector3 Position; //ó�� ���� ��ġ
		Vector2 Corner; //(-1 ~ +1)
		Vector3 Velocity;
		Vector4 Random; //x:�ֱ�, y:ũ��, z:ȸ��, w:����
		float Time;
	};
private:
	struct Desc
	{
		Color MinColor;
		Color MaxColor;

		Vector3 Gravity;
		float EndVelocity;

		Vector2 StartSize;
		Vector2 EndSize;

		Vector2 RotateSpeed;
		float ReadyTime;
		float ReadyRandomTime;

		float CurrentTime;
		float Padding[3];
	} desc;
	
private:
	ParticleData data;
	
	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	
	Texture* map = NULL;
	ID3DX11EffectShaderResourceVariable* sMap;

	VertexParticle* vertices = NULL;
	UINT* indices = NULL;
	
	float currentTime = 0.0f; //
	float lastAddTime = 0.0f; //���ڰ� ���������� �߰��� �ð�

	UINT leadCount = 0; //�߰��� ��ü ���� ����
	UINT gpuCount = 0; //GPU�� ���� ���� ����

	UINT activeCount = 0; //�׸� ����
	UINT deactiveCount = 0; //��Ȱ��ȭ
};