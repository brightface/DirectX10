#pragma once

class Reflection
{
public:
	Reflection(Shader* shader, Transform* transform, float width = 0, float height = 0);
	~Reflection();

	void Update();

	void PreRender();
	void Render();

private:
	Shader* shader;

	//반사판에대한 트랜스폼
	Transform* transform;
	Fixity* camera;
	//메인카메라 뒤집은 위치에대한 뷰야.

	float width, height;
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;

	ID3DX11EffectShaderResourceVariable* sReflectionSRV;
	ID3DX11EffectMatrixVariable* sReflectionView;
};