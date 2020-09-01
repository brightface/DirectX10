#pragma once

enum ShaderScope : uint
{
	ShaderScope_Unknown = 0U, //U,u ���̸� ����ε� ��Ʈ���̴�.
	ShaderScope_VS		= 1U << 0, // 000000010
	ShaderScope_PS		= 1U << 1, 
};
//��Ʈ����ũ�� ����ؼ� ���� ���̴��� ����߳� �ϰڴ�.
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
	//���ҽ��� ������ �ֳ�?

	auto GetShaderBlob() const { return shader_blob; }

	auto GetName() const { return name; }
	void SetName(const std::string& name) { this->name = name; }
	//�ܺο��� ������ϵ��� GETTER�Լ��� �ۼ��Ѵ�
	auto GetPath() const { return path; }
	auto GetShaderScope() const { return shader_scope; }
	auto GetCompilationState() const { return compilation_state; }

	auto GetEntryPoint() const -> const char*;
	auto GetTargetProfile() const -> const char*;
	auto GetShaderModel() const -> const char*;
	//�������� �Ϸ� ����� �ȉ���� �˷��ش�.
	bool IsCompiled() const { return compilation_state == CompilationState::Succeeded; }

	void Create(const ShaderScope& scope, std::string& shader);
	void Clear();

private:
	auto Compile(const ShaderScope& scope, std::string& shader) -> void*;

private:
	ID3D11Device* device				= nullptr;
	ID3DBlob* shader_blob				= nullptr;
	void* resource						= nullptr;
	//�������� �����Ͱ� �����ִ� ���̵� ������.

	std::string name					= ""; //TODO : 
	std::string path					= "";
	ShaderScope shader_scope			= ShaderScope_Unknown;
	CompilationState compilation_state	= CompilationState::Unknown;
};