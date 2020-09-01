#pragma once

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update() = 0;

public:
	void Position(float x, float y, float z);
	void Position(Vector3& vec);
	void Position(Vector3* vec);

	void Rotation(float x, float y, float z);
	void Rotation(Vector3& vec);
	void Rotation(Vector3* vec);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(Vector3& vec);
	void RotationDegree(Vector3* vec);
public:
	//void SetCameraPositionByCube(Vector3* cubePosition)
	//{	//값만 복사된다. 안된다. 이유는 동적으로 포지션이 변하니까 안되는가
	//	this->position = *cubePosition;
	//};
	
	
	void GetMatrix(Matrix* matrix);

	Vector3 Forward() { return forward; }
	Vector3 Up() { return up; }
	Vector3 Right() { return right; }

protected:
	virtual void Rotation();
	virtual void Move();

protected:
	void View();

private:
	
	Vector3 position = Vector3(0, 0, 0);
	
	Vector3 rotation = Vector3(0, 0, 0);

	Vector3 forward = Vector3(0, 0, 1);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 right = Vector3(1, 0, 0);

	Matrix matRotation; //회전 행렬
	Matrix matView; //뷰 행렬
};