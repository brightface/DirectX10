Texture2D BaseMap;

Texture2D Layer1AlphaMap;
Texture2D Layer1ColorMap;

Texture2D HeightMap;


struct VertexTerrain
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
    float3 Tangent : Tangent;
    float4 Color : Color;
};

///////////////////////////////////////////////////////////////////////////////

struct LineDesc
{
    float4 Color;
    uint Visible;
    float Thickness;
    float Size;
};

cbuffer CB_TerrainLine
{
    LineDesc TerrainLine;
};

//float4 GetLineColor(float3 wPosition)
//{
//    [flatten]
//    if(TerrainLine.Visible < 1)
//        return float4(0, 0, 0, 0);
    
//    float2 grid = wPosition.xz / TerrainLine.Size;
//    grid = frac(grid);
    
//    float thick = TerrainLine.Thickness / TerrainLine.Size;
    
//    [flatten]
//    if(grid.x < thick || grid.y < thick)
//        return TerrainLine.Color;
    
//    return float4(0, 0, 0, 0);
//}

float4 GetLineColor(float3 wPosition)
{
    [flatten]
    if (TerrainLine.Visible < 1)
        return float4(0, 0, 0, 0);
    
    float2 grid = wPosition.xz / TerrainLine.Size;
    float2 range = abs(frac(grid - 0.5f) - 0.5f);
    //return float4(range, 0, 1);
    
    float2 speed = fwidth(grid);
    //speed *= 5.0f;
    //return float4(speed, 0, 1);
    
    float2 pixel = range / speed;
    //return float4(pixel, 0, 1);
    
    float thick = saturate(min(pixel.x, pixel.y) - TerrainLine.Thickness);
    //return float4(thick, 0, 0, 1);
    
    return lerp(TerrainLine.Color, float4(0, 0, 0, 0), thick);
}


struct BrushDesc
{
    float4 Color;
    float3 Location;
    uint Type;
    uint Range;
};

cbuffer CB_TerrainBrush
{
    BrushDesc TerrainBrush;
};

float4 GetBrushColor(float3 wPosition)
{    
    [flatten]
    if (TerrainBrush.Type == 1)
    {
        [flatten]
        if ((wPosition.x >= (TerrainBrush.Location.x - TerrainBrush.Range)) &&
            (wPosition.x <= (TerrainBrush.Location.x + TerrainBrush.Range)) &&
            (wPosition.z >= (TerrainBrush.Location.z - TerrainBrush.Range)) &&
            (wPosition.z <= (TerrainBrush.Location.z + TerrainBrush.Range)))
        {
            return TerrainBrush.Color;
        }
    }
    
    [flatten]
    if (TerrainBrush.Type == 2)
    {
        float dx = wPosition.x - TerrainBrush.Location.x;
        float dz = wPosition.z - TerrainBrush.Location.z;
        float dist = sqrt(dx * dx + dz * dz);
        
        [flatten]
        if (dist <= (float) TerrainBrush.Range)
            return TerrainBrush.Color;
    }
    
    return float4(0, 0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////

MeshOutput VS_Terrain(VertexTerrain input)
{
    MeshOutput output = (MeshOutput)0;
    
    VS_GENERATE
    
    //output.Color = GetBrushColor(output.wPosition);
    
    return output;
}

MeshDepthOutput VS_Terrain_Depth(VertexTerrain input)
{
    MeshDepthOutput output;
    
    VS_DEPTH_GENERATE
    
    return output;
}

MeshOutput VS_Terrain_Projector(VertexTerrain input)
{
    MeshOutput output = VS_Terrain(input);
    VS_Projector(output.wvpPosition_Sub, input.Position);
    
    return output;
}

MeshOutput VS_Terrain_Reflection(VertexTerrain input)
{
    MeshOutput output = (MeshOutput) 0;
    
    VS_REFLECTION_GENERATE
    
    return output;
}

float4 PS_Terrain(MeshOutput input) : SV_Target
{
    float4 color = BaseMap.Sample(LinearSampler, input.Uv);;
    
    float alphaMap = Layer1AlphaMap.Sample(LinearSampler, input.Uv).r;
    float colorMap = Layer1ColorMap.Sample(LinearSampler, input.Uv).r;
    if (alphaMap > 0.0f)
        color = lerp(color, colorMap, alphaMap);
        
    
    Material.Diffuse = color;
    //DiffuseMap = BaseMap;
    
    color = PS_Shadow(input.sPosition, PS_AllLight(input));
    
    color += GetLineColor(input.wPosition);
    //color = GetLineColor(input.wPosition);
    
    color += GetBrushColor(input.wPosition);
    
    return CalcualteFogColor(color, input.wPosition);
}