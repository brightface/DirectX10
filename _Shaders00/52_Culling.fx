#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

struct VertexOutput
{
    float4 Position : SV_Position0; //Rasterzing Position;
    float4 wvpPosition : Position1; //WVP Position
    float4 wvpPosition_Sub : Position2; //WVP Sub Position
    float3 oPosition : Position3; //Original
    float3 wPosition : Position4; //World Position
    float4 sPosition : Position5; //Shaodw VP Position

    float3 Normal : Normal;
    float3 Tangent : Tangent;
    float2 Uv : Uv;
    float4 Color : COLOR;
    
    //float4 Culling : SV_CullDistance0;
    //float4 Culling2 : SV_CullDistance1;
    
    float4 Culling : SV_ClipDistance0;
    float4 Culling2 : SV_ClipDistance1;
};

float4 Planes[6];
VertexOutput VS(VertexModel input)
{
    VertexOutput output;
    SetModelWorld(World, input);
    
    VS_GENERATE
    
    output.Culling.x = dot(float4(output.wPosition, 1), Planes[0]);
    output.Culling.y = dot(float4(output.wPosition, 1), Planes[1]);
    output.Culling.z = dot(float4(output.wPosition, 1), Planes[2]);
    output.Culling.w = dot(float4(output.wPosition, 1), Planes[3]);
    output.Culling2.x = dot(float4(output.wPosition, 1), Planes[4]);
    output.Culling2.y = dot(float4(output.wPosition, 1), Planes[5]);
    output.Culling2.z = output.Culling2.w = 0.0f;

    return output;
}

float4 PS(VertexOutput input) : SV_Target0
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);

    float3 normal = normalize(input.Normal);
    float NdotL = dot(normal, -GlobalLight.Direction);

    return diffuse * NdotL;
}

RasterizerState Fill
{
    FillMode = Wireframe;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
    
    pass P1
    {
        SetRasterizerState(Fill);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}