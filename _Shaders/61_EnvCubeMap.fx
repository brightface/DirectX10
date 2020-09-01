    #include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"
#include "00_Terrain.fx"
#include "00_Sky.fx"

float4 PS(MeshOutput input) : SV_Target
{
    return CalcualteFogColor(PS_Shadow(input.sPosition, PS_AllLight(input)), input.wPosition);
}


struct WaterDesc
{
    float4 RefractionColor;

    float2 NormalMapTile;
    float WaveTranslation;
    float WaveScale;

    float WaterShininess;
    float WaterAlpha;
    float WaveSpeed;
    float WaterPositionY;
    
    float TerrainWidth;
    float TerrainHeight;
    float WaterWidth;
    float WaterHeight;
};

Texture2D WaterMap;

cbuffer CB_Water
{
    WaterDesc Water;
};

struct VertexOutput_Water
{
    float4 Position : SV_Position;
    float3 wPosition : Position1;
    float4 ReflectionPosition : Position2;
    float4 RefractionPosition : Position3;
    
    float2 Uv : Uv;
    
    float2 Normal : Uv1;
    float2 Normal2 : Uv2;
};

VertexOutput_Water VS_Water(VertexTexture input)
{
    VertexOutput_Water output;
    
    output.Position = WorldPosition(input.Position);
    output.wPosition = output.Position.xyz;
    
    output.Position = ViewProjection(output.Position); //WVP
    
    
    output.ReflectionPosition = WorldPosition(input.Position);
    output.ReflectionPosition = mul(output.ReflectionPosition, ReflectionView);
    output.ReflectionPosition = mul(output.ReflectionPosition, Projection);
    
    output.RefractionPosition = output.Position;
    
    output.Uv = input.Uv;
    
    output.Normal = input.Uv * Water.NormalMapTile.x;
    output.Normal2 = input.Uv * Water.NormalMapTile.y;
    
    return output;
}

float4 PS_Water(VertexOutput_Water input) : SV_Target
{
    input.Normal.y += Water.WaveTranslation;
    input.Normal2.x += Water.WaveTranslation;
    
    float4 normalMap = NormalMap.Sample(LinearSampler, input.Normal) * 2.0f - 1.0f;
    float4 normalMap2 = NormalMap.Sample(LinearSampler, input.Normal2) * 2.0f - 1.0f;
    float3 normal = normalMap.rgb + normalMap2.rgb;
    
    
    
    float2 reflection;
    reflection.x = input.ReflectionPosition.x / input.ReflectionPosition.w * 0.5f + 0.5f;
    reflection.y = -input.ReflectionPosition.y / input.ReflectionPosition.w * 0.5f + 0.5f;
    reflection += normal.xy * Water.WaveScale;
    float4 reflectionColor = ReflectionMap.Sample(LinearSampler, reflection);
    //return float4(reflectionColor.rgb, 0.5f);
    
    float2 refraction;
    refraction.x = input.RefractionPosition.x / input.RefractionPosition.w * 0.5f + 0.5f;
    refraction.y = -input.RefractionPosition.y / input.RefractionPosition.w * 0.5f + 0.5f;
    refraction += normal.xy * Water.WaveScale;
    float4 refractionColor = RefractionMap.Sample(LinearSampler, refraction);
    //return float4(refractionColor.rgb, 1.0f);
    
    
    float3 light = GlobalLight.Direction;
    light.yz *= -1.0f;
    
    float3 view = normalize(ViewPosition() - input.wPosition);
    float3 heightView = view.yyy;
    
    float r = (1.2f - 1.0f) / (1.2f / 1.0f);
    float fresnel = saturate(min(1, r + (1 - r) * pow(1 - dot(normal, heightView), 2)));
    float4 diffuse = lerp(reflectionColor, refractionColor, fresnel);
    //return float4(diffuse.rgb, 1.0f);
    
 
    float3 R = normalize(reflect(light, normal));
    float specular = saturate(dot(R, view));
    
    [flatten]
    if (specular > 0.0f)
    {
        specular = pow(specular, Water.WaterShininess);
        diffuse = saturate(diffuse + specular);
    }
    //return float4(diffuse.rgb, 1.0f);
    
    
    float x = (Water.TerrainWidth / 2 - Water.WaterWidth / 2) / Water.TerrainWidth;
    float y = (Water.TerrainHeight / 2 - Water.WaterHeight / 2) / Water.TerrainHeight;
    
    float u = input.Uv.x / Water.TerrainWidth * Water.WaterWidth;
    float v = input.Uv.y / Water.TerrainHeight * Water.WaterHeight;
    float2 uv = float2(x, y) + float2(u, v);
    
    float height = HeightMap.Sample(LinearSampler, uv).r * 255.0f / 10.0f;
    height = saturate(Water.WaterPositionY * height / Water.WaterPositionY);
    
    float4 waterColor = WaterMap.Sample(LinearSampler, input.Uv * 0.2f) * 2.0f;
    diffuse += (waterColor * height * 0.25f);
    
    return float4(diffuse.rgb, 1 - (height * 0.75f));
}

