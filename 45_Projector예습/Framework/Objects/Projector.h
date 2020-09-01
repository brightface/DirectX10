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
		
		//ndc 공간을 uv로 바꿀거야 이 공간을 view projection 매트릭스로 넘겨서
		Matrix View;
		Matrix Projection;
	} desc;

private:
	
	Shader* shader;
	//해당도이자 이미지의 크기
	float width, height;
	//고정카메라 오버라이딩으로 막아버린거야. 비추는곳 막아버림
	Fixity* camera;
	//일부러 perspective로 안만드는 이유가 있어
	Projection* projection;

	Texture* map;
	ID3DX11EffectShaderResourceVariable* sMap;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
};