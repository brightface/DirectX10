#pragma once

class MeshQuad : public Mesh
{
public:
	MeshQuad(Shader* shader);
	
	//�̰� �ʿ����.
	~MeshQuad(); //�̰� �ʿ���� ������ virtual �پ������� �θ�Ҹ��� ��ȯ�ǰŵ�

private:
	//�� ���� ����� ������̴�.
	void Create() override;
};