#pragma once

class Projector
{
public:
	Projector(Shader* shader, wstring mapFile, UINT width, UINT height);
	~Projector();

	void Update();
	void Render();

	Camera* GetCamera() { return (Camera *)camera; }

private:
	struct Desc
	{
		Color Color = D3DXCOLOR(1, 1, 1, 1);
		
		//ndc ������ uv�� �ٲܰž� �� ������ view projection ��Ʈ������ �Ѱܼ�
		Matrix View;
		Matrix Projection;
	} desc;

private:
	
	Shader* shader;
	//�ش絵���� �̹����� ũ��
	float width, height;
	//����ī�޶� �������̵����� ���ƹ����ž�. ���ߴ°� ���ƹ���
	Fixity* camera;
	//�Ϻη� perspective�� �ȸ���� ������ �־�
	Projection* projection;

	Texture* map;
	ID3DX11EffectShaderResourceVariable* sMap;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
};