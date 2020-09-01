#pragma once
#include "Camera.h"

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();
	
	void SetCameraLook(Vector3* cubePosition)
	{
		AimPosition = cubePosition;
		
	}
	void Update() override;

	void Speed(float move, float rotation = 2.0f);

private:
	Vector3* AimPosition;
	Vector3 rotationV = Vector3(1.0f,0.0f,0.0f); //초기화안하면 에러
	float move = 20.0f;
	float rotation = 2.0f;
	float distance = 5.0f;
};