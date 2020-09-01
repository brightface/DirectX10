#include "Framework.h"
#include "MeshQuad.h"

MeshQuad::MeshQuad(Shader * shader)
	: Mesh(shader)
{

}

MeshQuad::~MeshQuad()
{
}
//create는 버퍼만드는것
void MeshQuad::Create()
{
	float w = 0.5f;
	float h = 0.5f;

	vector<MeshVertex> v;
	//좌상단부터 시작 (1.0)
	//우리를 바라보는 방향 -1
	//x위치 y위치 z=0, uv (1,0) normal은 판뒤 전방쪽으로 노멀을 만들어주면 -1;
	v.push_back(MeshVertex(-w, -h, 0, 1, 0, 0, 0, -1));
	v.push_back(MeshVertex(-w, +h, 0, 0, 0, 0, 0, -1));
	v.push_back(MeshVertex(+w, -h, 0, 1, 1, 0, 0, -1));
	v.push_back(MeshVertex(+w, +h, 0, 1, 0, 0, 0, -1));


	vertices = new MeshVertex[v.size()];
	vertexCount = v.size();

	//배열을 벡터로 복사하는법
	//v.insert(v.begin(), vertices, vertices + 4);
	
	//벡터를 배열로 복사하는법 
	//배열의 크기가 안줘서 경고, 경고 안뜨게 하는법 - 배열을 이터레이터로 :시작주소, 배열의개수
	//이게 제일 안전해
	copy(v.begin(), v.end(), stdext::checked_array_iterator<MeshVertex *>(vertices, vertexCount));
	//대입이 아니라 복사 말이다. memcpy도 경고나온다.
	//벡터를 배열로 복사하는법

	indexCount = 6;
	indices = new UINT[indexCount]{ 0, 1, 2, 2, 1, 3 };
}