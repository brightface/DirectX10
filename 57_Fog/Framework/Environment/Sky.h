#pragma once

class Sky
{
public:
	Sky(Shader* shader);
	~Sky();

	void Pass(UINT scatteringPass, UINT domePass, UINT moonPass, UINT cloudPass);
	void Pass(UINT pass);

	void Update();
	void PreRender();
	void Render();
	void PostRender();

	void RealTime(bool val, float theta, float timeFactor = 1.0f);

private:
	struct ScatterDesc
	{
		Vector3 WaveLength = Vector3(0.65f, 0.57f, 0.475f);
		float Padding;

		Vector3 InvWaveLength;
		int SampleCount = 8;

		Vector3 WaveLengthMie;
		float Padding2;
	} scatterDesc;

	struct CloudDesc
	{
		float Tiles = 1.5f;
		float Cover = 0.005f;
		float Sharpness = 0.405f;
		float Speed = 0.05f;
	} cloudDesc;

private:
	Shader* shader;

	//실시간 바꿈 시간
	bool bRealTime = false;
	//그림자가 옆으로만 비출거야 90~180
	float timeFactor = 1.0f; //실시간 시간 얼마나 빠르게 할거냐
	float theta = 0.0f;// 세타각에 의해서 그림자 방향을

	class Scattering* scattering;//대기 산란
	ConstantBuffer* scatterBuffer;
	ID3DX11EffectConstantBuffer* sScatterBuffer;

	//레일리산란 미산란
	ID3DX11EffectShaderResourceVariable* sRayleighMap;
	ID3DX11EffectShaderResourceVariable* sMieMap;


	class Dome* dome;
	class Moon* moon;
	//빌보드 이용했거든 
	//달의 3D 좌표 2D로 바꿔 HP 좌표 -> 크기 변화가 없어. perspective 길어지고 짧아지고 그래.
	//애초에 귀찮다. otrth로 그래픽 거리에 따라 크기 변화가 없어지잖아.
	//반구라 그래.
	//

	//판으로 했어. 2D 그려질 위치 UI 처럼 바르라고. 2D위치에다 이미지를 그려라. 랜더링 할떄 
	//class Cloud* cloud; -attdive 알파블랜딩. 

	//class Cloud* cloud;
	ConstantBuffer* cloudBuffer;
	ID3DX11EffectConstantBuffer* sCloudBuffer;
};