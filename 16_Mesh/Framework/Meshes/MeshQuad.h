#pragma once

class MeshQuad : public Mesh
{
public:
	MeshQuad(Shader* shader);
	
	//이거 필요없다.
	~MeshQuad(); //이거 필요없다 어차피 virtual 붙어있으니 부모소멸자 소환되거든

private:
	//각 정점 모양을 만들것이다.
	void Create() override;
};