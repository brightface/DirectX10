#include "stdafx.h"
#include "D3D11_Shader.h"

//쉐이더 총 6개 픽셀 버택스 널 컴퓨터 도메인 1개 등
D3D11_Shader::D3D11_Shader(Graphics * graphics)
{
	device = graphics->GetDevice();
}

D3D11_Shader::~D3D11_Shader()
{
	Clear();
}

auto D3D11_Shader::GetEntryPoint() const -> const char *
{
	return nullptr;
}

auto D3D11_Shader::GetTargetProfile() const -> const char *
{
	return nullptr;
}

auto D3D11_Shader::GetShaderModel() const -> const char *
{
	return nullptr;
}

void D3D11_Shader::Create(const ShaderScope & scope, std::string & shader)
{
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

auto D3D11_Shader::Compile(const ShaderScope & scope, std::string & shader) -> void *
{
	return nullptr;
}
