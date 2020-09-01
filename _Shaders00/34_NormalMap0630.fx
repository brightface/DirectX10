#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : SV_Target
{
    MaterialDesc.DiffuseMap = flat4(1, 1, 1, 1);
    
    if(Selected ==0 )
    {
        
    }
    eles if(Selected == 1)
    {
        NormalMapping(input, Uv, input.Normal, input.Tanget);
    
    }
    else if(Selected == 2)
    {
        Texture(Material.Diffuse, DiffuseMap, input.Uv);
        
    }
    else if(Selected ==3)
    {
        NormalMapping(input.Uv, input.Normal, input.Tangent);
        Texture(Mapterial.Diffuse, DiffuseMap, input.Uv);
    }
    
    Texture(Material.Diffuse, DiffuseMap, input.Uv);
    Texture(Material.Specular, SpecularMap, input.Uv);
    
    MaterialDesc output;
    ComputeLight(output, input.Normal, input.wPosition);
    
    return float4(MaterialToColor(output).rgb, 1.0f);
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
    P_VP(P1, VS_Model, PS)
    P_VP(P2, VS_Animation, PS)
}