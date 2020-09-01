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

cbuffer TransformBuffer : register(b0) // ->  0 ~ 13 // 4096(�ִ�ũ��) * 16byte ����ߵ� 
{   //c���۴� 4*4 ����Ʈ�� �����־���Ѵ�.
    matrix world;
    matrix view;
    matrix proj;
    //���̴� ���� ���̴� ��İ���� ������ ������. ���� ��ġ����� ����. 
    
    //row_major matrix world;
    //row_major matrix view;
    //row_major matrix proj; //���켱������ �̰� �Ⱦ��� ������ ���ڴ�. �����ϱ�
};

//vs���� ����ؾߵ� ���°��� ����ϳ�?
//�̵���ų���� ����� ���ϸ� �ȴ�.


PixelInput VS(VertexInput input)
{
    // 1 x 4            4 x 4  -----> 1 x 4
    //1, 2, 3, 1   *    0000
    //                  0000
    //                  0000
    //                  0000

    PixelInput output; //�ٲܼ��� �ִ�.
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj); //CLIPPING SPACE �� ���ԵǾ� �ִ�.

    output.color = input.color;

    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}