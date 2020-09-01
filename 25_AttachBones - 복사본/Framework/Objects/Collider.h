#pragma once

struct ColliderObject
{
	//모델을 정확히 다 연산해서 충돌체를 심지 않는다.
	//개략적인 부분을 보여줘
	//충돌체를 심는것은 디자이너가 해.
	//우리는 툴을 만들어줘
	//우리는 충돌했느냐를 계산
	
	//충돌체 - 그냥 라인그리는것.
	class Transform* Init = NULL;
	class Transform* Transform = NULL;
	class Collider* Collider = NULL;
};

class Collider
{
public:
	//칼날 부분만 충돌체 씌운것이다.
	Collider(Transform* transform, Transform* init = NULL);
	~Collider();

	void Update();
	void Render(Color color = Color(0, 1, 0, 1));

private:
	//기준이 될애 될 본
	Transform* init = NULL;
	//받아서 2개를 곱할거야.
	Transform* transform;
	//선으로 그릴거야.
	Vector3 lines[8];
};