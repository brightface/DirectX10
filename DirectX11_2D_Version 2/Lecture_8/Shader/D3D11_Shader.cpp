#include "stdafx.h"
#include "D3D11_Shader.h"

D3D11_Shader::D3D11_Shader(Graphics * graphics)
{
	device = graphics->GetDevice();
}

D3D11_Shader::~D3D11_Shader()
{
	Clear();
}
//컨트롤 u ,컨트롤 시프트u 대문자 소문자
//정적 메모리를 하나 만들겠다. 해당하는 쉐이더 정보를 리턴을 해줄것이다.
auto D3D11_Shader::GetEntryPoint() const -> const char *
{
	static const char* entry_point_empty	= nullptr;
	static const char* entry_point_vs		= "VS";
	static const char* entry_point_ps		= "PS";

	switch (shader_scope)
	{
	case ShaderScope_VS: return entry_point_vs;
	case ShaderScope_PS: return entry_point_ps;
	}

	return entry_point_empty;
}
//vs ps 버전확인하는것
auto D3D11_Shader::GetTargetProfile() const -> const char *
{	
	static const char* target_profile_empty = nullptr;
	//옵션에 전처리기에서 정의한것 쓰겠다.
#if defined(GRAPHICS_API_VERSION_D3D11)
	static const char* target_profile_vs = "vs_5_0";
	static const char* target_profile_ps = "ps_5_0";
#elif defined(GRAPHICS_API_VERSION_D3D12)
	static const char* target_profile_vs = "vs_6_0";
	static const char* target_profile_ps = "ps_6_0";
#endif

	switch (shader_scope)
	{
	case ShaderScope_VS: return target_profile_vs;
	case ShaderScope_PS: return target_profile_ps;
	}

	return target_profile_empty;
}

//앞에 버전이 빠져있는것 - 모델번호를 반환해주는 함수
auto D3D11_Shader::GetShaderModel() const -> const char *
{
#if defined(GRAPHICS_API_VERSION_D3D11)
	static const char* shader_model = "5_0";
#elif defined(GRAPHICS_API_VERSION_D3D12)
	static const char* shader_model = "6_0";
#endif

	return shader_model;
}

void D3D11_Shader::Create(const ShaderScope & scope, const std::string & shader)
{
	shader_scope = scope;

	compilation_state = CompilationState::Compiling;
	resource = Compile(scope, shader);
	compilation_state = HasResource() ? CompilationState::Succeeded : CompilationState::Failed;
}

void D3D11_Shader::Clear()
{
	switch (shader_scope)
	{
	case ShaderScope_VS:
	{
		auto shader = static_cast<ID3D11VertexShader*>(resource);
		SAFE_RELEASE(shader);
		break;
	}
	case ShaderScope_PS:
	{
		auto shader = static_cast<ID3D11PixelShader*>(resource);
		SAFE_RELEASE(shader);
		break;
	}
	}

	SAFE_RELEASE(shader_blob);

	name				= ""; 
	path				= "";
	shader_scope		= ShaderScope_Unknown;
	compilation_state	= CompilationState::Unknown;
}

auto D3D11_Shader::Compile(const ShaderScope & scope, const std::string & shader) -> void *
{	//컴파일 에러 어디서 났는지 몇줄에서 났는지 알수있게한다.
	ID3DBlob* error = nullptr;
	//D3DX11CompileFromFileA를 사용해서 컴파일했지 hlsl 파일
	HRESULT result = D3DX11CompileFromFileA
	(
		shader.c_str(),
		nullptr,
		nullptr,
		GetEntryPoint(),
		GetTargetProfile(),
		0,
		0,
		nullptr,
		&shader_blob, //에러가 있을때만 아래의 에러에 들어오고 없을때는 쉐이더 블롭으로 간다.
		&error,
		nullptr
	); //

	if (error)
	{
		//blob은 바이너리 형태이기때문에 문자열로 바꿔주면 된다.
		auto str = static_cast<const char*>(error->GetBufferPointer());
		MessageBoxA(nullptr, str, "Shader Error!!!", MB_OK);
	}
	//성공했다면 흉 나온다.
	assert(SUCCEEDED(result));

	//이제 에러 없으니 쉐이더 만들면 된다.Blob에 널이 할당되어있다.
	
	void* shader_resource = nullptr;
	if (shader_blob)
	{
		switch (shader_scope)
		{
		case ShaderScope_VS:
		{
			result = device->CreateVertexShader
			(
				shader_blob->GetBufferPointer(), 
				shader_blob->GetBufferSize(), 
				nullptr, 
				reinterpret_cast<ID3D11VertexShader**>(&resource)
			); //static_cast 는 int -> float //reinterpret_cast 는 struct Test -> struct K; 모르는것 바꿀때 
			//int* ->void* ->int* // 3개의 주소가 같다 static은 그러나 reinter는 중간의 void의 주소가 다르다.
			
			assert(SUCCEEDED(result));

			break;
		}
		case ShaderScope_PS:
		{

			break;
		}
		}
	}

	return shader_resource;
}
