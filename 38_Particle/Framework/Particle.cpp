#include "Framework.h"
#include "Particle.h"
#include "Utilites/Xml.h"

Particle::Particle(wstring file)
{

}

void Particle::Reset()
{
	currentTime = 0.0f;
	lastAddTime = Time::Get()->Running();
	leadCount = gpuCount = activeCount = deactiveCount = 0;
	
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	vertices = new VertexParticle[date.MaxParticles * 4];
	for (UINT i = 0; i < data.MaxParticle; i++) {
		vertices[i * 4 + 0].Corner = Vector2(-1, -1);
		vertices[i * 4 + 1].Corner = Vector2(-1, 1);
		vertices[i * 4 + 2].Corner = Vector2(1, -1);
		vertices[i * 4 + 3].Corner = Vector2(1, 1);

	}

	indices[i * 6 + 0] = i * 4 + 0;
	indices[i * 6 + 1] = i * 4 + 1;
	indices[i * 6 + 2] = i * 4 + 2;
	indices[i * 6 + 3] = i * 4 + 2;
	indices[i * 6 + 4] = i * 4 + 1;
	indices[i * 6 + 5] = i * 4 + 3;
}
vertexBuffer = new VertexBuffer(vertices, data.MaxParticles * 4, sizeof(VertexParticle), 0, true);
indexBuffer = new IndexBuffer(indices, data.MaxParticles * 6);

}

void Particle::Add(Vector3 & position)
{
	if (Time::Get()->Running() - lastAddTime < 60 / 1000.0f)
		return;

	lastAddTime = Time::Get()->Running();

	UINT next = leadCount + 1;
	if (next >= data.MaxParticles)
	{
		next = 0;
	}

	if (next == deactiveCount) return; //꽉찬상황이라 리턴을 시키겠다.

	Vector3 velocity = Vector3(1, 1, 1);
	velocity *= data.StartVelocity;
	//속도를 랜덤으로 선택한다. 
	float horizontalVelocity = Math::Lerp(data.MinHorizontalVelocity, data.MaxHorizontalVelocity, Math::Random(0.0f, 1.0f));
	float horizontalAngle = Math::PI * 2.0f * Math::Random(0.0f, 1.0f);

	velocity.x += horizontalVelocity * cos(horizontalAngle);
	velocity.y += horizontalVelocity * sin(horizontalAngle);
	velocity.z += Math::Lerp<float>(data.MinVerticalVelocity, data.MaxVerticalVelocity, Math::Random(0, 0f, 0.0f, 0.0f));

}
Particle::~Particle()
{
}