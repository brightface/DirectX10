#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

//���� �޽� �ƿ�ǲ�̾�.
float4 PS(MeshOutput input) : SV_Target
{
     //light.Direction�� �ƴϾ� �۷ι� ���̷����̾�.
    float NdotL = dot(-GlobalLight.Direction, normalize(input.Normal));
    
    return DiffuseMap.Sample(LinearSampler, input.Uv) * NdotL;
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
    P_VP(P1, VS_Model, PS)
    P_VP(P2, VS_Animation, PS)
}