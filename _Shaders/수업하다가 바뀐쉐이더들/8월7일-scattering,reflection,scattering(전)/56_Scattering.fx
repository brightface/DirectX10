#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"
#include "00_Terrain.fx"
#include "00_Sky.fx"

float4 PS(MeshOutput input) : SV_Target
{
    return CalcPS_Shadow(input.sPosition, PS_AllLight(input), input.wPosition);
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
    P_DSS_BS_VP(P9, DepthEnable_False, AlphaBlend, VS_Moon, PS_Moon)//깊이를 끄고 그린다<먼저그려지니까-끄면 가려진다.>. 알파블랜드
    P_BS_VS(P10, DepthEnable_False, VS_Cloud, PS_Cloud)

}