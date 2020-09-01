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
{	//마우스 오른쪽 버튼 안누르고 있으면 카메라 움직이지 않는다.
	if (Mouse::Get()->Press(1) == false) return;

	//Vector3 f = Vector3(0, 0, 1);
	//Vector3 u = Vector3(0, 1, 0);
	//Vector3 r = Vector3(1, 0, 0);

	Vector3 f = Forward();
	Vector3 u = Up();
	Vector3 r = Right();

	//Move
	{
		Vector3 P;
		Position(&P);
		//카메라 포지션을 움직인다. (마우스 누른상태로)
		if (Keyboard::Get()->Press('W'))
			P = P + f * move * Time::Delta();
		else if (Keyboard::Get()->Press('S'))
			P = P - f * move * Time::Delta();

		if (Keyboard::Get()->Press('D'))
			P = P + r * move * Time::Delta();
		else if (Keyboard::Get()->Press('A'))
			P = P - r * move * Time::Delta();

		if (Keyboard::Get()->Press('E'))
			P = P + u * move * Time::Delta();
		else if (Keyboard::Get()->Press('Q'))
			P = P - u * move * Time::Delta();

		Position(P);
	}

	//Rotation
	Vector3 R;
	Rotation(&R);

	//마우스 누르면 Z축 은 안움직이고 X축 Y축만 움직인다.
	Vector3 val = Mouse::Get()->GetMoveValue();
	R.x = R.x + val.y * rotation * Time::Delta();
	R.y = R.y + val.x * rotation * Time::Delta();
	R.z = 0.0f;

	Rotation(R);
}

void Freedom::Speed(float move, float rotation)
{
	this->move = move;
	this->rotation = rotation;
}
