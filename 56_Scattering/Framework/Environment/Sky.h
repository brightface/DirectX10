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
	//�ǽð� �ٲ� �ð�
	bool bRealTime = false;
	//�׸��ڰ� �����θ� ����ž� 90~180
	float timeFactor = 1.0f; //�ǽð� �ð� �󸶳� ������ �Ұų�
	float theta = 0.0f; // ��Ÿ���� ���ؼ� �׸��� ������ 

	class Scattering* scattering; //��� ���
	ConstantBuffer* scatterBuffer; //�Ķ��� ������ �迭 �ϴ� �ػ��� - ���� ��ο� �ð�. �ذ� �߰ų� ���ų� �����迭 - ���� ������ ª���� �̼� ����. ���ڰ� ª��. ���� ������ ��� Ǫ����. �ٴ� Ǫ���� = ���� ������ ,����ġ �����غ����� ������. 
	//���ϸ� ���, �̻��, 
	ID3DX11EffectConstantBuffer* sScatterBuffer;
	//�ݱ����� �ٸ��ž� . �ݱ��� ���̾�. ���� 2���� �����Ű� 
	
	ID3DX11EffectShaderResourceVariable* sRayleighMap;
	ID3DX11EffectShaderResourceVariable* sMieMap;


	class Dome* dome;
	class Moon* moon; //�� : ���� 
	//������ �̿��߰ŵ� 
	//���� 3D ��ǥ 2D�� �ٲ� HP ��ǥ -> ũ�� ��ȭ�� ����. perspective ������� ª������ �׷�.
	//���ʿ� ������. otrth�� �׷��� �Ÿ��� ���� ũ�� ��ȭ�� �������ݾ�.
	//�ݱ��� �׷�.
	//

	//������ �߾�. 2D �׷��� ��ġ UI ó�� �ٸ����. 2D��ġ���� �̹����� �׷���. ������ �ҋ� 
	//class Cloud* cloud; -attdive ���ĺ���. 
	ConstantBuffer* cloudBuffer;
	ID3DX11EffectConstantBuffer* sCloudBuffer;
};