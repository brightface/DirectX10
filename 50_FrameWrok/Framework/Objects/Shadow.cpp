#include "Framework.h"
#include "Shadow.h"

Shadow::Shadow(Shader * shader, Vector3 & position, float radius, UINT width, UINT height)
	: shader(shader), position(position), radius(radius), width(width), height(height)
{
	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport((float)width, (float)height);

	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	sBuffer = shader->AsConstantBuffer("CB_Shadow");
	sShadowMap = shader->AsSRV("ShadowMap");

	desc.MapSize = Vector2(width, height);


	//Create Sampler State
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.MaxAnisotropy = 1;
		desc.MaxLOD = FLT_MAX;

		Check(D3D::GetDevice()->CreateSamplerState(&desc, &pcfSampler));
		sPcfSampler = shader->AsSampler("ShadowSampler");
	}
}

Shadow::~Shadow()
{
	SafeDelete(renderTarget);
	SafeDelete(depthStencil);
	SafeDelete(viewport);

	SafeDelete(buffer);

	SafeRelease(pcfSampler);
}

void Shadow::PreRender()
{
	ImGui::InputInt("Quality", (int *)&desc.Quality);
	desc.Quality %= 3;
	//조명방향 context에 있잖아.
	ImGui::SliderFloat3("Light Direction", Context::Get()->Direction(), -1, +1);
	ImGui::SliderFloat("Bias", &desc.Bias, -0.0001f, +0.001f, "%.4f");

	//얘로 셋팅한거잖아.
	renderTarget->PreRender(depthStencil);
	viewport->RSSetViewport();

	Vector3 up = Vector3(0, 1, 0);
	Vector3 direction = Context::Get()->Direction();
	Vector3 position = direction * radius * 2.0f;
	//바라볼 방향 direction 위치를 자를거기때문에 이렇게 구한거야.

	D3DXMatrixLookAtLH(&desc.View, &position, &this->position, &up);
	
	Vector3 origin; //포지션 중앙으로 한것. view 공간으로 먼저 할것이다.
	D3DVec3TransformCoord(&origin, &this->position, &desc.View); 
	//perspective 사용안하고 orhtho가 나아. 그리고 길게하는게 하는게 하나.
	float f = origin.x - radius; //왼쪾이 나오겠지.
	float b = origin.y - radius;
	float n = origin.z - raidus;

	float r = origin.x + radius; //왼쪾이 나오겠지.
	float t = origin.y + radius;
	float f = origin.z + raidus;

	D3DXMatrixOrithLH(&desc.Projection, r - f, t-b, n, f); //조명공간을 그린거야.
	
														   
														   //원래를 위치줘야돼.
	alcViewProjection();

	//계산한 이후에 쉐이더한테 보내줘야곘지?
	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	sShadowMap->SetResource(depthStencil->SRV());
	sPcfSampler->SetSampler(0, pcfSampler);
}

void Shadow::CalcViewProjection()
{
	Vector3 up = Vector3(0, 1, 0);
	Vector3 direction = Context::Get()->Direction();
	Vector3 position = direction * radius * -2.0f;

	D3DXMatrixLookAtLH(&desc.View, &position, &this->position, &up);


	Vector3 origin;
	D3DXVec3TransformCoord(&origin, &this->position, &desc.View);

	float l = origin.x - radius;
	float b = origin.y - radius;
	float n = origin.z - radius;

	float r = origin.x + radius;
	float t = origin.y + radius;
	float f = origin.z + radius;

	D3DXMatrixOrthoLH(&desc.Projection, r - l, t - b, n, f);
}
