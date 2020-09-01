#pragma once

enum ShaderScope : uint
{
	ShaderScope_Unknown = 0U, //U,u 붙이면 언사인드 인트형이다.
	ShaderScope_VS		= 1U << 0, // 000000010
	ShaderScope_PS		= 1U << 1, 
};
//비트마스크를 사용해서 무슨 쉐이더를 사용했나 하겠다.
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

class D3D11_Shader final
{
public:
	D3D11_Shader(class Graphics* graphics);
	~D3D11_Shader();

	auto GetResource() const { return resource; }
	bool HasResource() const { return resource != nullptr; }
	//리소스를 가지고 있냐?

	auto GetShaderBlob() const { return shader_blob; }

	auto GetName() const { return name; }
	void SetName(const std::string& name) { this->name = name; }
	//외부에서 변경못하도록 GETTER함수만 작성한다
	auto GetPath() const { return path; }
	auto GetShaderScope() const { return shader_scope; }
	auto GetCompilationState() const { return compilation_state; }

	auto GetEntryPoint() const -> const char*;
	auto GetTargetProfile() const -> const char*;
	auto GetShaderModel() const -> const char*;
	//컴파일이 완료 됬는지 안됬는지 알려준다.
	bool IsCompiled() const { return compilation_state == CompilationState::Succeeded; }

	void Create(const ShaderScope& scope, std::string& shader);
	void Clear();

private:
	auto Compile(const ShaderScope& scope, std::string& shader) -> void*;

private:
	ID3D11Device* device				= nullptr;
	ID3DBlob* shader_blob				= nullptr;
	void* resource						= nullptr;
	//여러개의 데이터가 들어갈수있는 보이드 포인터.

	std::string name					= ""; //TODO : 
	std::string path					= "";
	ShaderScope shader_scope			= ShaderScope_Unknown;
	CompilationState compilation_state	= CompilationState::Unknown;
};