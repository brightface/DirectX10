#include "00_Global.fx"
#include "00_Light.fx"

cbuffer CB_Snow
{
    float4 Color;

    float3 Velocity;
    float DrawDistance;

    float3 Origin;
    float Tubulence;

    float3 Extent;
};

struct VertexInput
{
    float4 Position : Position;
    float2 Uv : Uv;
    float Scale : Scale;
    float2 Random : Random;
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
    float Alpha : Alpha;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    float3 displace = Velocity * Time;
    
    input.Position.y = Origin.y + Extent.y - (input.Position.y - displace.y) % Extent.y;
    input.Position.x += cos(Time - input.Random.x) * Tubulence;
    input.Position.z += cos(Time - input.Random.y) * Tubulence;
    
    input.Position.xyz = Origin + (Extent + (input.Position.xyz + displace) % Extent) % Extent - (Extent * 0.5f);
    
    
    float4 position = WorldPosition(input.Position);
    
    float3 up = normalize(-Velocity);
    float3 forward = position.xyz - ViewPosition();
    float3 right = normalize(cross(up, forward));
    
    position.xyz += (input.Uv.x - 0.5f) * right * input.Scale;
    position.xyz += (1.0f - input.Uv.y - 0.5f) * up * input.Scale;
    position.w = 1.0f;
    
    output.Position = ViewProjection(position);
    output.Uv = input.Uv;
    
    output.Alpha = 1.0f;
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
    
    return diffuse;
}

technique11 T0
{
    P_BS_VP(P0, AlphaBlend, VS, PS)
    P_BS_VP(P1, AlphaBlend_AlphaToCoverageEnable, VS, PS)

    P_BS_VP(P2, AdditiveBlend, VS, PS)
    P_BS_VP(P3, AdditiveBlend_AlphaToCoverageEnable, VS, PS)
}