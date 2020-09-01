#include "00_Global.fx"
#include "00_Light.fx"

//mrt ���Ҷ��� ������ ������ ���´�.�ϳ��ۿ� ������ϱ� ����ü�� ����ž�.
struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
};

VertexOutput VS(float4 position : Position)
{
    VertexOutput output;
    
    output.Position = position;
    output.Uv.x = position.x * 0.5f + 0.5f;
    output.Uv.y = -position.y * 0.5f + 0.5f;

    return output;
}

float4 PS_Diffuse(VertexOutput input) : SV_Target
{
    return DiffuseMap.Sample(LinearSampler, input.Uv);
}
//���ϵǴ¾ְ� �۷� ��. �ȼ���.
struct PixelOutput
{
    float4 Target0 : SV_Target0;
    float4 Target1 : SV_Target1;
    float4 Target2 : SV_Target2;
};
//�������� �����̾� ����Ÿ���� �־ȿ� �� �����ϱ� �����°ž�. 
PixelOutput PS_Mrt(VertexOutput input)
{
    PixelOutput output;

    output.Target0 = DiffuseMap.Sample(LinearSampler, input.Uv);
    //�̹��� ������Ų�� ������
    output.Target1 = 1.0f - DiffuseMap.Sample(LinearSampler, input.Uv);
    
    float3 color = DiffuseMap.Sample(LinearSampler, input.Uv);
    float average = (color.r + color.g + color.b) / 3.0f;
    output.Target2 = float4(average, average, average, 1.0f);
    //mrt�� �ִ� 8����� �����ִ�.
    //���� 
    
    //����
    //float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
    //output.Target0 = 1.0f - diffuse;

    //���
    //float average = (diffuse.f + diffuse.g + diffuse.b) / 3.0f;
    //output.Target1 = float4(average, average, average, 1.0f);

    //�ﷷ�Ÿ�
    //float uv = input.Uv;
    //uv.x += sin(Time + uv.x * 10) * 0.01f;
    //uv.y += cos(TIme + uv.y * 10) * 0.01f;

    //output.Target2 = DiffuseMap.Sample(LinearSampler.uv);

    return output;
    return output;
}

technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)
    P_VP(P1, VS, PS_Mrt)
}