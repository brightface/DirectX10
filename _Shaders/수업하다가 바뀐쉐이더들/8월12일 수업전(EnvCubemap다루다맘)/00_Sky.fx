///////////////////////////////////////////////////////////////////////////////

static const float PI = 3.14159265f;
static const float G = -0.980f;
static const float G2 = -0.980f * -0.980f;

static const float EarthInnerRadius = 6356.7523142f;
static const float EarthOuterRadius = 6356.7523142f * 1.0157313f;

static const float KrESun = 0.0025f * 20.0f; //0.0025f - 레일리 상수 * 태양의 밝기
static const float KmESun = 0.0010f * 20.0f; //0.0025f - 미 상수 * 태양의 밝기
static const float Kr4PI = 0.0025f * 4.0f * 3.14159265f;
static const float Km4PI = 0.0010f * 4.0f * 3.14159265f;

static const float2 RayleighMieScaleHeight = { 0.25f, 0.1f };
static const float EarthScale = 1.0 / (6356.7523142 * 1.0157313 - 6356.7523142);

static const float SunExposure = -2.0f;

static const float FloatOne = 0.00390625;
static const float FloatOneHalf = 0.001953125;

///////////////////////////////////////////////////////////////////////////////

//이거 meshoutput으로 바꿔서 통합해주는 이유가 geometryShader로 meshoutput으로 받으려고
//그리고 meshoutput으로 리턴해줄려고

#define VS_SKY_GENERATE \
output.Position = WorldPosition(input.Position); \
output.wPosition = output.Position.xyz; \
\
output.Position = ViewProjection(output.Position); \
output.wvpPosition = output.Position; \
output.wvpPosition_Sub = output.Position;\
\
output.Uv = input.Uv;\
\
output.Culling.x = dot(float4(output.wPosition, 1), Culling[0]); \
output.Culling.y = dot(float4(output.wPosition, 1), Culling[1]); \
output.Culling.z = dot(float4(output.wPosition, 1), Culling[2]); \
output.Culling.w = dot(float4(output.wPosition, 1), Culling[3]); \
\
output.Clipping = float4(0, 0, 0, 0); \
output.Clipping.x = dot(float4(output.wPosition, 1), Clipping);


//얘는 mul 이것만 바꾸면되지?
#define VS_SKY_REFLECTION_GENERATE \
output.Position = WorldPosition(input.Position); \
output.wPosition = output.Position.xyz; \
\
output.Position = mul(output.Position, ReflectionView); \
output.Position = mul(output.Position, Projection); \
output.wvpPosition = output.Position; \
output.wvpPosition_Sub = output.Position;\
\
output.Uv = input.Uv;\
\
output.Culling.x = dot(float4(output.wPosition, 1), Culling[0]); \
output.Culling.y = dot(float4(output.wPosition, 1), Culling[1]); \
output.Culling.z = dot(float4(output.wPosition, 1), Culling[2]); \
output.Culling.w = dot(float4(output.wPosition, 1), Culling[3]); \
\
output.Clipping = float4(0, 0, 0, 0); \
output.Clipping.x = dot(float4(output.wPosition, 1), Clipping);

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

    float3 direction;
    direction.x = sin(angleY) * cos(angleXZ);
    direction.y = cos(angleY);
    direction.z = sin(angleY) * sin(angleXZ);
    direction = normalize(direction);

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

        pointP += sampleRay;
    }

    float3 rayleigh = rayleighSum * KrESun;
    float3 mie = mieSum * KmESun;

    rayleigh *= Scattering.InvWaveLength;
    mie *= Scattering.WaveLengthMie;

    output.R = float4(rayleigh, 1);
    output.M = float4(mie, 1);

    return output;
}


///////////////////////////////////////////////////////////////////////////////
// Draw Dome
///////////////////////////////////////////////////////////////////////////////
struct VertexOutput_Dome
{
    float4 Position : SV_Position;
    float3 oPosition : Position1;
    float2 Uv : Uv0;
};

//이거 바꼈지-VertexOutput
MeshOutput VS_Dome(VertexTexture input)
{	

// 구조체 초기화
    MeshOutput output = (MeshOutput)0;
    //이것도 바꼈지
    output.oPosition = -input.Position.xyz; //inputposition이었는데
    //똑같은거 inputPosition으로 넣어야하는데 gernerate로 만듬 
	//그리고 컬링하고  클립핑 랜더에 있잖아.

//이게 제네레이트야. 포지션 계산하는것도 동일하고
//	output.Culling.x = dot(float4(output.wPosition, 1), Culling[0]); \
//output.Culling.y = dot(float4(output.wPosition, 1), Culling[1]); \
//output.Culling.z = dot(float4(output.wPosition, 1), Culling[2]); \
//output.Culling.w = dot(float4(output.wPosition, 1), Culling[3]); \
//\
output.Clipping = float4(0, 0, 0, 0); \
output.Clipping.x = dot(float4(output.wPosition, 1), Clipping); \
\
	//뒤에 띄어쓰기 있으면 같은줄로 인지해. 반드시 띄어쓰기 없어야해.
	//노멀 안받았늬까 0으로 들어가 있을테니까
    VS_SKY_GENERATE
    
    return output;
}
//반사도 메시 아웃풋으로 굳이 안가도 될거 같은데 world, view,uv 만있으면 되니까 반사는 돌릴게.
MeshOutput VS_Dome_Reflection(VertexTexture input)
{
    MeshOutput output = (MeshOutput)0;
    
    output.oPosition = -input.Position.xyz;
    
    VS_SKY_REFLECTION_GENERATE
    
    return output;
}

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


