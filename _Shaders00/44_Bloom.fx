#include "00_Global.fx"
#include "00_Light.fx"

float2 PixelSize;
//mrt 한번에 가도 되는데 , 밝은부분 추출해서 먼저 차례차례 가겠다.

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

//얼마만큼 값 이상을 추출할건지. 얼마만큼 밝기 이상을 
float Threshold = 0.6f;
//밝은 부분만 추출하는 부분만 Luminosity
float4 PS_Luminosity(VertexOutput input) : SV_Target
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
    //들어오는 부분
    //threhold 비율이지  그 값 이상만 남기겠다. 
    return saturate((diffuse - Threshold) / (1 - Threshold));
}
//0.4 부분으로 나눈값 0.6
#define MAX_SAMPLE_COUNT 33
int SampleCount = 15;

float2 Offsets[MAX_SAMPLE_COUNT];
float Weights[MAX_SAMPLE_COUNT];

float4 PS_Blur(VertexOutput input) : SV_Target
{
    float4 color = 0;
    
    for (int i = 0; i < SampleCount; i++)
        color += DiffuseMap.Sample(LinearSampler, input.Uv + Offsets[i]) * Weights[i];
    //분석 , 평균이다.
    return color;
}

//합칠거야. composite
Texture2D LuminosityMap; //맵을 샘플링하고
Texture2D BlurMap; 
float4 PS_Composite(VertexOutput input) : SV_Target
{
    float4 luminosity = LuminosityMap.Sample(LinearSampler, input.Uv);
    float4 blur = BlurMap.Sample(LinearSampler, input.Uv);
    //원하는 식으로 섞은 식이다.
    luminosity *= (1.0f - saturate(blur));
    
    return float4((luminosity + blur).rgb, 1.0f);
}


float RGBCVtoHUE(in float3 RGB, in float C, in float V)
{
    float3 Delta = (V - RGB) / C;
    Delta.rgb -= Delta.brg;
    Delta.rgb += float3(2, 4, 6);

    Delta.brg = step(V, RGB) * Delta.brg;
    
    float H = max(Delta.r, max(Delta.g, Delta.b));
    return frac(H / 6);
}
 
float3 RGBtoHSV(in float3 RGB)
{
    float3 HSV = 0;
    HSV.z = max(RGB.r, max(RGB.g, RGB.b));
    float M = min(RGB.r, min(RGB.g, RGB.b));
    float C = HSV.z - M;
    if (C != 0)
    {
        HSV.x = RGBCVtoHUE(RGB, C, HSV.z);
        HSV.y = C / HSV.z;
    }
    return HSV;
}
 
float3 HUEtoRGB(in float H)
{
    float R = abs(H * 6 - 3) - 1;
    float G = 2 - abs(H * 6 - 2);
    float B = 2 - abs(H * 6 - 4);
    return saturate(float3(R, G, B));
}
 
float3 HSVtoRGB(float3 HSV)
{
    float3 RGB = HUEtoRGB(HSV.x) - 1;
    float3 temp = (RGB * HSV.y + 1);

    temp *= HSV.z;

    return temp;
}

float3 HSVComplement(float3 HSV)
{
    float3 complement = HSV;
    complement.x -= 0.5;
    if (complement.x < 0.0)
    {
        complement.x += 1.0;
    }
    
    return (complement);
}

float HueLerp(float h1, float h2, float v)
{
    float d = abs(h1 - h2);
    if (d <= 0.5)
    {
        return lerp(h1, h2, v);
    }
    else if (h1 < h2)
    {
        return frac(lerp((h1 + 1.0), h2, v));
    }
    else
    {
        return frac(lerp(h1, (h2 + 1.0), v));
    }
}

float3 PostComplement(float3 input)
{
    float3 guide = float3(1.0f, 0.5f, 0.0f);
    float amount = 0.5f;
    float correlation = 0.5f; //이값과 
    float concentration = 2.0f; //이값에 의해 바뀐다. 
 
    
    float3 input_hsv = RGBtoHSV(input);
    float3 hue_pole1 = RGBtoHSV(guide);
    float3 hue_pole2 = HSVComplement(hue_pole1);
 
    float dist1 = abs(input_hsv.x - hue_pole1.x);
    if (dist1 > 0.5)
        dist1 = 1.0 - dist1;
    float dist2 = abs(input_hsv.x - hue_pole2.x);
    if (dist2 > 0.5)
        dist2 = 1.0 - dist2;
 
    float descent = smoothstep(0, correlation, input_hsv.y);
 
    
    float3 output_hsv = input_hsv;
    if (dist1 < dist2)
    {
        float c = descent * amount * (1.0 - pow((dist1 * 2.0), 1.0 / concentration));
        
        output_hsv.x = HueLerp(input_hsv.x, hue_pole1.x, c);
        output_hsv.y = lerp(input_hsv.y, hue_pole1.y, c);
    }
    else
    {
        float c = descent * amount * (1.0 - pow((dist2 * 2.0), 1.0 / concentration));
        
        output_hsv.x = HueLerp(input_hsv.x, hue_pole2.x, c);
        output_hsv.y = lerp(input_hsv.y, hue_pole2.y, c);
    }
 
    return HSVtoRGB(output_hsv);
}
    
    //HSV 색공간이다.  색이 쨍한 느낌이다. 더 확실해진다.
float4 PS_Grading(VertexOutput input) : SV_Target
{
    float4 extract = DiffuseMap.Sample(LinearSampler, input.Uv);
    float3 color = PostComplement(float3(extract.r, extract.g, extract.b*2)); //이러면 파라색으로 나온다. 
    float3 color = PostComplement(float3(extract.r, extract.g, extract.b)); //이런 푸르딩딩한 색 만드는거야 밝은 쪽은 푸르딩딩한것 덜들어가. 그래이딩이 색이 진하게 , 낮으면낮을수록, hsv벡터로 색을 다루는것이다. rgv
	
    return float4(color, 1.0f);
}

technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)
    P_VP(P1, VS, PS_Luminosity)
    P_VP(P2, VS, PS_Blur)
    //P3 PS_Composite
    P_VP(P3, VS, PS_Composite)
    P_VP(P4, VS, PS_Grading)
    
}