#include "Framework.h"
#include "Perspective.h"

Perspective::Perspective(float width, float height, float zn, float zf, float fov)
	: Projection(width, height, zn, zf, fov)
{
	Set(width, height, zn, zf, fov);
}

Perspective::~Perspective()
{
}
//근거리 원거리 면이다.
void Perspective::Set(float width, float height, float zn, float zf, float fov)
{
	Super::Set(width, height, zn, zf, fov);

	aspect = width / height; //종행비로 만든다.
	D3DXMatrixPerspectiveFovLH(&matrix, fov, aspect, zn, zf);
}
