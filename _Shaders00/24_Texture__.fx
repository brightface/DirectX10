ByteAddressBuffer Input;
RWByteAddressBuffer Output;

struct Group
{
    uint3 GroupId;
    uint3 GroupThreadId;
    uint3 DispatchThreadId;
    uint GroupIndex;
    float RetValue;
};

struct ComputeInput
{
    uint3 GroupId : SV_GroupID;
    uint3 GroupThreadId : SV_GroupThreadID;
    uint3 DispatchThreadId : SV_DispatchThreadID;
    uint GroupIndex : SV_GroupIndex;
};
//��ó�� ȿ���� �Ȱ���
[numthreads(32, 32, 1)]
void CS(uint23 id : SV_DispatchThreadID)
{
    float pixel = Input.Load(int4(id, 0));
    float 

}


technique11 T0
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);

        SetComputeShader(CompileShader(cs_5_0, CS()));
    }
    pass P1
    {

    }
}