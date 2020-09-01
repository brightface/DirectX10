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
Texture2D ProjectionMap;
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

///////////////////////////////////////////////////////////////////////////////

void Texture(inout float4 color, Texture2D t, float2 uv, SamplerState samp)
{
    float4 sampling = t.Sample(samp, uv);
    
    [flatten]
    if(any(sampling.rgb))
        color = color * sampling;
}

void Texture(inout float4 color, Texture2D t, float2 uv)
{
    Texture(color, t, uv, LinearSampler);
}

///////////////////////////////////////////////////////////////////////////////

MaterialDesc MakeMaterial()
{
    MaterialDesc output;
    output.Ambient = float4(0, 0, 0, 0);
    output.Diffuse = float4(0, 0, 0, 0);
    output.Specular = float4(0, 0, 0, 0);
    output.Emissive = float4(0, 0, 0, 0);

    return output;
}

float3 MaterialToColor(MaterialDesc result)
{
    return (result.Ambient + result.Diffuse + result.Specular + result.Emissive).rgb;
}

void AddMaterial(inout MaterialDesc result, MaterialDesc val)
{
    result.Ambient += val.Ambient;
    result.Diffuse += val.Diffuse;
    result.Specular += val.Specular;
    result.Emissive += val.Emissive;
}

///////////////////////////////////////////////////////////////////////////////

void ComputeLight(out MaterialDesc output, float3 normal, float3 wPosition)
{
    output = MakeMaterial();

    float3 direction = -GlobalLight.Direction; //�̰� light�� ���°��̰�
    float NdotL = dot(direction, normalize(normal)); //Diffuse
    
    output.Ambient = GlobalLight.Ambient * Material.Ambient; //Ambient
    float3 E = normalize(ViewPosition() - wPosition);
    
    [flatten]
    if(NdotL > 0.0f)
    {
        output.Diffuse = Material.Diffuse * NdotL;
        
        [flatten]
        if(Material.Specular.a > 0.0f)
        {
            float3 R = normalize(reflect(-direction, normal));
            float RdotE = saturate(dot(R, E));
            
            float specular = pow(RdotE, Material.Specular.a);
            output.Specular = Material.Specular * specular * GlobalLight.Specular;
        }
    }
    
    [flatten]
    if(Material.Emissive.a > 0.0f)
    {
        float NdotE = dot(normalize(normal), E);
        float emissive = smoothstep(1.0f - Material.Emissive.a, 1.0f, 1.0f - saturate(NdotE));
        
        output.Emissive = Material.Emissive * emissive;
    }
}

///////////////////////////////////////////////////////////////////////////////

void NormalMapping(float2 uv, float3 normal, float3 tangent, SamplerState samp)
{
    float4 map = NormalMap.Sample(samp, uv);
    
    [flatten]
    if(any(map.rgb) == false)
        return;
    
    
    //��ָ����κ��� �ȼ� ���� ��ֺ��ͷ� ��ȯ
    float3 coord = map.rgb * 2.0f - 1.0f; //-1 ~ +1
    
    //ź��Ʈ(�̹���) ���� ����
    float3 N = normalize(normal); //Z
    float3 T = normalize(tangent - dot(tangent, N) * N); //X
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    
    coord = mul(coord, TBN); //ź��Ʈ(�̹���) ���� -> 3D ����
    
    Material.Diffuse *= saturate(dot(coord, -GlobalLight.Direction));
}

void NormalMapping(float2 uv, float3 normal, float3 tangent)
{
    NormalMapping(uv, normal, tangent, LinearSampler);
}

///////////////////////////////////////////////////////////////////////////////

#define MAX_POINT_LIGHTS 256
struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emissive;

    float3 Position;
    float Range;

    float Intensity;
    //���� �е� �Ⱦ��ݾ� C���۸� �迭�� �����ž� ���ؼ� �ѱ�ž�.
    float3 Padding;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emissive;

    float3 Position;
    float Range;

    float3 Direction;//��������ġ
    float Angle;//����
    
    float Intensity;
    //���� �е� �Ⱦ��ݾ� C���۸� �迭�� �����ž� ���ؼ� �ѱ�ž�.
    float3 Padding;
};
cbuffer CB_PointLights
{
    uint PointLightCount;
    float3 CB_PointLights_Padding;
    //�е��� ������ �������� ������ ��������
    PointLight PointLights[MAX_POINT_LIGHTS];
};
cbuffer CB_SpotLights
{
    uint SpotLightCount;
    float3 CB_SpotLights_Padding;
    //�е��� ������ �������� ������ ��������
    PointLight SpotLights[MAX_POINT_LIGHTS];
};

