#include "D3D11_Geometry.h"

template<class T>
inline D3D11_Geometry<T>::~D3D11_Geometry()
{
	Clear();
}

template<class T>
inline auto D3D11_Geometry<T>::GetVertices(const uint & offset, const uint & count) -> const std::vector<T>
{
	std::vector<T> sub_vertices;

	auto first	= vertices.begin() + offset;
	auto last	= first + count;

	sub_vertices.assign(first, last);

	return sub_vertices;
}

template<class T>
inline void D3D11_Geometry<T>::AddVertex(const T & vertex)
{
	vertices.emplace_back(vertex);
}

template<class T>
inline void D3D11_Geometry<T>::AddVerticse(const std::vector<T>& vertices)
{
	this->vertices.insert
	(
		this->vertices.end(),
		vertices.begin(),
		vertices.end()
	);
}

template<class T>
inline void D3D11_Geometry<T>::SetVerticse(const std::vector<T>& vertices)
{
	this->vertices.clear();
	this->vertices.shrink_to_fit();
	this->vertices = vertices;
}

template<class T>
inline auto D3D11_Geometry<T>::GetIndices(const uint & offset, const uint & count) -> const std::vector<uint>
{
	std::vector<T> sub_indices;

	auto first	= indices.begin() + offset;
	auto last	= first + count;

	sub_indices.assign(first, last);

	return sub_indices;
}

template<class T>
inline void D3D11_Geometry<T>::AddIndex(const uint & index)
{
	indices.emplace_back(index);
}

template<class T>
inline void D3D11_Geometry<T>::AddIndices(const std::vector<uint>& indices)
{
	this->indices.insert
	(
		this->indices.end(),
		indices.begin(),
		indices.end()
	);
}

template<class T>
inline void D3D11_Geometry<T>::SetIndices(const std::vector<uint>& indices)
{
	this->indices.clear();
	this->indices.shrink_to_fit();
	this->indices = indices;
}

template<class T>
inline void D3D11_Geometry<T>::Clear()
{
	vertices.clear();
	vertices.shrink_to_fit();

	indices.clear();
	indices.shrink_to_fit();
}
