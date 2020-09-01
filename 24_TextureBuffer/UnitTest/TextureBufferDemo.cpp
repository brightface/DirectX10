#include "stdafx.h"
#include "TextureBufferDemo.h"
#include "TextureBufferDemo.h"

void TextureBufferDemo::Initialize()
{
	ExecRawBuffer();

	//compute shader를 동작시킬 쉐이더 하나 있어야지

	Shader* shader = new Shader(L"24_Texture.fx");

	shader->AsSRV("input")->SetResource(textureBuffer->SRV());
	shader->AsUAV("Output")->SetUnroderedAccesView(textureBuffer->UAV());

	UINT width = textureBuffer->Width();
	UINT height = textureBuffer0 > Height();
	UINT arraySize = textureBuffer->ArraySize();

	float x = ((float)width / 32.0f) < 1.0f ? 1.0f : ((float)width / 32.0f);
	float y = ((float)height / 32.0f) < 1.0f ? 1.0f : ((float)height / 32.0f);

	shader->Dispatch(0, 0);
	//render2D0-> SRV 에 연결될것이다.
		//

}

void TextureBufferDemo::ExecRawBuffer()
{
	Shader* shader = new Shader(L"23_RawBuffer.fx");

	UINT count = 2 * 10 * 8 * 3;

	struct Output
	{
		UINT GroupId[3];
		UINT GroupThreadId[3];
		UINT DispatchThreadId[3];
		UINT GroupIndex;
		float RetValue;
	};

	struct Input
	{
		float Value = 0.0f;
	};
	Input* input = new Input[count];

	for (int i = 0; i < count; i++)
		input[i].Value = Math::Random(0.0f, 10000.0f);
	

	RawBuffer* rawBuffer = new RawBuffer(input, sizeof(Input) * count, sizeof(Output) * count);

	shader->AsSRV("Input")->SetResource(rawBuffer->SRV());
	shader->AsUAV("Output")->SetUnorderedAccessView(rawBuffer->UAV());
	shader->Dispatch(0, 0, 2, 1, 1);


	Output* output = new Output[count];
	rawBuffer->CopyFromOutput(output);

	FILE* file;
	fopen_s(&file, "../RawBuffer.csv", "w");

	for (UINT i = 0; i < count; i++)
	{
		Output temp = output[i];

		//fprintf
		//(
		//	file,
		//	"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
		//	i,
		//	temp.GroupId[0], temp.GroupId[1], temp.GroupId[2],
		//	temp.GroupThreadId[0], temp.GroupThreadId[1], temp.GroupThreadId[2],
		//	temp.DispatchThreadId[0], temp.DispatchThreadId[1], temp.DispatchThreadId[2],
		//	temp.GroupIndex
		//);

		fprintf
		(
			file,
			"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f\n",
			i,
			temp.GroupId[0], temp.GroupId[1], temp.GroupId[2],
			temp.GroupThreadId[0], temp.GroupThreadId[1], temp.GroupThreadId[2],
			temp.DispatchThreadId[0], temp.DispatchThreadId[1], temp.DispatchThreadId[2],
			temp.GroupIndex, temp.RetValue
		);
	}
	fclose(file);
}
