#include "Framework.h"
#include "MeshQuad.h"

MeshQuad::MeshQuad(Shader * shader)
	: Mesh(shader)
{

}

MeshQuad::~MeshQuad()
{
}
//create�� ���۸���°�
void MeshQuad::Create()
{
	float w = 0.5f;
	float h = 0.5f;

	vector<MeshVertex> v;
	//�»�ܺ��� ���� (1.0)
	//�츮�� �ٶ󺸴� ���� -1
	//x��ġ y��ġ z=0, uv (1,0) normal�� �ǵ� ���������� ����� ������ָ� -1;
	v.push_back(MeshVertex(-w, -h, 0, 1, 0, 0, 0, -1));
	v.push_back(MeshVertex(-w, +h, 0, 0, 0, 0, 0, -1));
	v.push_back(MeshVertex(+w, -h, 0, 1, 1, 0, 0, -1));
	v.push_back(MeshVertex(+w, +h, 0, 1, 0, 0, 0, -1));


	vertices = new MeshVertex[v.size()];
	vertexCount = v.size();

	//�迭�� ���ͷ� �����ϴ¹�
	//v.insert(v.begin(), vertices, vertices + 4);
	
	//���͸� �迭�� �����ϴ¹� 
	//�迭�� ũ�Ⱑ ���༭ ���, ��� �ȶ߰� �ϴ¹� - �迭�� ���ͷ����ͷ� :�����ּ�, �迭�ǰ���
	//�̰� ���� ������
	copy(v.begin(), v.end(), stdext::checked_array_iterator<MeshVertex *>(vertices, vertexCount));
	//������ �ƴ϶� ���� ���̴�. memcpy�� ����´�.
	//���͸� �迭�� �����ϴ¹�

	indexCount = 6;
	indices = new UINT[indexCount]{ 0, 1, 2, 2, 1, 3 };
}