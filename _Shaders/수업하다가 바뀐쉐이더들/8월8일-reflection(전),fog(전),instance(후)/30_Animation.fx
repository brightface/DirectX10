#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

//이제 메시 아웃풋이야.
float4 PS(MeshOutput input) : SV_Target
{
     //light.Direction이 아니야 글로벌 다이렉션이야.
    float NdotL = dot(-GlobalLight.Direction, normalize(input.Normal));
    
    return DiffuseMap.Sample(LinearSampler, input.Uv) * NdotL;
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
    P_VP(P1, VS_Model, PS)
    P_VP(P2, VS_Animation, PS)
}