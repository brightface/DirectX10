#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

Texture2D ShadowMap;
float4 PS(MeshOutput input) : SV_Target
{
    //흰색
    //float4 color = float4(1, 1, 1, 1);
    float4 color = PS_AllLight(input);
    //ps_alllight 조명까지 다 계산되어 오잖아.
    
    //월드 뷰 프로젝션 해놨었지 걔야 걔
    float4 position = input.sPosition;
    
    position.xyz /= position.w;
    
    position.x = position.x * 0.5f + 0.5f;
    position.y = -position.y * 0.5f + 0.5f;
   // position.z = position.z * 0.5f + 0.5f;
    
    [flatten]
    if (position.x < -1.0f || position.x > +1.0f ||
        position.y < -1.0f || position.y > +1.0f ||
        position.z < +0.0f || position.z > +1.0f)
    {
        return color;
    }

    position.x = position.x * 0.5f + 0.5f;
    position.y = -position.y * 0.5f + 0.5f;
    
    float depth = 0.0f; //Pass1
    float z = position.z - ShadowBias;
    float factor = 0.0f; //그림자 값 그림자가 된다.
    
    //r 값 깊이
    depth = ShadowMap.Sample(LinearSampler, position.xy).r;
    factor = (float) (depth >= z);
    
    if(ShadowQuality == 0)
    {
        //r 값 깊이
        depth = ShadowMap.Sample(LinearSampler, position.xy).r;
        factor = (float) (depth >= z);
    }
    else if (ShadowQuality == 1)
    {
        depth = position.z;
        factor = ShadowMap.SampleCmpLevelZero(ShadowSampler, position.xy, depth).r;
    }
    else if (ShadowQuality == 2)
    {
        depth = position.z;
        
        float2 size = 1.0f / ShadowMapSize;
        float2 offsets[] =
        {
            float2(-size.x, -size.y), float2(0.0f, -size.y), float2(+size.x, -size.y),
            float2(-size.x, 0.0f), float2(0.0f, 0.0f), float2(+size.x, 0.0f),
            float2(-size.x, +size.y), float2(0.0f, +size.y), float2(+size.x, +size.y),
        };
        
        
        float2 uv = 0;
        float sum = 0;
        
        [unroll(9)]
        for (int i = 0; i < 9; i++)
        {
            uv = position.xy + offsets[i];
            sum += ShadowMap.SampleCmpLevelZero(ShadowSampler, uv, depth).r;
        }
        
        factor = sum / 9.0f;
    }
    
    factor = saturate(factor + depth);
    return float4((color * factor).rgb, 1.0f);
}

technique11 T0
{
    //1Pass - Depth
    //P_VP(P0, VS_Depth_Mesh, PS_Depth_Shadow)    
    //P_VP(P1, VS_Depth_Model, PS_Depth_Shadow)
    //P_VP(P2, VS_Depth_Animation, PS_Depth_Shadow)

    //1Pass - Depth
    P_RS_VP(P0, FrontCounterClockwise_True, VS_Depth_Mesh, PS_Depth_Shadow)
    P_RS_VP(P1, FrontCounterClockwise_True, VS_Depth_Model, PS_Depth_Shadow)
    P_RS_VP(P2, FrontCounterClockwise_True, VS_Depth_Animation, PS_Depth_Shadow)

    //2Pass - Z
    P_VP(P3, VS_Mesh, PS)
    P_VP(P4, VS_Model, PS)
    P_VP(P5, VS_Animation, PS)

    //g_fighting 자글자글한것. z가 1이면 자글자글한것.그래서 생겨
    //같은 깊이에서 그려졌잖아. 그래서 자글자글
}