#include "Framework.h"
#include "Reflection.h"

Reflection::Reflection(Shader * shader, Transform * transform, float width, float height)
	: shader(shader), transform(transform)
{
	this->width = width > 0.0f ? width : D3D::Width();
	this->height = height > 0.0f ? height : D3D::Height();

	camera = new Fixity();
	renderTarget = new RenderTarget(this->width, this->height);
	depthStencil = new DepthStencil(this->width, this->height);
	viewport = new Viewport(this->width, this->height);

	sReflectionSRV = shader->AsSRV("ReflectionMap");
	sReflectionView = shader->AsMatrix("ReflectionView");
}

Reflection::~Reflection()
{
	SafeDelete(camera);
	SafeDelete(renderTarget);
	SafeDelete(depthStencil);
	SafeDelete(viewport);
}

void Reflection::Update()
{
	Vector3 R, T;
	Context::Get()->GetCamera()->Rotation(&R);
	Context::Get()->GetCamera()->Position(&T);
	//카메라를 x로 뒤집는다.
	R.x *= -1.0f; //x축으로 뒤집어야한다. 
	camera->Rotation(R); //피시티카메라

	Vector3 position;
	transform->Position(&position);

	//카메라의 포지션은?
	T.y = (position.y * 2.0f) - T.y;
	camera->Position(T);
}

void Reflection::PreRender()
{
	renderTarget->PreRender(depthStencil);
	viewport->RSSetViewport();

	Matrix view;//카메라의 뷰로 넘긴다.
	camera->GetMatrix(&view);
	sReflectionView->SetMatrix(view);
}

void Reflection::Render()
{	//카메라의 반사된걸 발라줄거야 그후 글로벌로가
	sReflectionSRV->SetResource(renderTarget->SRV());
}
