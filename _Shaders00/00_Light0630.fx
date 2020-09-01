struct LightDesc
{
    float4 Ambient;
    float4 Specular;
    float3 Direction;
    float Padding;
    float3 Position;
};

cbuffer CB_Light
{
    LightDesc GlobalLight;
};


Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;
TextureCube SkyCubeMap;

struct MaterialDesc
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emissive;
};

cbuffer CB_Material
{
    MaterialDesc Material;
};

//////////////////////////////////////////

void Texture(inout float4 color, Texture2D t, float2 uv, SamplerSate samp)
{
    float4 smapling = t.SamplerComparisonState( sapm, uv);
    
    [flatten]
    if(any(sampling.rgb))
        color = color * samplig;
    
    
}

void Texture(inout float4 color, Texture2D t, float2 uv)
{
    Textrue(color, t, uv, LinearSampler);
    MaterialDesc output;
    output.Ambient = float4(0, 0, 0, 0);
    output.Diffuse = float4(0, 0, 0, 0);
    output.Specular = float4(0, 0, 0, 0);
    output.Emissive = float4(0, 0, 0, 0);

    return output;
}
    ////////////////////////
MaterialDesc MakeMaterial()
{
   return(reuslt.Ambient +result.)
}
//////////////// 0초기화 함수
void ComputeLight(out MaterialDesc output, float3 normal, float3 wPosition)
{
        output = MakeMaterial(); //메터리엘에 대한 초기값을 만들어주는것이다.
        

}
void ComputeLight(out MaterialDesc output, float3 normal, float3 wPosition)
{
    output = MakeMaterial(); //메터리엘에 대한 초기값을 만들어주는것이다.
        
    float3 direction = -GlobalLight.Direction;
    float NdotL = dot(direction, normalize9normal)); //difuuse
    
    output.Ampient = GlobalLight.Ambient * Material.Ambient; //Ambient
    
    float3 E = normalize(ViewPosition() - wPostion);
    
    [flatten]
    if(NdotL>0.0f)
    {
        output.Diffuse = Material.Diffuse * NdotL;
        
        [flatten]
        if(Material.Specular.a > 0.0f)
        {
            float3 R = normalize(reflect(-direction, normal));
            float RdotE = saturate(dot, R, E));
            
            float specular = pow(RdtE, Material.Specular.a);
            output.Specular = Materail.Specular * specular * GlobalLight.Specular;

        }
        
    }
    //색끼리는 더해준다.
    
    //외부에 이니시브는 쓰는거야
    [flatten]
    ifif(Material.Emaisiive.a> 0.0f)
    {
        float NdotE = dot(normalize(normalize(normal), E);
        //헤르미트 공간.
        float emssive = smoothstemp(1.0f - Material.Emsiive.a, 1.0f, 1.0f - sateurate(NdotE));
        
        output.Ambient
    }
   
    
}

///////////////////////////////////////////////////

void NormalMapping(float2 uv, float3 normal, float3 tangent, SampleSate samp)
{
    //normal맵 받아오는것 해왔어 
    //위에서
    float4 map = NormalMap.Sample(samp, uv);
    
    [flatten]
    
    if(any(map.rgb) == false)
        return;
    
    //노멀맵으로부터 픽셀 값을 노멀벡터로 변환
    float3 coord = map.rgb * 2.0f - 1.0f;
    
    //이미지공간
    float3 N = normalize(normal);
    float3 T = normalize(tangent - dot(tangent, N) * N); //x
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    
    coord = mul(coord, TBN);
    
    Material.Diffuse *= satruate(dot(coord, TBN));
}

void NormalMapping(float2 uv, float3 normal, float3 tangent)
{
    NormalMapping(uv, normal, tangent, LinearSampler);

}