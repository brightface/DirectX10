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

	// 마지막에 + x 가 왜 있냐면 Imgui에서 + x 하면 + x만큼 밀어줘야한다.
	pOut->x = ((pOut->x + 1.0f) * 0.5f) * width + x;
	//해상도 1280이면 +x 해서 안보이겠지.
	pOut->y = ((-pOut->y + 1.0f) * 0.5f) * height + y;
	//y는 똑같지 않지 뒤집어야지. 어차피 뒤집은거 가져와야하니까.
	pOut->z = (pOut->z * (maxDepth - minDepth)) + minDepth;
	//비율식을 푼거야. 비율이잖아.
	//float w = width * 0.5f;
	//float h = height * 0.5f;
	//float d = maxDepth  - minDepth;
	//float x = this-> x + w;
	//float y = this-> y + h;

	////위치가 없이 크기로만 만들거야
	//Matrix Vp = Matrix
	//{
	//	+w,0,0,0,
	//	0,-h,0,0,
	//	0,0,+d,0,
	//	x, y, minDepth, 1 //최소 깊이가 작아서 불려지는게 적을거야
	//};

	
}