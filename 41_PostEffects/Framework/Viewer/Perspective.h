#pragma once

class Perspective : public Projection
{
public:
	//화면 비율을 결정해준다.  fov : 수직 시야각 field of view ,종행비율 , 가로 세로에 대한 비율 
	Perspective(float width, float height, float zn = 0.1f, float zf = 1000.0f, float fov = (float)D3DX_PI * 0.25f);
	~Perspective();

	void Set(float width, float height, float zn = 0.1f, float zf = 1000.0f, float fov = (float)D3DX_PI * 0.25f) override;

private:
	float aspect;
};