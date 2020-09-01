#include "Framework.h"
#include "Sky.h"
#include "Scattering.h"
#include "Dome.h"
#include "Moon.h"
#include "Cloud.h"

Sky::Sky(Shader * shader)
	: shader(shader)
{
	//hdr기법 태양이 떠오르는 기법 - 찾아놔
	//여러장을 찍어. 합성하는거야. 선명하는거야.
	//자동차를 같은 인덱스 다이나믹 큐브맵으로 랜더패스가 2맵 자동ㅇ차 그자체도 인스턴싱 화 되 어있다.
	//preRender 5  
	//Render 5 
	//실제는 게임에서는 랜더링 1번. 그래서 총 5번
	//원패스도 하나의 인스턴싱으로 간주. 5개를 한번에 그리기 10개를 한번에 그린다. 
	//쉐이더의 상당한 기법 랜더링 파이프라인라인의 한번의 기법이 들어가겠지. 엔진제작하냐.

	//스케터링 만들기 위한 식들 넣어놓은거야.
	//hdr 기법 -게임에많이 써
	scatterDesc.InvWaveLength.x = 1.0f / powf(scatterDesc.WaveLength.x, 4.0f);
	scatterDesc.InvWaveLength.y = 1.0f / powf(scatterDesc.WaveLength.y, 4.0f);
	scatterDesc.InvWaveLength.z = 1.0f / powf(scatterDesc.WaveLength.z, 4.0f);

	scatterDesc.WaveLengthMie.x = powf(scatterDesc.WaveLength.x, -0.84f);
	scatterDesc.WaveLengthMie.y = powf(scatterDesc.WaveLength.y, -0.84f);
	scatterDesc.WaveLengthMie.z = powf(scatterDesc.WaveLength.z, -0.84f);

	scattering = new Scattering(shader);
	scatterBuffer = new ConstantBuffer(&scatterDesc, sizeof(ScatterDesc));
	sScatterBuffer = shader->AsConstantBuffer("CB_Scattering");


	dome = new Dome(shader, Vector3(0, 16, 0), Vector3(80, 80, 80));
	moon = new Moon(shader);

	//cloud = new Cloud(shader);
	cloudBuffer = new ConstantBuffer(&cloudDesc, sizeof(CloudDesc));
	sCloudBuffer = shader->AsConstantBuffer("CB_Cloud");

	//스케터링 mrt 2개 나온다. 2개 섞을 거야. 산란 - 2개 섞을거야
	sRayleighMap = shader->AsSRV("RayleighMap");
	sMieMap = shader->AsSRV("MieMap");
}

Sky::~Sky()
{
	SafeDelete(scattering);
	SafeDelete(scatterBuffer);

	SafeDelete(dome);
	SafeDelete(moon);

	//SafeDelete(cloud);
	SafeDelete(cloudBuffer);
}

void Sky::Pass(UINT scatteringPass, UINT domePass, UINT moonPass, UINT cloudPass)
{
	scattering->Pass(scatteringPass);
	dome->Pass(domePass);
	moon->Pass(moonPass);
	cloud->Pass(cloudPass);
}

void Sky::Pass(UINT pass)
{
	scattering->Pass(pass++);
	dome->Pass(pass++);
	moon->Pass(pass++);
	cloud->Pass(Pass++);
}

void Sky::Update()
{
	if (bRealTime == true)
	{
		theta += Time::Delta() * timeFactor;

		if (theta > Math::PI)
			theta -= Math::PI * 2.0f;


		float x = sinf(theta);
		float y = cosf(theta);
		//다이렉션에다 넣는거 보이지?
		Context::Get()->Direction() = Vector3(x, y, 0.0f);
	}
	else
	{
		ImGui::SliderFloat("Theta", &theta, -Math::PI, Math::PI);

		float x = sinf(theta);
		float y = cosf(theta);

		Context::Get()->Direction() = Vector3(x, y, 0.0f);
	}

	//반구는 카메라를 따라다닐거야
	Vector3 domePosition;
	Context::Get()->GetCamera()->Position(&domePosition);
	dome->GetTransform()->Position(domePosition);
	cloud->GetTransform()->Position(domePosition);


	scattering->Update(); //mrt로 딸거야
	dome->Update();
	moon->Update();
	cloud->Update();
}

void Sky::PreRender()
{
	scatterBuffer->Render();
	sScatterBuffer->SetConstantBuffer(scatterBuffer->Buffer());

	scattering->PreRender();
}

void Sky::Render()
{
	//Dome
	{
		sRayleighMap->SetResource(scattering->RayleighRTV()->SRV());
		sMieMap->SetResource(scattering->MieRTV()->SRV());

		dome->Render();
	}//스케터링 결과 주는거잖아.

	//Moon
	{
		moon->Render(theta);
	}

	//Cloud
	{
		//cloudBuffer->Render();
		//sCloudBuffer->SetConstantBuffer(cloudBuffer->Buffer());

		//cloud->Render();
	}
}

void Sky::PostRender()
{
	scattering->PostRender();
	//cloud->PostRender();
}

void Sky::RealTime(bool val, float theta, float timeFactor)
{
	bRealTime = val;

	this->theta = theta;
	this->timeFactor = timeFactor;
}
