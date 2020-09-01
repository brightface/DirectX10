ByteAddressBuffer Input;
RWByteAddressBuffer Output; //출력은 RW가 붙어

struct Group
{
    uint3 GroupId;
    uint3 GroupThreadId; //쓰레드 아이디가 어떻게 매겨지는지 볼려고하는것
    uint3 DispatchThreadId;
    uint GroupIndex;
    float RetValue;
};

//인풋 구조체를 만들거야
struct ComputeInput
{
    uint3 GroupId : SV_GroupID; //SV니까 시스템에서 시스템 벨류겠지, 시스템에 주거나 받거나.
    uint3 GroupThreadId : SV_GroupThreadID; //어느 쓰레드 인지 아이디를 받아와
    uint3 DispatchThreadId : SV_DispatchThreadID;
    uint GroupIndex : SV_GroupIndex;
};

//쓰레드 개수야 1024개 넘어가면 안돼
[numthreads(10, 8, 3)]
void CS(ComputeInput input)
{
    Group group;
    group.GroupId = asuint(input.GroupId);//asunit로 uint 형으로 캐스팅해서 바꿔줘야 나온다.
    group.GroupThreadId = asuint(input.GroupThreadId);
    group.DispatchThreadId = asuint(input.DispatchThreadId);
    group.GroupIndex = asuint(input.GroupIndex);

    
    uint inAddress = (input.GroupId.x * 240 + input.GroupIndex) * 4;
    group.RetValue = asfloat(Input.Load(inAddress));
    
    //배열번호 4바이트 단위로 계산 그룹내에서의 번호
    uint outAddress = (input.GroupId.x * 240 + input.GroupIndex) * 11 * 4;    
    Output.Store3(outAddress + 0, asuint(group.GroupId));
    Output.Store3(outAddress + 12, asuint(group.GroupThreadId));
    Output.Store3(outAddress + 24, asuint(group.DispatchThreadId));
    Output.Store(outAddress + 36, asuint(group.GroupIndex));
    Output.Store(outAddress + 40, asuint(group.RetValue));
}

technique11 T0
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);

        SetComputeShader(CompileShader(cs_5_0, CS()));
    }
}