float4 PS_Dome(MeshOutput input) : SV_TARGET0
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw Moon
///////////////////////////////////////////////////////////////////////////////
matrix MoonProjection;
struct VertexOutput_Moon
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
};
//문 스카이 통일성 유지하려고 다 바꾸자 그냥 

MeshOutput VS_Moon(VertexTexture input)
{
    MeshOutput output = (MeshOutput)0;

    VS_SKY_GENERATE

    return output;
}

MeshOutput VS_Moon_Reflection(VertexTexture input)
{
    MeshOutput output = (MeshOutput)0;

    VS_SKY_REFLECTION_GENERATE
    
    output.Uv = input.Uv;

    return output;
}

Texture2D MoonMap;
float MoonAlpha;
float4 PS_Moon(MeshOutput input) : SV_Target0
{
    float4 color = MoonMap.Sample(LinearSampler, input.Uv);
    color.a *= MoonAlpha;

    return color;
}


///////////////////////////////////////////////////////////////////////////////
// Draw Cloud
///////////////////////////////////////////////////////////////////////////////
cbuffer CB_Cloud
{
    float CloudTiles;
    float CloudCover = -0.1f;
    float CloudSharpness = 0.25f;
    float CloudSpeed = 0.01f;
};

struct VertexOutput_Cloud
{
    float4 Position : SV_Position0;
    float2 Uv : Uv0;
    float2 oUv : Uv1;
};

MeshOutput VS_Cloud(VertexTexture input)
{
    MeshOutput output = (MeshOutput)0;

    VS_SKY_GENERATE

    output.Uv = (input.Uv * CloudTiles);

    return output;
}

MeshOutput VS_Cloud_Reflection(VertexTexture input)
{
    MeshOutput output = (MeshOutput)0;
	
    VS_SKY_REFLECTION_GENERATE

    output.Uv = (input.Uv * CloudTiles);
	//어차피 컬러 안써 output.oUv = input.Uv; 삭제 //oUv 안써.
    return output;
}

float Fade(float t)
{
    // return t * t * (3.0 - 2.0 * t);
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

texture2D CloudMap;
float Noise(float2 P)
{
    float2 Pi = FloatOne * floor(P) + FloatOneHalf;
    float2 Pf = frac(P);

    float2 grad00 = CloudMap.Sample(LinearSampler, Pi).rg * 4.0 - 1.0;
    float n00 = dot(grad00, Pf);

    float2 grad10 = CloudMap.Sample(LinearSampler, Pi + float2(FloatOne, 0.0)).rg * 4.0 - 1.0;
    float n10 = dot(grad10, Pf - float2(1.0, 0.0));

    float2 grad01 = CloudMap.Sample(LinearSampler, Pi + float2(0.0, FloatOne)).rg * 4.0 - 1.0;
    float n01 = dot(grad01, Pf - float2(0.0, 1.0));

    float2 grad11 = CloudMap.Sample(LinearSampler, Pi + float2(FloatOne, FloatOne)).rg * 4.0 - 1.0;
    float n11 = dot(grad11, Pf - float2(1.0, 1.0));

    float2 n_x = lerp(float2(n00, n01), float2(n10, n11), Fade(Pf.x));

    float n_xy = lerp(n_x.x, n_x.y, Fade(Pf.y));

    return n_xy;
}

float4 PS_Cloud(MeshOutput input) : SV_Target
{
    input.Uv = input.Uv * CloudTiles;

    float n = Noise(input.Uv + Time * CloudSpeed);
    float n2 = Noise(input.Uv * 2 + Time * CloudSpeed);
    float n3 = Noise(input.Uv * 4 + Time * CloudSpeed);
    float n4 = Noise(input.Uv * 8 + Time * CloudSpeed);
	
    float nFinal = n + (n2 / 2) + (n3 / 4) + (n4 / 8);
	
    float c = CloudCover - nFinal;
    if (c < 0) 
        c = 0;
 
    float density = 1.0 - pow(CloudSharpness, c);
    float4 color = density;
    color.rgb *= 2.0f;
    
    return color;
}
//MeshOutput이 정점쉐이더에서 Rasterzing Position에 안들어가면(SV_Position) 써있는정점
//position 값 유지가 된다. 값이 안바껴. 
//Geometry 로 정점쉐이더 값들어가면 SV_position 값이 안바껴.
//그 원리 이용해서 쓸거야.