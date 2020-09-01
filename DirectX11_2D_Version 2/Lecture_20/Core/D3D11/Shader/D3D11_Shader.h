#pragma once

enum ShaderScope : uint
{
	ShaderScope_Unknown = 0U,
	ShaderScope_VS		= 1U << 0, // 000000010
	ShaderScope_PS		= 1U << 1, 
};

//000000001
//000000010
//000000100 //or
//000000101

enum class CompilationState : uint
{
	Unknown,
	Compiling,
	Succeeded,
	Failed,
};

class D3D11_Shader final : public Object
{
public:
	D3D11_Shader(class Graphics* graphics);
	~D3D11_Shader();

	auto GetResource() const { return resource; }
	bool HasResource() const { return resource != nullptr; }
	auto GetShaderBlob() const { return shader_blob; }

	auto GetName() const { return name; }
	void SetName(const std::string& name) { this->name = name; }

	auto GetPath() const { return path; }
	auto GetShaderScope() const { return shader_scope; }
	auto GetCompilationState() const { return compilation_state; }

	auto GetEntryPoint() const -> const char*;
	auto GetTargetProfile() const -> const char*;
	auto GetShaderModel() const -> const char*;

	bool IsCompiled() const { return compilation_state == CompilationState::Succeeded; }

	void Create(const ShaderScope& scope, const std::string& shader);
	void Clear();

private:
	auto Compile(const ShaderScope& scope, const std::string& shader) -> void*;

private:
	ID3D11Device* device				= nullptr;
	ID3DBlob* shader_blob				= nullptr;
	void* resource						= nullptr;

	std::string name					= ""; //TODO : 
	std::string path					= "";
	ShaderScope shader_scope			= ShaderScope_Unknown;
	CompilationState compilation_state	= CompilationState::Unknown;
};