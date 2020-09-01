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

    float3 direction = -GlobalLight.Direction; //이건 light로 쓰는것이고
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
    
    
    //노멀맵으로부터 픽셀 값을 노멀벡터로 변환
    float3 coord = map.rgb * 2.0f - 1.0f; //-1 ~ +1
    
    //탄젠트(이미지) 공간 생성
    float3 N = normalize(normal); //Z
    float3 T = normalize(tangent - dot(tangent, N) * N); //X
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    
    coord = mul(coord, TBN); //탄젠트(이미지) 공간 -> 3D 공간
    
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
    //원래 패딩 안쓰잖아 C버퍼를 배열로 받을거야 통해서 넘길거야.
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

    float3 Direction;//시작할위치
    float Angle;//범위
    
    float Intensity;
    //원래 패딩 안쓰잖아 C버퍼를 배열로 받을거야 통해서 넘길거야.
    float3 Padding;
};
cbuffer CB_PointLights
{
    uint PointLightCount;
    float3 CB_PointLights_Padding;
    //패딩이 없으면 다음것이 땡겨져 버리겠지
    PointLight PointLights[MAX_POINT_LIGHTS];
};
cbuffer CB_SpotLights
{
    uint SpotLightCount;
    float3 CB_SpotLights_Padding;
    //패딩이 없으면 다음것이 땡겨져 버리겠지
    PointLight SpotLights[MAX_POINT_LIGHTS];
};

//포인트 라이트 전부야 - 책에서는 포인터라이트를 하나만써, ㅡㅡㅋ 모닥불 하나야.. 근데 여러개 쓸수 있어.;;
void ComputePointLight(inout MaterialDesc output, float3 normal, float3 wPosition)
{
    output = MakeMaterial(); //초기화 하나 해놓고
    MaterialDesc result = MakeMaterial(); //합산할 값.
    
    
    for (uint i = 0; i < PointLightCount; i++)
    {
        //DP * 조명의 개수 
        float3 light = PointLights[i].Position - wPosition;
        float dist = length(light);
        
        [flatten]
        if (dist > PointLights[i].Range)
            continue;
        
        //light를 거리로 나누니까 단위벡터이다.
        light /= dist;
        
        
        float NdotL = dot(light, normalize(normal)); //Diffuse
        //같은색은 곱한다했지.
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
        //밑에것이 더 많이 나온다. 빛이.
        float att = temp * temp * (1.0f / max(1.0f - PointLights[i].Intensity, 1e-8f));
        
        output.Ambient += result.Ambient * temp;
        output.Diffuse += result.Diffuse * att;
        output.Specular += result.Specular * att;
        output.Emissive += result.Emissive * att;
    }
}
void ComputeSpotLight(inout MaterialDesc output, float3 normal, float3 wPosition)
{
    output = MakeMaterial(); //초기화 하나 해놓고
    MaterialDesc result = MakeMaterial(); //합산할 값.
    
    
    for (uint i = 0; i < SpotLightCount; i++)
    {
        //DP * 조명의 개수 
        float3 light = SpotLights[i].Position - wPosition;
        float dist = length(light);
        
        [flatten] //구안에 범위에 안들어가는 조ㅕㅇ은 다 잘라내버렸다.
        if(dist > SpotLights[i].Range)
            continue;
        
        //light를 거리로 나누니까 단위벡터이다.
        light /= dist;
        
        
        float NdotL = dot(light, normalize(normal)); //Diffuse
        //같은색은 곱한다했지.
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
        //밑에것이 더 많이 나온다. 빛이.
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
    
    //전부 복사
    //메시 모델 애니메이션 까지 전부 복사 된다.
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
}; //구조체 똑같이 c버퍼 받을것

void VS_Projector(inout float4 wvp, float4 oPosition)
{
    wvp = WorldPosition(oPosition);
    wvp = mul(wvp, Projector.View);//main camera에 veiw 
    wvp = mul(wvp, Projector.Projection);

} 

//void PS_Projector(inout float4 color, float4 wvp)
//{
//float3 uvw = 0;
    
//    uvw.x = wvp.x / wvp.w * 0.5f + 0.5f;
//    uvw.y = -wvp.y / wvp.w * 0.5f + 0.5f;
//    uvw.z = wvp.z / wvp.w * 0.5f + 0.5f;

//// 이러면 ndc 공간이 된다. 0~1사이
    
//    //saturate 가 0~1까지다. uv도 0~1까지다.
//    //카메라 영역에 안들어온 애들, 0~1까지 범위를 넘어서겠지, 개내들 그리지 않겠다는거야
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