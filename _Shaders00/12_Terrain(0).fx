#include "00_Global.fx"
float3 LightDirection = float3(-1, -1, +1);

struct VertexInput
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
    float4 Color : Color;
    float3 Normal : Normal;
};

float4 GetHeightColor(float y)
{
    if(y > 20.0f)
        return float4(1, 0, 0, 1);
    
    if(y > 10.0f)
        return float4(0, 1, 0, 1);
    
    if (y > 5.0f)
        return float4(0, 0, 1, 1);
    
    return float4(1, 1, 1, 1);
}

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Color = GetHeightColor(output.Position.y);
    
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;
    
    return output;
}

Texture2D BaseMap;
//SamplerState LinearSampler
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = Wrap;
//    AddressV = Wrap;
//};

int Lighting;
float4 PS(VertexOutput input) : SV_Target
{    
    //float4 color = float4(1, 1, 1, 1);
    float4 color = BaseMap.Sample(LinearSampler, input.Uv) * input.Color;
    float3 normal = normalize(input.Normal);
    
    if (Lighting == true)    
        return color * dot(-LightDirection, normal);
    
    return color;
}

RasterizerState RS
{
    FillMode = Wireframe;
};

technique11 T0
{
    P_VP(P0,VS,PS)
    //
}