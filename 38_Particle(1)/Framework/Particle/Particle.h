#pragma once

class Particle : public Renderer
{
public:
	Particle(wstring file);
	~Particle();
	
	void Reset();
	void Add(Vector3& position);

public:
	void Update();

private:
	void MapVertices();
	void Activate();
	void Deactivate();

public:
	void Render();

private:
	void ReadFile(wstring file);

public:
	ParticleData& GetData() { return data; }
	void SetTexture(wstring file); 
private:
	struct VertexParticle
	{
		Vector3 Position;
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

	Texture* map = NULL;
	ID3DX11EffectShaderResourceVariable* sMap;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;


	VertexParticle* vertices = NULL;
	UINT* indices = NULL;

	
	float currentTime = 0.0f;
	float lastAddTime = 0.0f;

	UINT leadCount = 0; //�߰��� ��ü ���� ����
	UINT gpuCount = 0; //GPU�� ���� ���� ����

	UINT activeCount = 0; //�׸� ����
	UINT deactiveCount = 0;
};