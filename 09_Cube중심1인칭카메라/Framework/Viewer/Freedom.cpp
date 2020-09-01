#include "Framework.h"
#include "Freedom.h"

Freedom::Freedom()
	: Camera()
{

}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	if (Mouse::Get()->Press(1) == false) return;

	//Vector3 f = Vector3(0, 0, 1);
	//Vector3 u = Vector3(0, 1, 0);
	//Vector3 r = Vector3(1, 0, 0);

	Vector3 f = Forward();
	Vector3 u = Up();
	Vector3 r = Right();


	//Rotation


	Vector3 val = Mouse::Get()->GetMoveValue();
	rotationV.x = rotationV.x + val.y * rotation * Time::Delta();
	rotationV.y = rotationV.y + val.x * rotation * Time::Delta();
	rotationV.z = 0.0f;

	

	if (Mouse::Get()->Press(1) == true)
	{
		Rotation(rotationV);
		//Position(*CameraPosition - Forward());
	}
	
}

void Freedom::Speed(float move, float rotation)
{
	this->move = move;
	this->rotation = rotation;
}
