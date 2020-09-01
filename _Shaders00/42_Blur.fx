#include "00_Global.fx"
#include "00_Light.fx"

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


float2 PixelSize;
//주변픽셀의 평균이라 한다.
uint BlurCount = 8;
float4 PS_Blur(VertexOutput input) : SV_Target
{   
    //자기 주위 픽셀들 
    float2 arr[9] =
    {   //uv에 곱할거거든 주변픽셀값. 
        //왼상단픽셀
        float2(-1, -1), float2(+0, -1), float2(+1, -1),
                         //자기값 그대로 y값
        float2(-1, +0), float2(+0, +0), float2(+1, +0),
        
        float2(-1, +1), float2(+0, +1), float2(+1, +1),
    };
    
    float3 color = 0;
    
    for (uint blur = 0; blur < BlurCount; blur++)
    {   //unroll로 고정시켜도 되겠네, 함수니까
        [unroll(9)]
        for (int i = 0; i < 9; i++) //배열은 9개이다. 
        {   
            //왼쪽 픽셀을 알수 있지. 그다음에 블러만큼(옆으로 퍼져나가게) 크면클수록 옆으로 퍼져나가겠지. 옆픽셀사이즈까지.
            float x = arr[i].x * (float) blur * PixelSize.x;
            float y = arr[i].y * (float) blur * PixelSize.y;
            
            //uv를 만들거야.
            float2 uv = input.Uv + float2(x, y);
            color += DiffuseMap.Sample(LinearSampler, uv).rgb;
        }
    }
    //평균이니까 더한만큼 나눠야겠지. 9개 픽셀이니까. 
    color /= BlurCount * 9;
    
    return float4(color.rgb, 1);
}


uint RadialBlurCount = 8; 
float RadialBlurRadius = 0.0f;
float RadialBlurAmount = 0.04f; //4%
float2 RadialCenter = float2(0.5f, 0.5f); //어디서 블러를 시작할지

float4 PS_RadialBlur(VertexOutput input) : SV_Target
{   
    //uv에서 빼서 중심을 잡는거야.
    float2 radius = input.Uv - RadialCenter;
    float r = length(radius) + 1e-6f; //약간의 오차값을 + 시킬거야.
    radius /= r; //레디어스에 r로 나눠줄려고 radius 방향이 되지 노멜벡터 .
    //정규화, 방향이 된다.
    
    r = 2 * r / RadialBlurRadius; //비율이 될것이다.
    r = saturate(r); //r은 0~1까지 떨어뜨리는거야.
    
    //방향 * r * r // 2파이 r 퍼져나갈 비율. 그리고 블러ㄱ카운트를 나눠져. 횟수안에 갖히도록 만든거
    //델타는 비율만들려고 한거야
    float2 delta = radius * r * r * RadialBlurAmount / RadialBlurCount;
    delta = -delta; //델타를 뒤집을거야. uv에 대해 뺴서 그래, 양의값으로 만들려고 , uv에대해 더하면 비율이 안맞아.
    
    float4 color = 0;
    for (uint i = 0; i < RadialBlurCount; i++)
    {
        //디퓨즈 맵에 리니어 샘플러. 델타를 합산해놓을거야. 현재 픽셀이 아니라 다음 픽셀인거지 델타는
        color += DiffuseMap.Sample(LinearSampler, input.Uv);
        input.Uv += delta;
    }
    color /= RadialBlurCount;
    
    return float4(color.rgb, 1);
}

technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)
    P_VP(P1, VS, PS_Blur)
    P_VP(P2, VS, PS_RadialBlur)
}