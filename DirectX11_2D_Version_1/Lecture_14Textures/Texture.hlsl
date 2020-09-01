//�̹����� �ٸ� ���̴�

//struct VertexColor
//{
//    D3DXVECTOR3 position;
//    D3DXCOLOR color;
//};

//struct VectexTexture
//{
//    D3DXVECTOR3 position;
//    D3DXVECTOR2 uv; //Texture Coordinate �ؽ�ó ��ǥ��
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
    //�ؽ�ó ��ǥ���̱� ������ ���峪 �� ���ϰų� �̵���ų �ʿ� ����.
    return output;
}

//�ؽ�ó2D �ڷ������� �ؽ�ó �ڿ��� �޾ƾ� �Ѵ�. cpu�ٷ� �����ϰ� 
//register(b0) : b�� ������ �ڷ� , t�� �ؽ�ó �ڷ� ó������ 0���̱⿡ t0
Texture2D source_texture : register(t0);
SamplerState samp        : register(s0);
//���÷� �ڿ� 
//sampling �����ǵ����͸� �����ϴ°�.

float4 PS(PixelInput input) : SV_Target
{
    float4 color = source_texture.Sample(samp, input.uv);

   //1. clip(color.a - 0.9f);
    //�����Ͱ� 0���� ������ �ش� �����͸� ����Ѵ�.
    
    //2. (1)���� ����. 
    if (color.a < 0.1f)
        discard;
    //���� �������̴�. �̰� ���带 ����ؼ� ���ְڴ� ���������ǿ���
    
    return color;
}
//clip�Լ��� ����� �ִ�. ���İ�
//uv������� �ؽ�ó �ٿ��ð� ���İ��� �����ô�.!!
//discard 
//sampleing sate ��� �̾Ƴ����ΰ�.