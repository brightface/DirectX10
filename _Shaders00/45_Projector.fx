#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"


Texture2D ProjectorMap;
struct ProjectorDesc
{
    float4 Color;
    matrix View;
    matrix Projection;
};

cbuffer CB_Projector
{
    ProjectorDesc Projector; //c���۷� �� ���������� �޾��ٰž�.
};

void VS_Projector(inout float4 wvp, float4 position)
{
    wvp = WorldPosition(position);
    wvp = mul(wvp, Projector.View);
    wvp = mul(wvp, Projector.Projection);
}

void PS_Projector(inout float4 color, float4 wvp)
{
    float3 uvw = 0;
    
    uvw.x = wvp.x / wvp.w * 0.5f + 0.5f;
    uvw.y = -wvp.y / wvp.w * 0.5f + 0.5f;
    uvw.z = wvp.z / wvp.w * 0.5f + 0.5f;

// �̷��� ndc ������ �ȴ�. 0~1����
    
    //saturate �� 0~1������. uv�� 0~1������.
    //ī�޶� ������ �ȵ��� �ֵ�, 0~1���� ������ �Ѿ����, ������ �׸��� �ʰڴٴ°ž�
    [flatten]
    if (saturate(uvw.x) == uvw.x && saturate(uvw.y) == uvw.y && saturate(uvw.z) == uvw.z)
    {
        float4 map = ProjectorMap.Sample(LinearSampler, uvw.xy);
        
        map.rgb += Projector.Color.rgb;
        color = lerp(color, map, map.a);
        
    }
        
}

//Render.fx���� generate �θ������� wvpPosition_sub �� ������ ����ϴ� �Լ�
MeshOutput //VS �����ϴ°ž�
VS_Mesh_Projector(VertexMesh input)
{
    MeshOutput output = VS_Mesh(input);  
    //�޽� �� �����ݾ�. ����� �־(wvp���) �׸��� input.Position�� �ٲܰž�.
    VS_Projector(output.wvpPosition_Sub, input.Position);
    
    return output;
}

MeshOutput VS_Model_Projector(VertexModel input)
{
    MeshOutput output = VS_Model(input);
    VS_Projector(output.wvpPosition_Sub, input.Position);
    
    return output;
}

MeshOutput VS_Animation_Projector(VertexModel input)
{
    MeshOutput output = VS_Animation(input);
    VS_Projector(output.wvpPosition_Sub, input.Position);
    
    return output;
}

float4 PS(MeshOutput input) : SV_Target
{
    float3 uvw = 0;
    //���� �� ����.
    float4 wvp = input.wvpPosition_Sub;
    
    float4 color = PS_AllLight(input);
    
    uvw.x = wvp.x / wvp.w * 0.5f + 0.5f;
    uvw.y = wvp.y / wvp.w * 0.5f + 0.5f;
    uvw.z = wvp.z / wvp.w;
    //���� ���¿��� �ؾ��Ѵ�.
    float4 result = 0;
    
    [flatten] //�ڸ��ڴ�.
    if (saturate(uvw.x) == uvw.x && saturate(uvw.y) == uvw.y && saturate(uvw.z) == uvw.z) //0~1������ ������ �ڸ���.
    {
       
    
    //uvw.xz�� �ᵵ �� �������� ������ z�� ������ ���� uv�� �߶��ִ°� ���� ����.
        
        float4 map = ProjectorMap.Sample(LinearSampler, uvw.xy);
        
        map.rgb += Projector.Color.rgb;
        color = lerp(color, map, map.a); //���� ���İ� 0�̸� �������̰� ���İ� 1�̸� �����̴�. �׷��� ������ ���ϰ��̴�.
        
        return map;
    }
    
   
  
        
    float4 color = PS_AllLight(input);
    //PS_Projector(color, input.wvpPosition_Sub); //�������Ϳ��ٰ� �� ���������� �����Ų��.

    return color;
}


technique11 T0
{
    P_VP(P0, VS_Mesh_Projecteor, PS)
    P_VP(P1, VS_Model_Projector, PS)
    P_VP(P2, VS_Animation_Projector, PS)
}