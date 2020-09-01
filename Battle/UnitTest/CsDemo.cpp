#include "stdafx.h"
#include "CsDemo.h"
#include "CsDemo.h"

void CsDemo::Initialize()
{
	ExecRawBuffer();
}

void CsDemo::ExecRawBuffer()
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

	for (UINT i = 0; i < count; i++)
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
