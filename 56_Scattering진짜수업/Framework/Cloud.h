#pragma once
//컬립 노이즈 / 랜덤값으로 팍팍튀는게 아니라 픽셀마다 튀는게 아니라 픽셀이 융화되면서 모양높이맵이랑 같다.
//문명. 마크. 마크 지면을 그렇게 생성할떄  정복 승리 문화 승리 3개월이 타임머신, 
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