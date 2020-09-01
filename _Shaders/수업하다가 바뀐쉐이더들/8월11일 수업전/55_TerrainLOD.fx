#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"
#include "00_Terrain.fx"

struct TerrainLODDesc
{
    float2 Distance;
    float2 TessFactor;
    
    float Cellspacing;
    float CellspacingU;
    float CellspacingV;
    float HeightScale;
    
    float4 CullPlane[6];
};

cbuffer CB_TerrainLOD
{
    TerrainLODDesc TerrainLOD;
};


struct VertexInput
{
    float4 Position : Position;
    float2 Uv : Uv;
    float2 Bound : Bound;
};

struct VertexOutput
{
    float4 Position : Position;
    float2 Uv : Uv;
    float2 Bound : Bound;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = input.Position;
    output.Uv = input.Uv;
    output.Bound = input.Bound;

    return output;
}

struct CHullOutput
{
    float Edge[4] : SV_TessFactor;
    float Inside[2] : SV_InsideTessFactor;
};

float CalcTessFactor(float3 position)
{
    float d = distance(position, ViewPosition());
    float s = saturate((d - TerrainLOD.Distance.y) / (TerrainLOD.Distance.x - TerrainLOD.Distance.y));
 
    return lerp(TerrainLOD.TessFactor.x, TerrainLOD.TessFactor.y, s);
}


bool OutFrustumPlane(float3 center, float3 extent, float4 plane)
{
    float3 n = abs(plane.xyz);
    float r = dot(extent, n);
    float s = dot(float4(center, 1), plane);
    
    return (s + r) < 0.0f;
}

bool OutFrustum(float3 center, float3 extent)
{
    [unroll(6)]
    for (int i = 0; i < 6; i++)
    {
        [flatten]
        if (OutFrustumPlane(center, extent, TerrainLOD.CullPlane[i]))
            return true;
    }
    
    return false;
}

CHullOutput CHS(InputPatch<VertexOutput, 4> input, uint indexId : SV_PrimitiveID)
{
    CHullOutput output;
    
    float4 position[4];
    position[0] = WorldPosition(input[0].Position);
    position[1] = WorldPosition(input[1].Position);
    position[2] = WorldPosition(input[2].Position);
    position[3] = WorldPosition(input[3].Position);
    
    
    float minY = input[0].Bound.x;
    float maxY = input[0].Bound.y;
    
    float3 minBox = float3(position[2].x, minY, position[2].z);
    float3 maxBox = float3(position[1].x, maxY, position[1].z);
    
    float3 boxCenter = (minBox + maxBox) * 0.5f;
    float3 boxExtent = abs(maxBox - minBox) * 0.5f;
    
    [flatten]
    if (OutFrustum(boxCenter, boxExtent))
    {
        output.Edge[0] = -1;
        output.Edge[1] = -1;
        output.Edge[2] = -1;
        output.Edge[3] = -1;
    
        output.Inside[0] = -1;
        output.Inside[1] = -1;
        
        return output;
    }
    
    
    
    float3 e0 = (position[0] + position[2]).xyz * 0.5f;
    float3 e1 = (position[0] + position[1]).xyz * 0.5f;
    float3 e2 = (position[1] + position[3]).xyz * 0.5f;
    float3 e3 = (position[2] + position[3]).xyz * 0.5f;
    
    float3 center = (position[0].xyz + position[1].xyz + position[2].xyz + position[3].xyz) * 0.25f;
    
    
    output.Edge[0] = CalcTessFactor(e0);
    output.Edge[1] = CalcTessFactor(e1);
    output.Edge[2] = CalcTessFactor(e2);
    output.Edge[3] = CalcTessFactor(e3);
    
    output.Inside[0] = CalcTessFactor(center);
    output.Inside[1] = CalcTessFactor(center);
    
    return output;
}


struct HullOutput
{
    float4 Position : Position;
    float2 Uv : Uv;
};

[domain("quad")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CHS")]
HullOutput HS(InputPatch<VertexOutput, 4> input, uint pointID : SV_OutputControlPointID)
{
    HullOutput output;
    output.Position = input[pointID].Position;
    output.Uv = input[pointID].Uv;

    return output;
}


struct DomainOutput
{
    float4 Position : SV_Position;
    float3 wPosition : Position1;
    float2 Uv : Uv;
};

[domain("quad")]
DomainOutput DS(CHullOutput input, const OutputPatch<HullOutput, 4> patch, float2 uv : SV_DomainLocation)
{
    DomainOutput output;
    
    float3 v1 = lerp(patch[0].Position.xyz, patch[1].Position.xyz, uv.x);
    float3 v2 = lerp(patch[2].Position.xyz, patch[3].Position.xyz, uv.x);
    float3 position = lerp(v1, v2, uv.y);
    
    float2 texCoord1 = lerp(patch[0].Uv, patch[1].Uv, uv.x);
    float2 texCoord2 = lerp(patch[2].Uv, patch[3].Uv, uv.x);
    float2 texCoord = lerp(texCoord1, texCoord2, uv.y);
    
    
    position.y = HeightMap.SampleLevel(LinearSampler, texCoord, 0).r * 255 / TerrainLOD.HeightScale;
    
    output.Position = WorldPosition(float4(position, 1));
    output.wPosition = output.Position.xyz;
    
    output.Position = ViewProjection(output.Position);
    output.Uv = texCoord;
    
    return output;
}

float4 PS(DomainOutput input) : SV_Target
{
    float2 left = input.Uv + float2(-TerrainLOD.CellspacingU, 0.0f);
    float2 right = input.Uv + float2(+TerrainLOD.CellspacingU, 0.0f);
    float2 top = input.Uv + float2(0.0f, -TerrainLOD.CellspacingV);
    float2 bottom = input.Uv + float2(0.0f, +TerrainLOD.CellspacingV);
    
    float leftY = HeightMap.Sample(PointSampler, left).r * 255 / TerrainLOD.HeightScale;
    float rightY = HeightMap.Sample(PointSampler, right).r * 255 / TerrainLOD.HeightScale;
    float topY = HeightMap.Sample(PointSampler, top).r * 255 / TerrainLOD.HeightScale;
    float bottomY = HeightMap.Sample(PointSampler, bottom).r * 255 / TerrainLOD.HeightScale;
    
    float3 tangent = normalize(float3(TerrainLOD.Cellspacing * 2.0f, rightY - leftY, 0.0f)); //X
    float3 biTangent = normalize(float3(0.0f, bottomY - topY, TerrainLOD.Cellspacing * -2.0f)); //Y
    float3 normal = normalize(cross(tangent, biTangent)); //Z
    
    return float4(normal, 1);
    //return BaseMap.Sample(LinearSampler, input.Uv);
}

technique11 T0
{
    P_VTP(P0, VS, HS, DS, PS)
    P_RS_VTP(P1, FillMode_WireFrame, VS, HS, DS, PS)
}