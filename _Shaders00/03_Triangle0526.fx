struct VertexInput
{
    float4 Position : Position;
};

struct VertexOutput
{
    float4 Position : SV_Position;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = input.Position;
    
    return output;
}

float4 Color = float4(0, 1, 0, 1);
float Red;
float4 PS(VertexOutput input) : SV_Target
{
    //return float4(1, 0, 0, 1);
    return Color;
    //return float4(Red, 0, 0, 1);
}

///////////////////////////////////////////////////////////////////////////////

struct VertexInput2
{
    float4 Position : Position;
    float4 Color : Color;
};

struct VertexOutput2
{
    float4 Position : SV_Position;
    float4 Color : Color;
};

VertexOutput2 VS2(VertexInput2 input)
{
    VertexOutput2 output;
    output.Position = input.Position;
    output.Color = input.Color;
    
    return output;
}

float4 PS2(VertexOutput2 input) : SV_Target
{
    return input.Color;
}

///////////////////////////////////////////////////////////////////////////////

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS2()));
        SetPixelShader(CompileShader(ps_5_0, PS2()));
    }
}