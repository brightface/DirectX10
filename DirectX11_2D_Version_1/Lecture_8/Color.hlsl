struct VertexInput
{
    float4 position : POSITION0;
    float4 color : COLOR0;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
    float4 color : COLOR0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = input.position;
    output.color = input.color;

    return output;
}
//RGBA 4개의 정보 픽셀개수만큼 호출 레스터라이즈가 한
float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}

//gpu야 그려봐 . 그럼 어디에다 그린지 몰라
//float4 PS(PixelInput input) 
//{
//    return input.color;
//}