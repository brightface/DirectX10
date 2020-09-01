#include "00_Global.fx"
#include "00_Light.fx"

float2 PixelSize;

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
};

VertexOutput VS(float4 position : Position)
{
    VertexOutput output;
    
    output.Position = position;
    output.Uv.x = position.x * 0.5f + 0.5f;
    output.Uv.y = -position.y * 0.5f + 0.5f;

    return output;
}

float4 PS_Diffuse(VertexOutput input) : SV_Target
{
    return DiffuseMap.Sample(LinearSampler, input.Uv);
}

//가우시안 함수 가중치
static const float Weights[13] =
{
    0.0561f, 0.1353f, 0.2780f, 0.4868f, 0.7261f, 0.9231f,
    1.0f,//자기자신
    0.9231f, 0.7261f, 0.4868f, 0.2780f, 0.1353f, 0.0561f
};

float4 PS_GaussianBlurX(VertexOutput input) : SV_Target
{
    float2 uv = input.Uv;
    float u = PixelSize.x;
    
    float sum = 0;
    float4 color = 0;
    for (int i = -6; i <= 6; i++)
    {
        float2 temp = uv + float2(u * (float) i, 0.0f);
        color += DiffuseMap.Sample(LinearSampler, temp) * Weights[6 + i];
        
        sum += Weights[6 + i];
    }

    color /= sum;
    
    return float4(color.rgb, 1.0f);
}

float4 PS_GaussianBlurY(VertexOutput input) : SV_Target
{
    float2 uv = input.Uv;
    float v = PixelSize.y;
    
    float sum = 0;
    float4 color = 0;
    for (int i = -6; i <= 6; i++)
    {
        float2 temp = uv + float2(0.0f, v * (float) i);
        color += DiffuseMap.Sample(LinearSampler, temp) * Weights[6 + i];
        
        sum += Weights[6 + i];
    }

    color /= sum;
    
    return float4(color.rgb, 1.0f);
}


struct PixelOutput
{
    float4 Target0 : SV_Target0;
    float4 Target1 : SV_Target1;
};

PixelOutput PS_GaussianBlurComposite(VertexOutput input)
{
    PixelOutput output;
    
    float2 uv = input.Uv;
    float u = PixelSize.x;
    
    float sum = 0;
    float4 color = 0;
    for (int i = -6; i <= 6; i++)
    {
        float2 temp = uv + float2(u * (float) i, 0.0f);
        color += DiffuseMap.Sample(LinearSampler, temp) * Weights[6 + i];
        
        sum += Weights[6 + i];
    }
    color /= sum;
    output.Target0 = float4(color.rgb, 1.0f);
    
    
    float v = PixelSize.y;
    
    sum = 0;
    color = 0;
    for (i = -6; i <= 6; i++)
    {
        float2 temp = uv + float2(0.0f, v * (float) i);
        color += DiffuseMap.Sample(LinearSampler, temp) * Weights[6 + i];
        
        sum += Weights[6 + i];
    }
    color /= sum;
    output.Target1 = float4(color.rgb, 1.0f);
    
    return output;
}
//x,y합쳐서 랜더링할 코드를 만들거야. 
Texture2D GaussianMrt[2]; //0번 1번 을 넣어줄거야. 
float4 PS_GaussianCombined(VertexOutput input) : SV_Target
{   //0번이 x잖아. 
    float4 x = GaussianMrt[0].Sample(LinearSampler, input.Uv);
    //1번이 y잖아.
    float4 y = GaussianMrt[1].Sample(LinearSampler, input.Uv);
    
    return float4((x.rgb + y.rgb) * 0.5f, 1.0f); //평균낼거야.

}

technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)
    P_VP(P1, VS, PS_GaussianBlurX)
    P_VP(P2, VS, PS_GaussianBlurY)
    P_VP(P3, VS, PS_GaussianBlurComposite)
    P_VP(P4, VS, PS_GaussianCombined)
}