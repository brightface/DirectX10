#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    float4 position = 0;
    
    float3 up = float3(0, 1, 0);
    float3 forward = float3(0, 0, 1);
    float3 right = normalize(corss(up, forward));
    
    float4 position = WorldPosition(input.Position);
    position.xyz += (input.Uv.x - 0.5f) * right * input.Scale.x;
    position.xyz += (1.0f - input.UV.y - 05f) * up * input.Scale.y;
    position.w = 1.0f;
    
    position.y += cos(position.y) + sin(Time);
    
    output.Position = ViewProjection(position);
    output.Uv = input.Uv;
    
    return output;
}
float4 PS(VertexOutput input) : SV_Target
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler);
    if(diffuse.a <0.3)
        discard; // 픽쉘세이더만 사용할수있다. discard
    
    clip(diffuse.a - 0, 3f); // 0 이하일떄의 값을 잘라내겠다.
    return diffuse;
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
    P_VP(P1, VS_Model, PS)
    P_VP(P2, VS_Animation, PS)
}