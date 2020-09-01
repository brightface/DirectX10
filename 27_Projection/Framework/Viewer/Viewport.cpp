#include "Framework.h"
#include "Viewport.h"

Viewport::Viewport(UINT width, UINT height, float x, float y, float minDepth, float maxDepth)
{
	this->width = width < 1 ? (UINT)D3D::Width() : width;
	this->height = height < 1 ? (UINT)D3D::Height() : height;

	Set(width, height, x, y, minDepth, maxDepth);
}

Viewport::~Viewport()
{
	
}

void Viewport::RSSetViewport()
{
	D3D::GetDC()->RSSetViewports(1, &viewport);
}

void Viewport::Set(UINT width, UINT height, float x, float y, float minDepth, float maxDepth)
{
	viewport.TopLeftX = this->x = x;
	viewport.TopLeftY = this->y = y;
	viewport.MinDepth = this->minDepth = minDepth;
	viewport.MaxDepth = this->maxDepth = maxDepth;

	this->width = width;
	viewport.Width = (float)this->width;

	this->height = height;
	viewport.Height = (float)this->height;

	RSSetViewport();
}

void Viewport::Project(Vector3 * pOut, Vector3 & source, Matrix & W, Matrix & V, Matrix & P)
{
	Vector3 position = source;

	Matrix wvp = W * V * P;
	D3DXVec3TransformCoord(pOut, &position, &wvp);

	// �������� + x �� �� �ֳĸ� Imgui���� + x �ϸ� + x��ŭ �о�����Ѵ�.
	pOut->x = ((pOut->x + 1.0f) * 0.5f) * width + x;
	//�ػ� 1280�̸� +x �ؼ� �Ⱥ��̰���.
	pOut->y = ((-pOut->y + 1.0f) * 0.5f) * height + y;
	//y�� �Ȱ��� ���� ���������. ������ �������� �����;��ϴϱ�.
	pOut->z = (pOut->z * (maxDepth - minDepth)) + minDepth;
	//�������� Ǭ�ž�. �������ݾ�.
	//float w = width * 0.5f;
	//float h = height * 0.5f;
	//float d = maxDepth  - minDepth;
	//float x = this-> x + w;
	//float y = this-> y + h;

	////��ġ�� ���� ũ��θ� ����ž�
	//Matrix Vp = Matrix
	//{
	//	+w,0,0,0,
	//	0,-h,0,0,
	//	0,0,+d,0,
	//	x, y, minDepth, 1 //�ּ� ���̰� �۾Ƽ� �ҷ����°� �����ž�
	//};

	
}