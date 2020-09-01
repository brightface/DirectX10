#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"
#include "00_Terrain.fx"
#include "00_Sky.fx"

float4 PS(MeshOutput input) : SV_Target
{
    return CalcualteFogColor(PS_Shadow(input.sPosition, PS_AllLight(input)), input.wPosition);
}


MeshOutput VS_Draw_Mesh(VertexMesh input)
{
    MeshOutput output = VS_Mesh(input);
    
    float4 position = WorldPosition(input.Position);
    position = mul(position, ReflectionView);
    position = mul(position, Projection);
    
    output.wvpPosition_Sub = position;
    
    return output;
}

float4 PS_Reflection(MeshOutput input) : SV_Target
{
    float4 reflectionPosition = input.wvpPosition_Sub;
    
    float2 reflection;
    reflection.x = reflectionPosition.x / reflectionPosition.w * 0.5f + 0.5f;
    reflection.y = -reflectionPosition.y / reflectionPosition.w * 0.5f + 0.5f;

    float4 color = ReflectionMap.Sample(LinearSampler, reflection);
    
    return float4(color.rgb, 0.5f);
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
    P_BS_VP(P19, AlphaBlend, VS_Draw_Mesh, PS_Reflection)
}