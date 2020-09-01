matrix World, View, Projection;
Texture2D DiffuseMap;
float3 LightDirection;
 
struct VertexInput
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
    
};
struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
   
    output.Position = mul(input.Position,World);
    output.Position = mul(input.Position,View);
    output.Position = mul(input.Position, Projection);
    
    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv; 
    
    
    return output;
}

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 PS(VertexOutput input) : SV_Target
{
    //return DiffuseMap.Sample(LinearSampler, input.Uv);
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
    float3 normal = normalize(input.Normal);
    float NdotL = dot(-LightDirection, normal);
    
    return diffuse;
}
RasterizerState RS
{
    FillMode = Wireframe;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
    
    pass P1
    {
        SetRasterizerState(RS);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
