#include "stdafx.h"
#include "MeshDemo.h"

void MeshDemo::Initialize()
{
	shader = new Shader(L"30_Animation.fxo");

	planeMaterial = new Material(shader);//���̴� �ϳ� �־��ְ�
	planeMaterial->DiffuseMap("White.png"); //ȭ��Ʈ �ϳ� �־��ְ�
	//�޽÷����� �ٲ��� �޽� �÷����� �ƴ϶�
	plane = new MeshRender(shader, new MeshPlane());
	plane->AddTransform()->Scale(10, 1, 2); //�ѹ��ϸ� �߰��ϸ� �߰��� ������
	plane->UpdateTransforms();//�ѹ��� ��ġ �ȹٲ� plane�� ���⼭ �ѹ��� �����ָ� ��

	//�ڽ� ���׸��� ��������
	boxMaterial = new Material(shader);
	boxMaterial->DiffuseMap(L"Box.png");

	box = new MeshRender(shader, new MeshCube());
	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = box->AddTransform();

		transform->Scale(0.25f, 0.25f, 0.25f);
		transform->Position(Vector3(x, 0.25f * 0.5f, 0.0f));
		//transform->RotationDegree(0, Math::Random(-180, 180), 0);
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
	}
	box->UpdateTransforms();//���Ѵ����� �ѹ��� �ϸ��
}

void MeshDemo::Update()
{
	plane->Update();
	box->Update();
}

void MeshDemo::Render()
{
	planeMaterial->Render();//���׸����� ���� ���ߵȴ�.
	plane->Render();

	boxMaterial->Render();
	box->Render();
}