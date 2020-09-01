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
////////////////////projection
void Viewport::Project(Vector3 * pOut, Vector3 & source, Matrix & W, Matrix & V, Matrix & P)
{
	Vector3 position = source;

	Matrix wvp = W * V * P;
	D3DXVec3TransformCoord(pOut, &position, &wvp);

	pOut->x = ((pOut->x + 1.0f) * 0.5f) * width + x;
	pOut->y = ((-pOut->y + 1.0f) * 0.5f) * height + y;
	pOut->z = (pOut->z * (maxDepth - minDepth)) + minDepth;
}
////////////////////unprojection
void Viewport::Unproject(Vector3 * pOut, Vector3 & source, Matrix & W, Matrix & V, Matrix & P)
{
	Vector3 position = source;
	//viewport부터 나와야하잖아.
	//ndc공간 : 화면에 대한 비율
	//화면공간 -> ndc공간

	pOut->x = ((position.x - x) / width) * 2.0f - 1.0f;
	pOut->y = (((position.y - y) / height) * 2.0f - 1.0f) * -1.0f;
	pOut->z = ((position.z - minDepth) / (maxDepth - minDepth));

	Matrix wvp = W * V * P;
	D3DXMatrixInverse(&wvp, NULL, &wvp);
	//변환해 주면 될거잖아.
	//카메라의 방향에서 쏘는 벡터라 지면에서는 카메라가 쏘는게 안보여
	D3DXVec3TransformCoord(pOut, pOut, &wvp);
}
