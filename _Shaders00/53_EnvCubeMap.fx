#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"
#include "00_Terrain.fx"

struct EnvCubeDesc
{
    uint Type;
    float Alpha;
    float RefractAmount;
    float Padding;
    
    float FresnelAmount;
    float FresnelBias;
    float FresnelScale;
    float Padding2;
    
    matrix Views[6];
    matrix Projection;
};

cbuffer CB_EnvCube
{
    EnvCubeDesc EnvCube;
};

[maxvertexcount(18)]
void GS_EnvCube_PreRender(triangle MeshOutput input[3], inout TriangleStream<MeshGeometryOutput> stream)
{
    int vertex = 0;
    MeshGeometryOutput output;
    
    [unroll(6)]
    for (int i = 0; i < 6; i++)
    {
        output.TargetIndex = i;
        
        [unroll(3)]
        for (vertex = 0; vertex < 3; vertex++)
        {
            output.Position = mul(float4(input[vertex].wPosition, 1), EnvCube.Views[i]);
            output.Position = mul(output.Position, EnvCube.Projection);
            
            output.oPosition = input[vertex].oPosition;
            output.wPosition = input[vertex].wPosition;
            output.wvpPosition = output.Position;
            output.wvpPosition_Sub = output.Position;
            output.sPosition = input[vertex].sPosition;
            output.Normal = input[vertex].Normal;
            output.Tangent = input[vertex].Tangent;
            output.Uv = input[vertex].Uv;
            output.Color = input[vertex].Color;
            
            stream.Append(output);
        }

        stream.RestartStrip();
    }
}

float4 PS_EnvCube_PreRender_Sky(MeshGeometryOutput input) : SV_Target
{
    return PS_Sky(ConvertMeshOutput(input));
}

float4 PS_EnvCube_PreRender_Terrain(MeshGeometryOutput input) : SV_Target
{
    return PS_Terrain(ConvertMeshOutput(input));
}

float4 PS_EnvCube_PreRender(MeshGeometryOutput input) : SV_Target
{
    return PS_Shadow(input.sPosition, PS_AllLight(ConvertMeshOutput(input)));
}

float4 PS_EnvCube(MeshOutput input) : SV_Target
{
    float4 color = 0;
    float4 diffuse = 0;
    
    float3 view = normalize(input.wPosition - ViewPosition());
    float3 normal = normalize(input.Normal);
    float3 reflection = reflect(view, normal);
    float3 refraction = refract(view, normal, EnvCube.RefractAmount);
    
    if(EnvCube.Type == 0)
    {
        color = EnvCubeMap.Sample(LinearSampler, input.oPosition);
    }
    else if (EnvCube.Type == 1)
    {
        color = EnvCubeMap.Sample(LinearSampler, reflection);
    }
    else if (EnvCube.Type == 2)
    {
        color = EnvCubeMap.Sample(LinearSampler, -refraction);
        color.a = EnvCube.Alpha;
    }
    else if (EnvCube.Type == 3)
    {
        diffuse = PS_AllLight(input);
        color = EnvCubeMap.Sample(LinearSampler, reflection);
        
        color.rgb *= 0.15f + diffuse * 0.95f;
        color.a = EnvCube.Alpha;
    }
    else if (EnvCube.Type == 4)
    {
        diffuse = PS_AllLight(input);
        color = EnvCubeMap.Sample(LinearSampler, reflection);
        
        float4 fresnel = EnvCube.FresnelBias + (1.0f - EnvCube.FresnelScale) * pow(abs(1.0f - dot(view, normal)), EnvCube.FresnelAmount);
        color = EnvCube.FresnelAmount * diffuse + lerp(diffuse, color, fresnel);
        color.a = EnvCube.Alpha;
    }
    
    return color;
}

float4 PS(MeshOutput input) : SV_Target
{
    return PS_Shadow(input.sPosition, PS_AllLight(input));
}

technique11 T0_Shadow
{
    //1Pass - Depth
    P_RS_VP(P0, FrontCounterClockwise_True, VS_Depth_Mesh, PS_Depth_Shadow)
    P_RS_VP(P1, FrontCounterClockwise_True, VS_Depth_Model, PS_Depth_Shadow)
    P_RS_VP(P2, FrontCounterClockwise_True, VS_Depth_Animation, PS_Depth_Shadow)
    P_RS_VP(P3, FrontCounterClockwise_True, VS_Depth_Terrain, PS_Depth_Shadow)

    //2Pass - Sky
    P_RS_DSS_VP(P4, FrontCounterClockwise_True, DepthEnable_False, VS_Mesh, PS_Sky)

    //2Pass - Z
    P_VP(P5, VS_Mesh, PS)
    P_VP(P6, VS_Model, PS)
    P_VP(P7, VS_Animation, PS)
    P_VP(P8, VS_Terrain, PS_Terrain)

    //EnvCube PreRender
    P_RS_DSS_VGP(P9, FrontCounterClockwise_True, DepthEnable_False, VS_Mesh, GS_EnvCube_PreRender, PS_EnvCube_PreRender_Sky)
    P_VGP(P10, VS_Mesh, GS_EnvCube_PreRender, PS_EnvCube_PreRender)
    P_VGP(P11, VS_Model, GS_EnvCube_PreRender, PS_EnvCube_PreRender)
    P_VGP(P12, VS_Animation, GS_EnvCube_PreRender, PS_EnvCube_PreRender)
    P_VGP(P13, VS_Terrain, GS_EnvCube_PreRender, PS_EnvCube_PreRender_Terrain)

    //EnvCube Render
    P_BS_VP(P14, AlphaBlend, VS_Mesh, PS_EnvCube)
    P_BS_VP(P15, AlphaBlend, VS_Model, PS_EnvCube)
    P_BS_VP(P16, AlphaBlend, VS_Animation, PS_EnvCube)
}