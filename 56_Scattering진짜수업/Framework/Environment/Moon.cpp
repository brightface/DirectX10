#include "Framework.h"
#include "Moon.h"

Moon::Moon(Shader * shader)
	: Renderer(shader)
	, distance(95), glowDistance(90)
{
	moon = new Texture(L"Environment/Moon.png");
	moonGlow = new Texture(L"Environment/MoonGlow.png");

	sMoon = shader->AsSRV("MoonMap");
	sAlpha = shader->AsScalar("MoonAlpha");


	VertexTexture vertices[6];

	vertices[0].Position = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1].Position = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[2].Position = Vector3(+1.0f, -1.0f, 0.0f);
	vertices[3].Position = Vector3(+1.0f, -1.0f, 0.0f);
	vertices[4].Position = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[5].Position = Vector3(+1.0f, +1.0f, 0.0f);

	vertices[0].Uv = Vector2(0, 1);
	vertices[1].Uv = Vector2(0, 0);
	vertices[2].Uv = Vector2(1, 1);
	vertices[3].Uv = Vector2(1, 1);
	vertices[4].Uv = Vector2(0, 0);
	vertices[5].Uv = Vector2(1, 0);

	vertexBuffer = new VertexBuffer(vertices, 6, sizeof(VertexTexture));

	D3DXMatrixOrthoLH(&projection, D3D::Width(), D3D::Height(), 0.1f, 1000);
}

Moon::~Moon()
{

}

void Moon::Update()
{
	Super::Update();
}

void Moon::Render(float theta)
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->AsMatrix("MoonProjection")->SetMatrix(projection);

	sAlpha->SetFloat(GetAlpha(theta));

	//Moon
	{
		Matrix W = GetTransform(theta);
		transform->World(W);

		Super::Render();

		sMoon->SetResource(moon->SRV());
		shader->Draw(0, Pass(), 6);
	}

	//Glow
	{
		Matrix W = GetGlowTransform(theta);
		transform->World(W);

		Super::Render();

		sMoon->SetResource(moonGlow->SRV());
		shader->Draw(0, Pass(), 6);
	}
}

float Moon::GetAlpha(float theta)
{
	if (theta < Math::PI * 0.5f || theta > Math::PI * 1.5f)
		return fabsf(sinf(theta + Math::PI / 2.0f));

	return 0.0f;
}

D3DXMATRIX Moon::GetTransform(float theta)
{
	Vector3 position;
	Context::Get()->GetCamera()->Position(&position);


	Matrix S, R, T, D;
	D3DXMatrixScaling(&S, 4, 4, 1);
	D3DXMatrixRotationYawPitchRoll(&R, Math::PI * 0.5f, theta - (Math::PI * 0.5f), 0);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	Vector3 direction = Context::Get()->Direction();
	D3DXMatrixTranslation
	(
		&D
		, direction.x * distance
		, direction.y * distance
		, direction.z * distance
	);

	return S * R * T * D;


	D3DXMATRIX R;
	Context::Get()->GetCamera()->GetMatrix(&R);
	D3DXMatrixInverse(&R, NULL, &R);

	float x = asin(-R._32);
	float y = atan2(R._31, R._33);
	float z = atan2(R._12, R._22);

	D3DXVECTOR3 rotation(x, y, z);

	D3DXVECTOR3 position;
	position.x = Context::Get()->Direction().x * 50;
	position.y = Context::Get()->Direction().y * 50;
	position.z = Context::Get()->Direction().z * 50;

	D3DXVECTOR3 camPos;
	Context::Get()->GetCamera()->Position(&camPos);

	D3DXMATRIX S, RX, RY, RZ, T1, T2;
	D3DXMatrixScaling(&S, 100, 100, 100);
	D3DXMatrixRotationX(&RX, rotation.x);
	D3DXMatrixRotationY(&RY, rotation.y);
	D3DXMatrixRotationZ(&RZ, rotation.z);
	D3DXMatrixTranslation(&T1, position.x, position.y, position.z);
	D3DXMatrixTranslation(&T2, camPos.x, camPos.y, camPos.z);

	return S * RX * RY * RZ * T1 * T2;
}

D3DXMATRIX Moon::GetGlowTransform(float theta)
{
	D3DXVECTOR3 position;
	Context::Get()->GetCamera()->Position(&position);

	D3DXMATRIX S, R, T, D;
	D3DXMatrixScaling(&S, 12, 12, 1);
	D3DXMatrixRotationYawPitchRoll(&R, Math::PI * 0.5f, theta - (Math::PI * 0.5f), 0);
	D3DXMatrixTranslation(&T, position.z, position.y, position.z);

	D3DXVECTOR3 direction = Context::Get()->Direction();
	D3DXMatrixTranslation
	(
		&D
		, direction.x * glowDistance
		, direction.y * glowDistance
		, direction.z * glowDistance
	);

	return S * R * T * D;
}