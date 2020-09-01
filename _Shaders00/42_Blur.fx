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
//�ֺ��ȼ��� ����̶� �Ѵ�.
uint BlurCount = 8;
float4 PS_Blur(VertexOutput input) : SV_Target
{   
    //�ڱ� ���� �ȼ��� 
    float2 arr[9] =
    {   //uv�� ���ҰŰŵ� �ֺ��ȼ���. 
        //�޻���ȼ�
        float2(-1, -1), float2(+0, -1), float2(+1, -1),
                         //�ڱⰪ �״�� y��
        float2(-1, +0), float2(+0, +0), float2(+1, +0),
        
        float2(-1, +1), float2(+0, +1), float2(+1, +1),
    };
    
    float3 color = 0;
    
    for (uint blur = 0; blur < BlurCount; blur++)
    {   //unroll�� �������ѵ� �ǰڳ�, �Լ��ϱ�
        [unroll(9)]
        for (int i = 0; i < 9; i++) //�迭�� 9���̴�. 
        {   
            //���� �ȼ��� �˼� ����. �״����� ����ŭ(������ ����������) ũ��Ŭ���� ������ ������������. ���ȼ����������.
            float x = arr[i].x * (float) blur * PixelSize.x;
            float y = arr[i].y * (float) blur * PixelSize.y;
            
            //uv�� ����ž�.
            float2 uv = input.Uv + float2(x, y);
            color += DiffuseMap.Sample(LinearSampler, uv).rgb;
        }
    }
    //����̴ϱ� ���Ѹ�ŭ �����߰���. 9�� �ȼ��̴ϱ�. 
    color /= BlurCount * 9;
    
    return float4(color.rgb, 1);
}


uint RadialBlurCount = 8; 
float RadialBlurRadius = 0.0f;
float RadialBlurAmount = 0.04f; //4%
float2 RadialCenter = float2(0.5f, 0.5f); //��� ���� ��������

float4 PS_RadialBlur(VertexOutput input) : SV_Target
{   
    //uv���� ���� �߽��� ��°ž�.
    float2 radius = input.Uv - RadialCenter;
    float r = length(radius) + 1e-6f; //�ణ�� �������� + ��ų�ž�.
    radius /= r; //������ r�� �����ٷ��� radius ������ ���� ��Ấ�� .
    //����ȭ, ������ �ȴ�.
    
    r = 2 * r / RadialBlurRadius; //������ �ɰ��̴�.
    r = saturate(r); //r�� 0~1���� ����߸��°ž�.
    
    //���� * r * r // 2���� r �������� ����. �׸��� ����ī��Ʈ�� ������. Ƚ���ȿ� �������� �����
    //��Ÿ�� ����������� �Ѱž�
    float2 delta = radius * r * r * RadialBlurAmount / RadialBlurCount;
    delta = -delta; //��Ÿ�� �������ž�. uv�� ���� ���� �׷�, ���ǰ����� ������� , uv������ ���ϸ� ������ �ȸ¾�.
    
    float4 color = 0;
    for (uint i = 0; i < RadialBlurCount; i++)
    {
        //��ǻ�� �ʿ� ���Ͼ� ���÷�. ��Ÿ�� �ջ��س����ž�. ���� �ȼ��� �ƴ϶� ���� �ȼ��ΰ��� ��Ÿ��
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