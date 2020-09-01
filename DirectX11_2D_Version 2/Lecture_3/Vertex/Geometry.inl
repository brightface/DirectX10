#include "Geometry.h"

template<class T>
inline Geometry<T>::~Geometry()
{
	Clear();
}

template<class T>
inline auto Geometry<T>::GetVertices(const uint & offset, const uint & count) -> const std::vector<T>
{
	std::vector<T> sub_vertices;

	auto first	= vertices.begin() + offset;
	auto last	= first + count;

	sub_vertices.assign(first, last);

	return sub_vertices;
}

template<class T>
inline void Geometry<T>::AddVertex(const T & vertex)
{	//Push_back 가능 그러나 성능이 높다.
	vertices.emplace_back(vertex);
}

template<class T>
inline void Geometry<T>::AddVerticse(const std::vector<T>& vertices)
{
	this->vertices.insert
	(
		this->vertices.end(),
		vertices.begin(),
		vertices.end()
	);
}

template<class T>
inline void Geometry<T>::SetVerticse(const std::vector<T>& vertices)
{

	this->vertices.clear();//공간은 남아있는데 데이터만 지운다.
	this->vertices.shrink_to_fit();//내부 메모리 수축(공간다시 날려준다)
	this->vertices = vertices;//assign 함수를 활용할수도 있다.
}

template<class T>
inline auto Geometry<T>::GetIndices(const uint & offset, const uint & count) -> const std::vector<uint>
{
	std::vector<T> sub_indices;

	auto first	= indices.begin() + offset;
	auto last	= first + count;

	sub_indices.assign(first, last);

	return sub_indices;
}

template<class T>
inline void Geometry<T>::AddIndex(const uint & index)
{
	indices.emplace_back(index);
}

template<class T>
inline void Geometry<T>::AddIndices(const std::vector<uint>& indices)
{
	this->indices.insert
	(
		this->indices.end(),
		indices.begin(),
		indices.end()
	);
}

template<class T>
inline void Geometry<T>::SetIndices(const std::vector<uint>& indices)
{
	this->indices.clear();
	this->indices.shrink_to_fit();
	this->indices = indices;
}

template<class T>
inline void Geometry<T>::Clear()
{
	vertices.clear();
	vertices.shrink_to_fit();

	indices.clear();
	indices.shrink_to_fit();
}
