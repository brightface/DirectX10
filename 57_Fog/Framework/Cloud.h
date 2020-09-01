#pragma once
//�ø� ������ / ���������� ����Ƣ�°� �ƴ϶� �ȼ����� Ƣ�°� �ƴ϶� �ȼ��� ��ȭ�Ǹ鼭 �����̸��̶� ����.
//����. ��ũ. ��ũ ������ �׷��� �����ҋ�  ���� �¸� ��ȭ �¸� 3������ Ÿ�Ӹӽ�, 
class Cloud : public Renderer
{
public:
	Cloud(Shader* shader);
	~Cloud();

	void Update();
	void Render();
	void PostRender();

private:
	void CreateTexture();

private:
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;
	ID3DX11EffectShaderResourceVariable* sSRV;

	Render2D* render2D;
};