//����Ʈ ����Ʈ ���ξ� - å������ �����Ͷ���Ʈ�� �ϳ�����, �ѤѤ� ��ں� �ϳ���.. �ٵ� ������ ���� �־�.;;
void ComputePointLight(inout MaterialDesc output, float3 normal, float3 wPosition)
{
    output = MakeMaterial(); //�ʱ�ȭ �ϳ� �س���
    MaterialDesc result = MakeMaterial(); //�ջ��� ��.
    
    
    for (uint i = 0; i < PointLightCount; i++)
    {
        //DP * ������ ���� 
        float3 light = PointLights[i].Position - wPosition;
        float dist = length(light);
        
        [flatten]
        if (dist > PointLights[i].Range)
            continue;
        
        //light�� �Ÿ��� �����ϱ� ���������̴�.
        light /= dist;
        
        
        float NdotL = dot(light, normalize(normal)); //Diffuse
        //�������� ���Ѵ�����.
        result.Ambient = Material.Ambient * PointLights[i].Ambient;
        float3 E = normalize(ViewPosition() - wPosition);
    
        [flatten]
        if (NdotL > 0.0f)
        {
            result.Diffuse = Material.Diffuse * PointLights[i].Diffuse * NdotL;
        
            [flatten]
            if (Material.Specular.a > 0.0f)
            {
                float3 R = normalize(reflect(-light, normal));
                float RdotE = saturate(dot(R, E));
            
                float specular = pow(RdotE, Material.Specular.a);
                result.Specular = Material.Specular * specular * PointLights[i].Specular;
            }
        }
    
        [flatten]
        if (Material.Emissive.a > 0.0f)
        {
            float NdotE = dot(normalize(normal), E);
            float emissive = smoothstep(1.0f - Material.Emissive.a, 1.0f, 1.0f - saturate(NdotE));
        
            result.Emissive = Material.Emissive * PointLights[i].Emissive * emissive;
        }
        
        
        float temp = 1.0f / saturate(dist / PointLights[i].Range);
        //float att = temp * temp * PointLights[i].Intensity;
        //�ؿ����� �� ���� ���´�. ����.
        float att = temp * temp * (1.0f / max(1.0f - PointLights[i].Intensity, 1e-8f));
        
        output.Ambient += result.Ambient * temp;
        output.Diffuse += result.Diffuse * att;
        output.Specular += result.Specular * att;
        output.Emissive += result.Emissive * att;
    }
}
void ComputeSpotLight(inout MaterialDesc output, float3 normal, float3 wPosition)
{
    output = MakeMaterial(); //�ʱ�ȭ �ϳ� �س���
    MaterialDesc result = MakeMaterial(); //�ջ��� ��.
    
    
    for (uint i = 0; i < SpotLightCount; i++)
    {
        //DP * ������ ���� 
        float3 light = SpotLights[i].Position - wPosition;
        float dist = length(light);
        
        [flatten] //���ȿ� ������ �ȵ��� ���Ť��� �� �߶󳻹��ȴ�.
        if(dist > SpotLights[i].Range)
            continue;
        
        //light�� �Ÿ��� �����ϱ� ���������̴�.
        light /= dist;
        
        
        float NdotL = dot(light, normalize(normal)); //Diffuse
        //�������� ���Ѵ�����.
        result.Ambient = Material.Ambient * SpotLights[i].Ambient;
        float3 E = normalize(ViewPosition() - wPosition);
    
        [flatten]
        if (NdotL > 0.0f)
        {
            result.Diffuse = Material.Diffuse * PointLights[i].Diffuse * NdotL;
        
            [flatten]
            if (Material.Specular.a > 0.0f)
            {
                float3 R = normalize(reflect(-light, normal));
                float RdotE = saturate(dot(R, E));
            
                float specular = pow(RdotE, Material.Specular.a);
                result.Specular = Material.Specular * specular * PointLights[i].Specular;
            }
        }
    
        [flatten]
        if (Material.Emissive.a > 0.0f)
        {
            float NdotE = dot(normalize(normal), E);
            float emissive = smoothstep(1.0f - Material.Emissive.a, 1.0f, 1.0f - saturate(NdotE));
        
            result.Emissive = Material.Emissive * SpotLights[i].Emissive * emissive;
        }
        
        
        float temp = saturate(dist / PointLights[i].Range);
        //float att = temp * temp * PointLights[i].Intensity;
        //�ؿ����� �� ���� ���´�. ����.
        float att = temp * temp * (1.0f / max(1.0f - PointLights[i].Intensity, 1e-8f));
        
        output.Ambient += result.Ambient * temp;
        output.Diffuse += result.Diffuse * att;
        output.Specular += result.Specular * att;
        output.Emissive += result.Emissive * att;
    }
}

///////////////////////////////////////////////////////
float4 PS_AllLight(MeshOutput input)
{
    
    //���� ����
    //�޽� �� �ִϸ��̼� ���� ���� ���� �ȴ�.
}

/////////////////////////////////////////////////////////
struct ProjectorDesc
{
    float4 Color;
    matrix View;
    matrix Projection;
};

cbuffer CB_Projector
{
    ProjectorDesc Projector;
}; //����ü �Ȱ��� c���� ������

void VS_Projector(inout float4 wvp, float4 oPosition)
{
    wvp = WorldPosition(oPosition);
    wvp = mul(wvp, Projector.View);//main camera�� veiw 
    wvp = mul(wvp, Projector.Projection);

} 

//void PS_Projector(inout float4 color, float4 wvp)
//{
//float3 uvw = 0;
    
//    uvw.x = wvp.x / wvp.w * 0.5f + 0.5f;
//    uvw.y = -wvp.y / wvp.w * 0.5f + 0.5f;
//    uvw.z = wvp.z / wvp.w * 0.5f + 0.5f;

//// �̷��� ndc ������ �ȴ�. 0~1����
    
//    //saturate �� 0~1������. uv�� 0~1������.
//    //ī�޶� ������ �ȵ��� �ֵ�, 0~1���� ������ �Ѿ����, ������ �׸��� �ʰڴٴ°ž�
//    [flatten]
//    if (saturate(uvw.x) == uvw.x && saturate(uvw.y) == uvw.y && saturate(uvw.z) == uvw.z)
//    {
//        float4 map = ProjectorMap.Sample(LinearSampler, uvw.xy);
        
//        map.rgb += Projector.Color.
//rgb;
//        color = lerp(color, map, map.a);
        
//    }
        
//}
//////////////////////////////////////////
float4 PS_Sky(MeshOutput input) : SV_Target
{
    return SkyCubeMap.Sample(LinearSampler, input.oPosition);
}