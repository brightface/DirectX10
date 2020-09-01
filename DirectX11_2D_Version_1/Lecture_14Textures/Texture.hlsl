//이미지를 바를 쉐이더

//struct VertexColor
//{
//    D3DXVECTOR3 position;
//    D3DXCOLOR color;
//};

//struct VectexTexture
//{
//    D3DXVECTOR3 position;
//    D3DXVECTOR2 uv; //Texture Coordinate 텍스처 좌표계
//};
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
    //텍스처 좌표계이기 때문에 월드나 뭐 곱하거나 이동시킬 필요 없다.
    return output;
}

//텍스처2D 자료형으로 텍스처 자원을 받아야 한다. cpu바로 접근하게 
//register(b0) : b는 버퍼형 자료 , t는 텍스처 자료 처음시작 0번이기에 t0
Texture2D source_texture : register(t0);
SamplerState samp        : register(s0);
//샘플러 자원 
//sampling 임의의데이터를 추출하는것.

float4 PS(PixelInput input) : SV_Target
{
    float4 color = source_texture.Sample(samp, input.uv);

   //1. clip(color.a - 0.9f);
    //데이터가 0보다 작으면 해당 데이터를 폐기한다.
    
    //2. (1)번과 같다. 
    if (color.a < 0.1f)
        discard;
    //끝이 검정색이다. 이거 블랜드를 사용해서 없애겠다 마지막강의에서
    
    return color;
}
//clip함수로 지울수 있다. 알파값
//uv만들었고 텍스처 붙여봤고 알파값도 지워봤다.!!
//discard 
//sampleing sate 어떻게 뽑아낼것인가.