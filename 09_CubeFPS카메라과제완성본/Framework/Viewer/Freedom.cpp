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
	
	
	Vector3 f = Forward();
	Vector3 u = Up();
	Vector3 r = Right();

	//Move
	{
		if (Mouse::Get()->Press(1) == true)
		{
			Vector3 val = Mouse::Get()->GetMoveValue();
			rotationV.x = rotationV.x + val.y * rotation * Time::Delta();
			rotationV.y = rotationV.y + val.x * rotation * Time::Delta();
			rotationV.z = 0.0f;

			Rotation(rotationV);
			Position(*AimPosition - Forward()* distance);


		}
		if (Keyboard::Get()->Press('E'))
	
			distance += 1.0f * Time::Delta();

		
		if (Keyboard::Get()->Press('Q') && distance > 1.0f)
			distance -= 1.0f *Time::Delta();
	}


		/*if (Keyboard::Get()->Press('W'))
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

		Position(P);*/
	


	
}

void Freedom::Speed(float move, float rotation)
{
	this->move = move;
	this->rotation = rotation;
}
