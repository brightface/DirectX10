#pragma once
#include "stdafx.h"

template <class T>
class Geometry
{
public:
	Geometry() = default;
	virtual ~Geometry();

	//============================================================
	// [Vertex]
	//============================================================
	
	//정석 
	//const uint GetVertexCount() {}
	//=
	auto GetVertexCount()		const { return static_cast<uint>(vertices.size()); }
	auto GetVertexByteWidth()	const { return GetVertexCount() * sizeof(T); }
	//data 함수는 배열의 시작주소를 얻을수 있다.
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
	// [MICS] 기타 miscellaneous
	//============================================================
	void Clear();

protected:
	std::vector<T> vertices;
	std::vector<uint> indices;
};
#include "Geometry.inl"