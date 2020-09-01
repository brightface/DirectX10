struct VertexInput
{
    float4 position : POSITION0;
    float4 color : COLOR0;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
    float4 color : COLOR0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = input.position;
    output.color = input.color;

    return output;
}
//RGBA 4���� ���� �ȼ�������ŭ ȣ�� �����Ͷ���� ��
float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}

//gpu�� �׷��� . �׷� ��𿡴� �׸��� ����
//float4 PS(PixelInput input) 
//{
//    return input.color;
//}