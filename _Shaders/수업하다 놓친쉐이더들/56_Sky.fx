///////////////////////////////////////////////////////////////////////////////

static const float EarthInnerRadius = 6356.7523142f;
static const float EarthOuterRadius = 6356.7523142f * 1.0157313f; //원판에 의해서 도니까 .적도가 살짝 뚱글해

static const float KrESun = 0.0025f * 20.0f; //0.0025f - 레일리 상수 * 태양의 밝기
static const float KmESun = 0.0010f * 20.0f; //0.0025f - 미 상수 * 태양의 밝기
static const float Kr4PI = 0.0025f * 4.0f * 3.14159265f; //-레일리산란에 대한 거리
static const float Km4PI = 0.0010f * 4.0f * 3.14159265f; //-미산란에 대한 거리

static const float2 RayleighMieScaleHeight = { 0.25f, 0.1f };//높이
static const float EarthScale = 1.0 / (6356.7523142 * 1.0157313 - 6356.7523142);//지구의 크기 적도부분의대한 비율

static const float SunExposure = -2.0f; //태양의 강도

static const float FloatOne = 0.00390625;//로그산란
static const float FloatOneHalf = 0.001953125;//반값

///////////////////////////////////////////////////////////////////////////////
//중력

static const float PI = 3.14159265f;
static const float G = -0.980f;
static const float G2 = -0.980f * -0.980f;
///////////////////////////////////////////////////////////////////////////////
// Scattering
///////////////////////////////////////////////////////////////////////////////
struct ScatteringDesc
{
    float3 WaveLength;
    float Padding;
    
    float3 InvWaveLength;
    int SampleCount;

    float3 WaveLengthMie;
};

cbuffer CB_Scattering
{
    ScatteringDesc Scattering;
};


struct VertexOutput_Scattering
{
    float4 Position : SV_Position0;
    float2 Uv : Uv0;
};

VertexOutput_Scattering VS_Scattering(VertexTexture input)
{
    VertexOutput_Scattering output;

    output.Position = input.Position;
    output.Uv = input.Uv;

    return output;
}

float HitOuterSphere(float3 position, float3 direction)
{
    float3 light = -position;

    float b = dot(light, direction);
    float c = dot(light, light);

    float d = c - b * b;
    float q = sqrt(EarthOuterRadius * EarthOuterRadius - d);

    return b + q;
}

float2 GetDensityRatio(float height)
{
    float altitude = (height - EarthInnerRadius) * EarthScale;

    return exp(-altitude / RayleighMieScaleHeight);
}

float2 GetDistance(float3 p1, float3 p2)
{
    float2 opticalDepth = 0;

    float3 temp = p2 - p1;
    float far = length(temp);
    float3 direction = temp / far;


    float sampleLength = far / Scattering.SampleCount;
    float scaledLength = sampleLength * EarthScale;

    float3 sampleRay = direction * sampleLength;
    p1 += sampleRay * 0.5f;

    for (int i = 0; i < Scattering.SampleCount; i++)
    {
        float height = length(p1);
        opticalDepth += GetDensityRatio(height);

        p1 += sampleRay;
    }

    return opticalDepth * scaledLength;
}

struct PixelOutput_Scattering
{
    float4 R : SV_Target0;
    float4 M : SV_Target1;
};

