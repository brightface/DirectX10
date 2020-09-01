struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

cbuffer TransformBuffer : register(b0) // ->  0 ~ 13 // 4096 * 16byte
{
    matrix world;
    matrix view;
    matrix proj;
};

PixelInput VS(VertexInput input)
{
    // 1 x 4            4 x 4  -----> 1 x 4
    //1, 2, 3, 1   *    0000
    //                  0000
    //                  0000
    //                  0000

    PixelInput output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);

    output.uv = input.uv;

    return output;
}

Texture2D source_texture1 : register(t0); //첫번째 이미지
//2번쨰 이미지  cf(128개 슬롯을 가지며 , 0~127번까지 사용가능)
Texture2D source_texture2 : register(t1); //128 -> 0 ~ 127
//샘플러 자원은 16개의 슬롯을 갖늗나.
SamplerState samp        : register(s0); //16 -> 0 ~ 15

float4 PS(PixelInput input) : SV_Target
{
   // float4 color = 0.0f;

    //uv좌표에 따라 다르게 표시할것이다.
    
    //if (input.uv.x < 1.0f)
    //    color = source_texture1.Sample(samp, input.uv);
    //else
    //    color = source_texture2.Sample(samp, float2(input.uv.x - 1.0f, input.uv.y));
    //UV좌표계는 0~1사이다.

    //clip(color.a - 0.9f);
    //if(color.a < 0.1f)
    //    discard;

    float4 color = source_texture1.Sample(samp, input.uv);

    return color;
}