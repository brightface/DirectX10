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
    ProjectorDesc Projector; //c버퍼로 뷰 프로젝션을 받아줄거야.
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

// 이러면 ndc 공간이 된다. 0~1사이
    
    //saturate 가 0~1까지다. uv도 0~1까지다.
    //카메라 영역에 안들어온 애들, 0~1까지 범위를 넘어서겠지, 개내들 그리지 않겠다는거야
    [flatten]
    if (saturate(uvw.x) == uvw.x && saturate(uvw.y) == uvw.y && saturate(uvw.z) == uvw.z)
    {
        float4 map = ProjectorMap.Sample(LinearSampler, uvw.xy);
        
        map.rgb += Projector.Color.rgb;
        color = lerp(color, map, map.a);
        
    }
        
}

//Render.fx에서 generate 부른다음에 wvpPosition_sub 의 포지션 계산하는 함수
MeshOutput //VS 정의하는거야
VS_Mesh_Projector(VertexMesh input)
{
    MeshOutput output = VS_Mesh(input);  
    //메시 값 계산되잖아. 서브로 넣어서(wvp계산) 그리고 input.Position만 바꿀거야.
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
    //따로 준 공간.
    float4 wvp = input.wvpPosition_Sub;
    
    float4 color = PS_AllLight(input);
    
    uvw.x = wvp.x / wvp.w * 0.5f + 0.5f;
    uvw.y = wvp.y / wvp.w * 0.5f + 0.5f;
    uvw.z = wvp.z / wvp.w;
    //계산된 상태에서 해야한다.
    float4 result = 0;
    
    [flatten] //자르겠다.
    if (saturate(uvw.x) == uvw.x && saturate(uvw.y) == uvw.y && saturate(uvw.z) == uvw.z) //0~1까지만 나오게 자른다.
    {
       
    
    //uvw.xz을 써도 돼 수직으로 비출라고 z도 수직에 따라 uv를 발라주는게 가장 좋아.
        
        float4 map = ProjectorMap.Sample(LinearSampler, uvw.xy);
        
        map.rgb += Projector.Color.rgb;
        color = lerp(color, map, map.a); //맵의 알파가 0이면 불투명이고 알파가 1이면 투명이다. 그러면 적절히 섞일것이다.
        
        return map;
    }
    
   
  
        
    float4 color = PS_AllLight(input);
    //PS_Projector(color, input.wvpPosition_Sub); //프로젝터에다가 뷰 프로젝션을 적용시킨것.

    return color;
}


technique11 T0
{
    P_VP(P0, VS_Mesh_Projecteor, PS)
    P_VP(P1, VS_Model_Projector, PS)
    P_VP(P2, VS_Animation_Projector, PS)
}