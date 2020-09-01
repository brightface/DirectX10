#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : SV_Target
{
    float3 position = input.wvpPosition.xyz / input.wvpPosition.w;
    
    //if(position.z < 0.9f)
        //return float4(1, 0, 0, 1);
    
    float n = 0.1f;
    float f = 1000.0f;
    
    float depth = (2.0f * n) / (f + n - position.z * (f - n));
    
    return float4(depth, depth, depth, 1.0f);
}

technique11 T0
{
    P_VP(P0, VS_Mesh_Projector, PS)
    P_VP(P1, VS_Model_Projector, PS)
    P_VP(P2, VS_Animation_Projector, PS)
}