#pragma once

struct ColliderObject
{
	//���� ��Ȯ�� �� �����ؼ� �浹ü�� ���� �ʴ´�.
	//�������� �κ��� ������
	//�浹ü�� �ɴ°��� �����̳ʰ� ��.
	//�츮�� ���� �������
	//�츮�� �浹�ߴ��ĸ� ���
	
	//�浹ü - �׳� ���α׸��°�.
	class Transform* Init = NULL;
	class Transform* Transform = NULL;
	class Collider* Collider = NULL;
};

class Collider
{
public:
	//Į�� �κи� �浹ü ������̴�.
	Collider(Transform* transform, Transform* init = NULL);
	~Collider();

	void Update();
	void Render(Color color = Color(0, 1, 0, 1));

private:
	//������ �ɾ� �� ��
	Transform* init = NULL;
	//�޾Ƽ� 2���� ���Ұž�.
	Transform* transform;
	//������ �׸��ž�.
	Vector3 lines[8];
};