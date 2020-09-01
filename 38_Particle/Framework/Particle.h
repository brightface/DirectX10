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
		Vector3 Position; //처음 시작 위치
		Vector2 Corner; //(-1 ~ +1)
		Vector3 Velocity;
		Vector4 Random; //x:주기, y:크기, z:회전, w:색상
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
	float lastAddTime = 0.0f; //입자가 마지막으로 추가된 시간

	UINT leadCount = 0; //추가된 전체 입자 갯수
	UINT gpuCount = 0; //GPU로 보낼 입자 갯수

	UINT activeCount = 0; //그릴 갯수
	UINT deactiveCount = 0; //비활성화
};