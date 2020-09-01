#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

technique11 T0
{
    P_VP(P0, VS_Mesh_Depth, PS_Shadow_Depth)
    P_VP(P1, VS_Model_Depth, PS_Shadow_Depth)
    P_VP(P2, VS_Animation_Depth, PS_Shadow_Depth)
}