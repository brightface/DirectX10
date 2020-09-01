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
	//ī�޶� x�� �����´�.
	R.x *= -1.0f; //x������ ��������Ѵ�. 
	camera->Rotation(R); //�ǽ�Ƽī�޶�

	Vector3 position;
	transform->Position(&position);

	//ī�޶��� ��������?
	T.y = (position.y * 2.0f) - T.y;
	camera->Position(T);
}

void Reflection::PreRender()
{
	renderTarget->PreRender(depthStencil);
	viewport->RSSetViewport();

	Matrix view;//ī�޶��� ��� �ѱ��.
	camera->GetMatrix(&view);
	sReflectionView->SetMatrix(view);
}

void Reflection::Render()
{	//ī�޶��� �ݻ�Ȱ� �߶��ٰž� ���� �۷ι��ΰ�
	sReflectionSRV->SetResource(renderTarget->SRV());
}
