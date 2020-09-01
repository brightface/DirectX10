#pragma once
//IA 단계니까 vertexbuffer다루겠다.
class D3D11_VertexBuffer final
{
public:
	D3D11_VertexBuffer(class Graphics* graphics);
	~D3D11_VertexBuffer();

	auto GetResource()	const { return buffer; }
	auto GetStride()	const { return stride; }
	auto GetOffset()	const { return offset; }
	auto GetCount()		const { return count; }

	//버퍼 타입을 모르니까 템플릿, 1.색상 2. 텍스처(좌표정보인듯)
	template <typename T>
	void Create(const std::vector<T>& vertices, const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE);
	//디폴트파라미터는 오버로딩을 하지마라. 
	void Clear();

	//id3d11 컴인터페이스, create가 있다.
private:
	ID3D11Device* device	= nullptr;
	ID3D11Buffer* buffer	= nullptr;
	uint stride				= 0; //정점하나의 크기
	uint offset				= 0;
	uint count				= 0;
};

//우리가 사용할 정점의 타입
template<typename T>
inline void D3D11_VertexBuffer::Create(const std::vector<T>& vertices, const D3D11_USAGE & usage)
{
	stride	= sizeof(T);
	count	= static_cast<uint>(vertices.size()); //개수

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	//알트키 누르고 위치바꾸기
	desc.ByteWidth	= stride * count;
	desc.Usage		= usage; //이것에 따라서 cpu접근할수있는지 없는지 하겠다. 또한 접근 가능하다면 cpuFlag 사용해야한다.
	desc.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
	
	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE: desc.CPUAccessFlags = 0; //쓸수없다.												break;
	case D3D11_USAGE_DYNAMIC:	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							break;
	case D3D11_USAGE_STAGING:	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ; //읽기도 가능	break; 
	}

	D3D11_SUBRESOURCE_DATA sub_data;
	ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
	sub_data.pSysMem = vertices.data(); //시작주소 넣어준다.

	auto hr = device->CreateBuffer(&desc, &sub_data, &buffer);
	assert(SUCCEEDED(hr));
}
