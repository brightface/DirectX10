#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : BINORMAL[n] SV_Target{
return 
}

struct VectexInput
{
    float4 Postion = Postion;
    float2 Scale : Scale;
};

struct VertexOutput
{
    float4 Position : Position;
    float2 Sclae : Scale;
};
struct VertexOutput_Billboard
{
    float4 Position : Position;
    float2 Sclae : Scale;
};
VertexOutput_Billboard VS_Billboard(VertexInput inpput){

    VertexOutput_BillBoard output;
    
    output.Position = WorldPosition(input.Position);
    output.Scale = input.Scale;
    
    return output;
};


struct GeometryOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
};

[maxvertexcount(4)] // �ѹ��� ��� ���ý� ���������ΰ�
void GS_Billboard(point VertexOutput_Billboard Input[1], inout TriangleStream<GeometryOutput> stream)
{
float3 up = float3(0, 1, 0);
//float3 forward = float3(0, 0, 1);
    float3 foward = Input[0].Position.xyz - ViewPosition();
    //�����带 ���ڰ�������� ������ . 8�����̳� ���ڰ� ������� ����ž�.

    float3 right = normalize(cross(up, forward));

float2 size = input[0].Scale = 0.5f;

float4 position[4];
//getmety ������ �⺻ Ʈ���̾ٱ� ��Ƽ���̾�
    position[0] = float4(Input[0].position.xyz - size.x * right - size.y* up, 1);//���ϴ� ����
    position[1] = float4(Input[0].position.xyz - size.x * right + size.y * up, 1); //���ϴ� ����
    position[2] = float4(Input[0].position.xyz + size.x * right - size.y * up, 1); //���ϴ� ����
    position[3] = float4(Input[0].position.xyz + size.x * right + size.y * up, 1); //���ϴ� ����


//

float2 uv[4] = { float2(0, 1), float2(0, 0), float2(1, 1), float2(1, 0) };

GeometryOutput output;

[unroll(4)]
for(int i = 0;i<4; i++)
{
    output.Position = ViewProjection[position[i]];
    output.Uv=uv[i];
    stream.Append(output);
}

//�ϳ��� �����ž�. 3���� ��ƹ޴°� ���ߤ���
}

[maxvertexcount(8)]
void GS_Cross(point VertexOutput_Billboard Input[1], inout TriangleStream<GeometryOutput> stream)
{
    float3 up = float3(0, 1, 0);
//float3 forward = float3(0, 0, 1);
    float3 foward = Input[0].Position.xyz - ViewPosition();
    //�����带 ���ڰ�������� ������ . 8�����̳� ���ڰ� ������� ����ž�.

    float3 right = normalize(cross(up, forward));

    float2 size = input[0].Scale = 0.5f;

    float4 position[8];
//getmety ������ �⺻ Ʈ���̾ٱ� ��Ƽ���̾�
    position[0] = float4(Input[0].position.xyz - size.x * right - size.y * up, 1); //���ϴ� ����
    position[1] = float4(Input[0].position.xyz - size.x * right + size.y * up, 1); //���ϴ� ����
    position[2] = float4(Input[0].position.xyz + size.x * right - size.y * up, 1); //���ϴ� ����
    position[3] = float4(Input[0].position.xyz + size.x * right + size.y * up, 1); //���ϴ� ����
    position[4] = float4(Input[0].position.xyz - size.x * forward - size.y * up, 1); //���ϴ� ����
    position[5] = float4(Input[0].position.xyz - size.x * forward + size.y * up, 1); //���ϴ� ����
    position[6] = float4(Input[0].position.xyz + size.x * forward - size.y * up, 1); //���ϴ� ����
    position[7] = float4(Input[0].position.xyz + size.x * forward + size.y * up, 1); //���ϴ� ����



//

    float2 uv[4] = { float2(0, 1), float2(0, 0), float2(1, 1), float2(1, 0) };

    GeometryOutput output;

[unroll(8)]
    for (int i = 0; i < 4; i++)
    {
        output.Position = ViewProjection[position[i]];
        output.Uv = uv[i%4];
        stream.Append(output);
    
        [flatten]
        if(i==3)
            stream.RestartStrip();//strip�� �ٽý����ض� ��������
        
    }

//�ϳ��� �����ž�. 3���� ��ƹ޴°� ���ߤ���
}
float4 PS(VertexOutput input) : SV_Target
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler);
    if(diffuse.a <0.3)
        discard; // �Ƚ����̴��� ����Ҽ��ִ�. discard
    
    clip(diffuse.a - 0, 3f); // 0 �����ϋ��� ���� �߶󳻰ڴ�.
    return diffuse;
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
    P_VP(P1, VS_Model, PS)
    P_VP(P2, VS_Animation, PS)
    
    P_BS_VGP(P3, VS_Billboard, GS_Billboard, PS_Billboard)
    P_RS_BS_VGP(P4, CullMode_None,AlpahBlend,VS_Billboard, GS_Billboard, PS_Billboard)
    
}
