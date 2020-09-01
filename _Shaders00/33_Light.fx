#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : SV_Target
{
    Texture(Materail.diffuse, DiffuseMap, input.uv);
    
    MaterialDescc output;
    ComputeLight(output, input.Normal, input.wPosition);
    
    return float4(MaterialToColor(output).rgb, 1.0f);
}


technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
    P_VP(P1, VS_Model, PS)
    P_VP(P2, VS_Animation, PS)
}