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

auto D3D11_Shader::GetTargetProfile() const -> const char *
{
	static const char* target_profile_empty = nullptr;

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
{
	ID3DBlob* error = nullptr;

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
		&shader_blob,
		&error,
		nullptr
	);

	if (error)
	{
		auto str = static_cast<const char*>(error->GetBufferPointer());
		MessageBoxA(nullptr, str, "Shader Error!!!", MB_OK);
	}

	assert(SUCCEEDED(result));

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
				reinterpret_cast<ID3D11VertexShader**>(&shader_resource)
			);
			assert(SUCCEEDED(result));
			break;
		}
		case ShaderScope_PS:
		{
			result = device->CreatePixelShader
			(
				shader_blob->GetBufferPointer(),
				shader_blob->GetBufferSize(),
				nullptr,
				reinterpret_cast<ID3D11PixelShader**>(&shader_resource)
			);
			assert(SUCCEEDED(result));
			break;
		}
		}
	}

	return shader_resource;
}
