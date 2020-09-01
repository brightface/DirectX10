#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : SV_Target
{
    float4 color = PS_AllLight(input);
    PS_Projector(color, input.wvpPosition_Sub);
    
    return color;
}

technique11 T0
{
    P_VP(P0, VS_Mesh_Projector, PS)
    P_VP(P1, VS_Model_Projector, PS)
    P_VP(P2, VS_Animation_Projector, PS)
}