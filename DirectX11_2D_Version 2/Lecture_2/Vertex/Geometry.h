#pragma once
#include "stdafx.h"

//<템플릿>
//CPP에다가 만들수 없다. 메모리에 할당전까지 타입을 알수없다.
//헤더파일은 컴파일러에 물리지 않는다.  구현부에 만들수 없다.
//헤더에서만 정의해야만해

//ini : 컴파일러가 바로 빌드
//inline 함수
//헤더에서 정의가 되서 거의 인라인함수만 정의해놓은 파일
template <class T>
class Geometry
{
public:
	Geometry() = default;
	virtual ~Geometry();

	//============================================================
	// [Vertex]
	//============================================================
	auto GetVertexCount()		const { return static_cast<uint>(vertices.size()); }
	auto GetVertexByteWidth()	const { return GetVertexCount() * sizeof(T); }
	auto GetVertexPointer()		const { return vertices.data(); }
	auto GetVertices()			const -> const std::vector<T>& { return vertices; }
	auto GetVertices(const uint& offset, const uint& count) -> const std::vector<T>;

	void AddVertex(const T& vertex);
	void AddVerticse(const std::vector<T>& vertices);
	void SetVerticse(const std::vector<T>& vertices);

	//============================================================
	// [Index]
	//============================================================
	auto GetIndexCount()		const { return static_cast<uint>(indices.size()); }
	auto GetIndexByteWidth()	const { return GetIndexCount() * sizeof(uint); }
	auto GetIndexPointer()		const { return indices.data(); }
	auto GetIndices()			const -> const std::vector<T>& { return indices; }
	auto GetIndices(const uint& offset, const uint& count) -> const std::vector<uint>;

	void AddIndex(const uint& index);
	void AddIndices(const std::vector<uint>& indices);
	void SetIndices(const std::vector<uint>& indices);

	//============================================================
	// [MICS]
	//============================================================
	void Clear();

protected:
	std::vector<T> vertices;
	std::vector<uint> indices;
};
#include "Geometry.inl"