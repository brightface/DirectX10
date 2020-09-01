ByteAddressBuffer Input;
RWByteAddressBuffer Output; //����� RW�� �پ�

struct Group
{
    uint3 GroupId;
    uint3 GroupThreadId; //������ ���̵� ��� �Ű������� �������ϴ°�
    uint3 DispatchThreadId;
    uint GroupIndex;
    float RetValue;
};

//��ǲ ����ü�� ����ž�
struct ComputeInput
{
    uint3 GroupId : SV_GroupID; //SV�ϱ� �ý��ۿ��� �ý��� ��������, �ý��ۿ� �ְų� �ްų�.
    uint3 GroupThreadId : SV_GroupThreadID; //��� ������ ���� ���̵� �޾ƿ�
    uint3 DispatchThreadId : SV_DispatchThreadID;
    uint GroupIndex : SV_GroupIndex;
};

//������ ������ 1024�� �Ѿ�� �ȵ�
[numthreads(10, 8, 3)]
void CS(ComputeInput input)
{
    Group group;
    group.GroupId = asuint(input.GroupId);//asunit�� uint ������ ĳ�����ؼ� �ٲ���� ���´�.
    group.GroupThreadId = asuint(input.GroupThreadId);
    group.DispatchThreadId = asuint(input.DispatchThreadId);
    group.GroupIndex = asuint(input.GroupIndex);

    
    uint inAddress = (input.GroupId.x * 240 + input.GroupIndex) * 4;
    group.RetValue = asfloat(Input.Load(inAddress));
    
    //�迭��ȣ 4����Ʈ ������ ��� �׷쳻������ ��ȣ
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