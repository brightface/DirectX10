#include "00_Global.fx"
#include "00_Light.fx"

struct VertexInput
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
    
    matrix Transform : Inst1_Transform; //애에 의해 인스턴시 일어남
    float4 Color : Inst2_Color;
    //인스턴스 번호가 들어와
    uint InstanceId : SV_InstanceID; //인스턴스 아이디 가져올수있따.
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
     //포지션 안주면 멋진 그림 완성돼
    // *2.0f 하면 옆으로 쫙 깔린다 id바뀔때마다 id를 알아낼수있다. (아이디가 1이면 x축으로 1만큼 이동하겠지,) 
    input.Position = input.Position + float4((float) input.InstanceId * 2.0f, 0, 0, 1);
    
    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);
    
    output.Normal = WorldNormal(input.Normal);
    output.Uv = input.Uv;
    
    //처음에는 0번이 들어올테니까 처음에는 검은색이다가 흰색으로 변해가겠지
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