PixelOutput_Scattering PS_Scattering(VertexOutput_Scattering input)
{
    PixelOutput_Scattering output;
    
    float3 sunDirection = -normalize(GlobalLight.Direction);
    float2 uv = input.Uv;

    float3 pointPv = float3(0, EarthInnerRadius + 1e-3f, 0.0f);
    float angleXZ = PI * uv.y;
    float angleY = 100.0f * uv.x * PI / 180.0f;

    //구면 - 태양위치 
    float3 direction;
    direction.x = sin(angleY) * cos(angleXZ);
    direction.y = cos(angleY);
    direction.z = sin(angleY) * sin(angleXZ);
    direction = normalize(direction);

    //빛이 닿는 공간 구하려고 
    float farPvPa = HitOuterSphere(pointPv, direction);
    float3 ray = direction;

    float3 pointP = pointPv;
    float sampleLength = farPvPa / Scattering.SampleCount;
    float scaledLength = sampleLength * EarthScale;
    float3 sampleRay = ray * sampleLength;
    pointP += sampleRay * 0.5f;

    float3 rayleighSum = 0;
    float3 mieSum = 0;
    for (int i = 0; i < Scattering.SampleCount; i++)
    {
        float pHeight = length(pointP);

        float2 densityRatio = GetDensityRatio(pHeight);
        densityRatio *= scaledLength;


        float2 viewerOpticalDepth = GetDistance(pointP, pointPv);

        float farPPc = HitOuterSphere(pointP, sunDirection);
        float2 sunOpticalDepth = GetDistance(pointP, pointP + sunDirection * farPPc);

        float2 opticalDepthP = sunOpticalDepth.xy + viewerOpticalDepth.xy;
        float3 attenuation = exp(-Kr4PI * Scattering.InvWaveLength * opticalDepthP.x - Km4PI * opticalDepthP.y);

        rayleighSum += densityRatio.x * attenuation;
        mieSum += densityRatio.y * attenuation;
        //커지면 정밀해져
        pointP += sampleRay;
    }
    //레일리랑 미산란 .2개 빼내지 
    float3 rayleigh = rayleighSum * KrESun;
    float3 mie = mieSum * KmESun;

    rayleigh *= Scattering.InvWaveLength;
    mie *= Scattering.WaveLengthMie;

    output.R = float4(rayleigh, 1);
    output.M = float4(mie, 1);

    return output;
}
///////////////////////////////////////////////////////////////////////////////
// Draw SkySphere
///////////////////////////////////////////////////////////////////////////////
struct VertexOutput_Dome
{
    float4 Position : SV_Position;
    float3 oPosition : Position1;
    float2 Uv : Uv0;
};

VertexOutput_Dome VS_Dome(VertexTexture input)
{
    VertexOutput_Dome output;
    
    output.oPosition = -input.Position.xyz;
    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);
    
    output.Uv = input.Uv;
    
    return output;
}

//반사 때 쓸거야 , 반사 , 굴절 //반사 + 굴절 = 물//
// 언리얼 씨로 넘어가면서 바로 분석하라고 시작해 //

/////////////////////////////////////////////////////////////

//VertexOutput_Dome VS_Reflection_Dome(VertexTexture input)
//{
//    VertexOutput_Dome output;
    
//    output.oPosition = -input.Position.xyz;
//    output.Position = WorldPosition(input.Position);
//    output.Position = mul(output.Position, ReflectionView);
//    output.Position = mul(output.Position, Projection);
    
//    output.Uv = input.Uv;
    
//    return output;
//}

float GetRayleighPhase(float c)
{
    return 0.75f * (1.0f + c);
}

float GetMiePhase(float c, float c2)
{
    float3 result = 0;
    result.x = 1.5f * ((1.0f - G2) / (2.0f + G2));
    result.y = 1.0f + G2;
    result.z = 2.0f * G;

    return result.x * (1.0f + c2) / pow(result.y - result.z * c, 1.5f);
}

float3 HDR(float3 LDR)
{
    return 1.0f - exp(SunExposure * LDR);
}


Texture2D RayleighMap;
Texture2D MieMap;
Texture2D StarMap;


float4 PS_Dome(VertexOutput_Dome input) : SV_TARGET0
{
    float3 sunDirection = -normalize(GlobalLight.Direction);

    float temp = dot(sunDirection, input.oPosition) / length(input.oPosition);
    float temp2 = temp * temp;

    float3 rSamples = RayleighMap.Sample(LinearSampler, input.Uv).rgb;
    float3 mSamples = MieMap.Sample(LinearSampler, input.Uv).rgb;

    float3 color = 0;
    color = GetRayleighPhase(temp2) * rSamples + GetMiePhase(temp, temp2) * mSamples;
    color = HDR(color);

    color += max(0, (1 - color.rgb)) * float3(0.05f, 0.05f, 0.1f);

    float starIntensity = GlobalLight.Direction.y;
    return float4(color, 1) + StarMap.Sample(LinearSampler, input.Uv) * saturate(starIntensity);
}

//맨뒤에 그려져야 하니까 뎁스스텐실이랑 다 킬거야
