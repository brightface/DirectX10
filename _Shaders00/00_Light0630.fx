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
//////////////// 0�ʱ�ȭ �Լ�
void ComputeLight(out MaterialDesc output, float3 normal, float3 wPosition)
{
        output = MakeMaterial(); //���͸����� ���� �ʱⰪ�� ������ִ°��̴�.
        

}
void ComputeLight(out MaterialDesc output, float3 normal, float3 wPosition)
{
    output = MakeMaterial(); //���͸����� ���� �ʱⰪ�� ������ִ°��̴�.
        
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
    //�������� �����ش�.
    
    //�ܺο� �̴Ͻú�� ���°ž�
    [flatten]
    ifif(Material.Emaisiive.a> 0.0f)
    {
        float NdotE = dot(normalize(normalize(normal), E);
        //�츣��Ʈ ����.
        float emssive = smoothstemp(1.0f - Material.Emsiive.a, 1.0f, 1.0f - sateurate(NdotE));
        
        output.Ambient
    }
   
    
}

///////////////////////////////////////////////////

void NormalMapping(float2 uv, float3 normal, float3 tangent, SampleSate samp)
{
    //normal�� �޾ƿ��°� �ؿԾ� 
    //������
    float4 map = NormalMap.Sample(samp, uv);
    
    [flatten]
    
    if(any(map.rgb) == false)
        return;
    
    //��ָ����κ��� �ȼ� ���� ��ֺ��ͷ� ��ȯ
    float3 coord = map.rgb * 2.0f - 1.0f;
    
    //�̹�������
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