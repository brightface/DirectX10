#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"
#include "00_Terrain.fx"
#include "00_Sky.fx"

float4 PS(MeshOutput input) : SV_Target
{
    return CalcualteFogColor(PS_Shadow(input.sPosition, PS_AllLight(input)), input.wPosition);
}

//반사용
MeshOutput VS_Draw_Mesh(VertexMesh input)
{
    MeshOutput output = VS_Mesh(input);
    //반사 받을놈
    float4 position = WorldPosition(input.Position); //반사 공간이 달라야하니까
    position = mul(position, ReflectionView);
    position = mul(position, Projection);
    
    output.wvpPosition_Sub = position;
    
    return output;
}

struct WaterDesc
{
    float4 RefractionColor; //굴절색

    float2 NormalMapTile; //굴절 잘게 쪼개맂
    float WaveTranslation;//물결의 이동 
    float WaveScale; //굴절할떄 ,x,y 물결 출렁정도

    float WaterShininess; //스페큘러,, 얼마나 뽀샤시 
    float WaterAlpha;//깊이에 따라 투명도
    float WaveSpeed; //파도에 속도
    float WaterPositionY;//물의 높이
    
    float TerrainWidth;
    float TerrainHeight;
    float WaterWidth;
    float WaterHeight;
};
Texture2D WaterMap;

cbuffer CB_Water
{
    WaterDesc Water;
};

struct VertexOutput_Water
{
    float4 Position : SV_Position;//반사용
    float3 wPosition : Position1;//굴절용
    float4 ReflectionPosition : Position2;
    float4 RefractionPosition : Position3;
    
    float2 Uv : Uv;
    
    float2 Normal : Uv1;
    float2 Normal2 : Uv2;
};

VertexOutput_Water VS_Water(VertexTexture input)
{
    VertexOutput_Water output;
    
    output.Position = WorldPosition(input.Position);
    output.wPosition = output.Position.xyz;
    
    output.Position = ViewProjection(output.Position); //WVP
    
    
    output.ReflectionPosition = WorldPosition(input.Position);
    output.ReflectionPosition = mul(output.ReflectionPosition, ReflectionView);//반사용으로 따자나
    output.ReflectionPosition = mul(output.ReflectionPosition, Projection);//굴절은 동일하니까
    
    output.RefractionPosition = output.Position;
    
    output.Uv = input.Uv;
    
    output.Normal = input.Uv * Water.NormalMapTile.x;
    output.Normal2 = input.Uv * Water.NormalMapTile.y;
    
    return output;
}

float4 PS_Water(VertexOutput_Cloud_Water input) :SV_Position //SV_Target
{
    input.Normal.y += Water.WaveTranslation;
    input.Normal2.x += Water.WaveTranslation;
    //노멀맵 샘플링 할려고 다르게 한거야. 최대한 물결처럼 보이게 할려고
    float4 normalMap = NormalMap.Sample(LinearSampler, input.Normal) * 2.0f - 1.0f;
    float4 normalMap2 = NormalMap.Sample(LinearSampler, input.Normal2) * 2.0f - 1.0f;
    float3 normal = normalMap.rgb + normalMap2.rgb;
    //노멀맵 하나 따서 반사와 굴절 잘 나오는지 보고 가자.
    reflection += normal.xy * Water.WaveScale;
    reflection += normal.xy * Water.WaveScale; //파도가 나오겠지. 이러면 다양한 조합 만든거야.
    refletion.x = input.ReflectionPosition.x /input.ReflectionPosition.w*0.5f+ 0
    refletion.y = input.ReflectionPosition.x / input.ReflectionPosition.w * 0.5f+ + 0
    return 
    
    float3 light = GlobalLight.Direction;
    light.yz += -1.0f;
    
    flaot3view = normalize(ViewPosition() - input.wPosition);
    float3 heightView = View.yyy
    
    floatf = (1.2f - 1.0f) / (1.2f / 1.0f);
    float fresnel = saturate(min(1, radians + (1 - r) * pow(1 - dot(normal, heightView).2)));//플레즈널 방정식
    float4 diffuse = lerp(reflectionColor, refractionColor, fresnel);
    
    return float4(diffuse.rgb, 1.0f); // 먼가를 가기전에 또한번 출력해주면 좋겠지.
    

}

technique11 T0
{
    //1Pass - Depth
    P_RS_VP(P0, FrontCounterClockwise_True, VS_Mesh_Depth, PS_Shadow_Depth)
    P_RS_VP(P1, FrontCounterClockwise_True, VS_Model_Depth, PS_Shadow_Depth)
    P_RS_VP(P2, FrontCounterClockwise_True, VS_Animation_Depth, PS_Shadow_Depth)

    //2Pass - Shadow
    P_VP(P3, VS_Mesh, PS)
    P_VP(P4, VS_Model, PS)
    P_VP(P5, VS_Animation, PS)
    P_VP(P6, VS_Terrain, PS_Terrain)

    //2Pass - Sky
    P_VP(P7, VS_Scattering, PS_Scattering)
    P_DSS_VP(P8, DepthEnable_False, VS_Dome, PS_Dome)
    P_DSS_BS_VP(P9, DepthEnable_False, AlphaBlend, VS_Moon, PS_Moon)
    P_DSS_BS_VP(P10, DepthEnable_False, AdditiveBlend, VS_Cloud, PS_Cloud)

    
    //1Pass - Reflection
    P_VP(P11, VS_Mesh_Reflection, PS)
    P_VP(P12, VS_Model_Reflection, PS)
    P_VP(P13, VS_Animation_Reflection, PS)
    P_VP(P14, VS_Terrain_Reflection, PS_Terrain)

    //1Pass - Reflection Sky
    P_VP(P15, VS_Scattering, PS_Scattering)
    P_DSS_VP(P16, DepthEnable_False, VS_Dome_Reflection, PS_Dome)
    P_DSS_BS_VP(P17, DepthEnable_False, AlphaBlend, VS_Moon_Reflection, PS_Moon)
    P_DSS_BS_VP(P18, DepthEnable_False, AdditiveBlend, VS_Cloud_Reflection, PS_Cloud)

    //Pass - Reflection Mesh -메시만이야 모델 반사시키고 싶으면 모델 똑같이 넣으면돼
    P_BS_VP(P19, AlphaBlend, VS_Water, PS_Water)

}