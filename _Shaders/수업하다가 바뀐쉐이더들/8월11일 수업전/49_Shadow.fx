#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

//float4 PS(MeshOutput input) : SV_Target
//{
//    //float4 color = float4(1, 1, 1, 1);
    
//    //float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);
//    //color *= NdotL;
    
//    float4 color = PS_AllLight(input);
    
    
//    float4 position = input.sPosition;
//    position.xyz /= position.w;
    
//    [flatten]
//    if (position.x < -1.0f || position.x > +1.0f ||
//        position.y < -1.0f || position.y > +1.0f ||
//        position.z < +0.0f || position.z > +1.0f)
//    {
//        return color;
//    }
    
    
//    position.x = position.x * 0.5f + 0.5f;
//    position.y = -position.y * 0.5f + 0.5f;
    
    
//    float depth = 0.0f;
//    float z = position.z - ShadowBias;
//    float factor = 0.0f;
    
//    if(ShadowQuality == 0)
//    {
//        depth = ShadowMap.Sample(LinearSampler, position.xy).r;
//        factor = (float) (depth >= z);
//    }
//    else if (ShadowQuality == 1)
//    {
//        depth = position.z;
//        factor = ShadowMap.SampleCmpLevelZero(ShadowPcfSampler, position.xy, depth);
//    }
//    else if (ShadowQuality == 2)
//    {
//        depth = position.z;
        
//        float2 size = 1.0f / ShadowMapSize;
//        float2 offsets[] =
//        {
//            float2(-size.x, -size.y), float2(0.0f, -size.y), float2(+size.x, -size.y),
//            float2(-size.x, 0.0f), float2(0.0f, 0.0f), float2(+size.x, 0.0f),
//            float2(-size.x, +size.y), float2(0.0f, +size.y), float2(+size.x, +size.y),
//        };
        
        
//        float2 uv = 0;
//        float sum = 0;
        
//        [unroll(9)]
//        for (int i = 0; i < 9; i++)
//        {
//            uv = position.xy + offsets[i];
//            sum += ShadowMap.SampleCmpLevelZero(ShadowPcfSampler, uv, depth).r;
//        }
        
//        factor = sum / 9.0f;
//    }
    
    
//    factor = saturate(factor + depth);
//    return float4((color * factor).rgb, 1.0f);
//}

float4 PS(MeshOutput input) : SV_Target
{
    return PS_Shadow(input.sPosition, PS_AllLight(input));
}

technique11 T0
{
    //1Pass - Depth
    //P_VP(P0, VS_Mesh_Depth, PS_Shadow_Depth)
    //P_VP(P1, VS_Model_Depth, PS_Shadow_Depth)
    //P_VP(P2, VS_Animation_Depth, PS_Shadow_Depth)

    P_RS_VP(P0, FrontCounterClockwise_True, VS_Mesh_Depth, PS_Shadow_Depth)
    P_RS_VP(P1, FrontCounterClockwise_True, VS_Model_Depth, PS_Shadow_Depth)
    P_RS_VP(P2, FrontCounterClockwise_True, VS_Animation_Depth, PS_Shadow_Depth)

    //2Pass - Shadow
    P_VP(P3, VS_Mesh, PS)
    P_VP(P4, VS_Model, PS)
    P_VP(P5, VS_Animation, PS)
}