#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

struct VertexOutput
{
    float4 Position : Position;
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

struct PixelOutput
{
    float4 Target0 : SV_Target0;
    float4 Target1 : SV_Target1;
    float4 Target2 : SV_Target2;
};

PixelOutput PS_Mrt(VertexOutput input)
{
    PixelOutput output;
    
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
    output.Target0 = 1.0f - diffuse;
    
    float average = (diffuse.r * diffuse.g * diffuse.b) / 3.0f;
    output.Target1 = float4(average, average, average, 1.0f);

    float2 uv = input.Uv;
    uv.x += sin(Time + uv.x * 10) * 0.01f;
    uv.y += cos(Time + uv.y * 10) * 0.01f;
    output.Target2 = DiffuseMap.Sample(LinearSampler, uv);
    
    return output;
}


technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)
    P_VP(P1, VS, PS_Mrt)
}