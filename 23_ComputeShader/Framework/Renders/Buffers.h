#pragma once

class VertexBuffer
{
public:
	VertexBuffer(void* data, UINT count, UINT stride, UINT slot = 0, bool bCpuWrite = false, bool bGpuWrite = false);
	~VertexBuffer();

	UINT Count() { return count; }
	UINT Stride() { return stride; }
	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT count;
	UINT stride;
	UINT slot;

	bool bCpuWrite;
	bool bGpuWrite;
};

///////////////////////////////////////////////////////////////////////////////

class IndexBuffer
{
public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	UINT Count() { return count; }
	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT count;
};

///////////////////////////////////////////////////////////////////////////////

class ConstantBuffer
{
public:
	ConstantBuffer(void* data, UINT dataSize);
	~ConstantBuffer();

	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;
};

///////////////////////////////////////////////////////////////////////////////

class CsResource
{
public:
	CsResource();
	virtual ~CsResource();

protected:
	virtual void CreateInput() {}

	virtual void CreateSRV() {}

	virtual void CreateOutput() {}
	virtual void CreateUAV() {}

	virtual void CreateResult() {}

	void CreateBuffer();

	//쉐이더로부터 정보주는것

public:
	//쉐이더로부터 넘겨주는것.
	ID3D11ShaderResourceView* SRV() { return srv; }

	//쉐이더로 리턴을 받을것
	ID3D11UnorderedAccessView* UAV() { return uav; }

protected:
	ID3D11Resource* input = NULL;
	ID3D11ShaderResourceView* srv = NULL;

	
	ID3D11Resource* output = NULL;
	ID3D11UnorderedAccessView* uav = NULL;

	//스테이트형 : 한번에 접근가능하다. 스테이징은 읽고 쓰기가 가능하다.
	ID3D11Resource* result = NULL;
};

///////////////////////////////////////////////////////////////////////////////

class RawBuffer : public CsResource
{
public:
	//입력된 데이트의 크기, 출력될 데이터의 크기
	RawBuffer(void* inputData, UINT inputByte, UINT outputByte);
	~RawBuffer();

private:
	void CreateInput() override;
	void CreateSRV() override;

	void CreateOutput() override;
	void CreateUAV() override;

	void CreateResult() override;

public:
	void CopyToInput(void* data);
	void CopyFromOutput(void* data);

private:
	void* inputData;
	UINT inputByte;
	UINT outputByte;
};

///////////////////////////////////////////////////////////////////////////////