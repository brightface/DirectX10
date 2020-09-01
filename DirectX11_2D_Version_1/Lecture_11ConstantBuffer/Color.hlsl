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

cbuffer TransformBuffer : register(b0) // ->  0 ~ 13 // 4096(최대크기) * 16byte 맞춰야돼 
{   //c버퍼는 4*4 바이트를 맞춰주어야한다.
    matrix world;
    matrix view;
    matrix proj;
    //쉐이더 에서 쓰이는 행렬계산이 월등히 빠르다. 따라서 전치행렬을 쓴다. 
    
    //row_major matrix world;
    //row_major matrix view;
    //row_major matrix proj; //열우선이지만 이것 안쓰고 위에것 쓰겠다. 빠르니까
};

//vs에서 사용해야돼 들어온것을 어떻게하냐?
//이동시킬려면 행렬을 곱하면 된다.


PixelInput VS(VertexInput input)
{
    // 1 x 4            4 x 4  -----> 1 x 4
    //1, 2, 3, 1   *    0000
    //                  0000
    //                  0000
    //                  0000

    PixelInput output; //바꿀수가 있다.
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj); //CLIPPING SPACE 가 포함되어 있다.

    output.color = input.color;

    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}