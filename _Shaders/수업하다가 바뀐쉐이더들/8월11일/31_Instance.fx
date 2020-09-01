#include "00_Global.fx"
#include "00_Light.fx"

struct VertexInput
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
    
    matrix Transform : Inst1_Transform; //�ֿ� ���� �ν��Ͻ� �Ͼ
    float4 Color : Inst2_Color;
    //�ν��Ͻ� ��ȣ�� ����
    uint InstanceId : SV_InstanceID; //�ν��Ͻ� ���̵� �����ü��ֵ�.
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
    float4 Color : Color;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    World = input.Transform;
     //������ ���ָ� ���� �׸� �ϼ���
    // *2.0f �ϸ� ������ �� �򸰴� id�ٲ𶧸��� id�� �˾Ƴ����ִ�. (���̵� 1�̸� x������ 1��ŭ �̵��ϰ���,) 
    input.Position = input.Position + float4((float) input.InstanceId * 2.0f, 0, 0, 1);
    
    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);
    
    output.Normal = WorldNormal(input.Normal);
    output.Uv = input.Uv;
    
    //ó������ 0���� �����״ϱ� ó������ �������̴ٰ� ������� ���ذ�����
     //output.Color = float4(input.InstanceId/50, input.InstanceId/50,input.InstanceId/50, 1);
    output.Color = input.Color;
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target
{
    float NdotL = dot(-GlobalLight.Direction, normalize(input.Normal));
    
    return DiffuseMap.Sample(LinearSampler, input.Uv) * NdotL * input.Color;
}

technique11 T0
{
    P_VP(P0, VS, PS)
}