technique11 T0
{
    //1Pass - Depth
    P_RS_VP(P0, FrontCounterClockwise_True, VS_Mesh_Depth, PS_Shadow_Depth)
    P_RS_VP(P1, FrontCounterClockwise_True, VS_Model_Depth, PS_Shadow_Depth)
    P_RS_VP(P2, FrontCounterClockwise_True, VS_Animation_Depth, PS_Shadow_Depth)

    //2Pass - Shadow
    P_VP(P3, VS_Mesh, PS)
    P_VP(P4, VS_Model, PS)
    P_VP(P5, VS_Animation, PS)
    P_VP(P6, VS_Terrain, PS_Terrain)

    //2Pass - Sky
    P_VP(P7, VS_Scattering, PS_Scattering)
    P_DSS_VP(P8, DepthEnable_False, VS_Dome, PS_Dome)
    P_DSS_BS_VP(P9, DepthEnable_False, AlphaBlend, VS_Moon, PS_Moon)
    P_DSS_BS_VP(P10, DepthEnable_False, AdditiveBlend, VS_Cloud, PS_Cloud)

    
    //1Pass - Reflection
    P_VP(P11, VS_Mesh_Reflection, PS)
    P_VP(P12, VS_Model_Reflection, PS)
    P_VP(P13, VS_Animation_Reflection, PS)
    P_VP(P14, VS_Terrain_Reflection, PS_Terrain)

    //1Pass - Reflection Sky
    P_VP(P15, VS_Scattering, PS_Scattering)
    P_DSS_VP(P16, DepthEnable_False, VS_Dome_Reflection, PS_Dome)
    P_DSS_BS_VP(P17, DepthEnable_False, AlphaBlend, VS_Moon_Reflection, PS_Moon)
    P_DSS_BS_VP(P18, DepthEnable_False, AdditiveBlend, VS_Cloud_Reflection, PS_Cloud)

    //Pass - Reflection Mesh
    P_BS_VP(P19, AlphaBlend, VS_Water, PS_Water)


    //1Pass - Environment
    P_VGP(P20, VS_Mesh, GS_EnvCube_PreRender, PS)
    P_VGP(P21, VS_Model, GS_EnvCube_PreRender, PS)
    P_VGP(P22, VS_Animation, GS_EnvCube_PreRender, PS)
    P_VGP(P23, VS_Terrain, GS_EnvCube_PreRender, PS_Terrain)

    //1Pass - Environment
    P_DSS_VGP(P24, DepthEnable_False, VS_Dome, GS_EnvCube_PreRender, PS_Dome)
    P_DSS_BS_VGP(P25, DepthEnable_False, AlphaBlend, VS_Moon, GS_EnvCube_PreRender, PS_Moon)
    P_DSS_BS_VGP(P26, DepthEnable_False, AdditiveBlend, VS_Cloud, GS_EnvCube_PreRender, PS_Cloud)

    //2Pass - EnvCubeMap
    P_BS_VP(P27, AlphaBlend, VS_Mesh, PS_EnvCube)
    P_BS_VP(P28, AlphaBlend, VS_Model, PS_EnvCube)
    P_BS_VP(P29, AlphaBlend, VS_Animation, PS_EnvCube)
}