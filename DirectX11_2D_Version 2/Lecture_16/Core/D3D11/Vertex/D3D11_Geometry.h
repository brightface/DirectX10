#pragma once
#include "stdafx.h"

template <class T>
class D3D11_Geometry
{
public:
	D3D11_Geometry() = default;
	virtual ~D3D11_Geometry();

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
	auto GetIndices()			const -> const std::vector<uint>& { return indices; }
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
#include "D3D11_Geometry.inl"