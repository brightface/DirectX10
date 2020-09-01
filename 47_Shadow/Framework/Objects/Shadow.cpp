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

	//�ش��ϴ� desc �ְ���. 
	//Create Sampler State
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC)); //�̰ɷ� �� ������. 
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR; //���� �ִ� ���� . ���� ��� �Ұų�? 
		//desc.Filter = D3D11_FiL //�ι��� ��int  comparision �����ž�.
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; //uv�� ����� Ŭ������ �������ž�.
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP; 
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP; 
		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL; //�۰ų� ����
		desc.MaxAnisotropy = 1;
		//msa�� �����ִ�. �����߿� ���ä�����
		desc.MaxLOD = FLT_MAX; //�ִ� ���� ����ع����� ��

		Check(D3D::GetDevice()->CreateSamplerState(&desc, &pcfSampler));
		sPcfSampler = shader->AsSampler("ShadowPcfSampler");
	}
}

Shadow::~Shadow()
{
	SafeDelete(renderTarget);
	SafeDelete(depthStencil);
	SafeDelete(viewport);

	SafeDelete(buffer);

	SafeRelease(pcfSampler); //�̰� ������߰���
}

void Shadow::PreRender()
{
	ImGui::InputInt("Quality", (int *)&desc.Quality);
	desc.Quality %= 2;
	
	//������� context�� ���ݾ�.
	ImGui::SliderFloat3("Light Direction", Context::Get()->Direction(), -1, +1);
	ImGui::SliderFloat("Bias", &desc.Bias, -0.0001f, +0.001f, "%.4f"); //���� 0.0001 

	//��� �����Ѱ��ݾ�.
	renderTarget->PreRender(depthStencil);
	viewport->RSSetViewport();

	Vector3 up = Vector3(0, 1, 0);
	Vector3 direction = Context::Get()->Direction();
	Vector3 position = direction * radius * 2.0f;
	//�ٶ� ���� direction ��ġ�� �ڸ��ű⶧���� �̷��� ���Ѱž�.

	D3DXMatrixLookAtLH(&desc.View, &position, &this->position, &up);
	
	Vector3 origin; //������ �߾����� �Ѱ�. view �������� ���� �Ұ��̴�.
	D3DVec3TransformCoord(&origin, &this->position, &desc.View); 
	//perspective �����ϰ� orhtho�� ����. �׸��� ����ϴ°� �ϴ°� �ϳ�.
	float f = origin.x - radius; //�ަU�� ��������.
	float b = origin.y - radius;
	float n = origin.z - raidus;

	float r = origin.x + radius; //�ަU�� ��������.
	float t = origin.y + radius;
	float f = origin.z + raidus;

	D3DXMatrixOrithLH(&desc.Projection, r - f, t-b, n, f); //��������� �׸��ž�.
	
														   
														   //������ ��ġ��ߵ�.
	alcViewProjection();

	//����� ���Ŀ� ���̴����� ������߁���?
	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	sShadowMap->SetResource(depthStencil->SRV());
	sPcfSampler->SetSampler(0, pcfSampler); //hsls s0 �ٿ��ڳ� 0 �̰ž�.
	//�̰� �ϳ� �о���. 
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
