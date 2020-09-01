#include "00_Global.fx"

Texture2D ParticleMap;

struct ParticleDesc
{
    float4 MinColor;
    float4 MaxColor;
    
    float3 Gravity;
    float EndVelocity;
    
    float2 StartSize;
    float2 EndSize;
    
    float2 RotateSpeed;
    float ReadyTime;
    float ReadyRandomTime;
    
    float CurrentTime;
};

cbuffer CB_Particle
{
    ParticleDesc Particle;
};


struct VertexInput
{
    float4 Position : Position;
    float2 Corner : Uv;
    float3 Velocity : Velocity;
    float4 Random : Random; //x : 주기, y - 크기, z - 회전, w - 색상
    float Time : Time;
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float4 Color : Color;
    float2 Uv : Uv;
};

float4 ComputePosition(float3 position, float3 velocity, float age, float normalizedAge)
{
    float start = length(velocity);
    float end = start * Particle.EndVelocity;
    
    float integral = start * normalizedAge + (end - start) * normalizedAge / 2;
    
    position += normalize(velocity) * integral * Particle.ReadyTime;
    position += Particle.Gravity * age * normalizedAge;
    
    return ViewProjection(float4(position, 1));
}

float ComputeSize(float value, float normalizedAge)
{
    float start = lerp(Particle.StartSize.x, Particle.StartSize.y, value);
    float end = lerp(Particle.EndSize.x, Particle.EndSize.y, value);
    
    return lerp(start, end, normalizedAge);
}

float2x2 ComputeRotation(float value, float age)
{
    float speed = lerp(Particle.RotateSpeed.x, Particle.RotateSpeed.y, value);
    float radian = speed * age;
    
    float c = cos(radian);
    float s = sin(radian);
    
    return float2x2(c, -s, s, c);
}

float4 ComputeColor(float4 projectedPosition, float randomValue, float normalizedAge)
{
    float4 color = lerp(Particle.MinColor, Particle.MaxColor, randomValue);
    color.a *= normalizedAge * (1 - normalizedAge) * (1 - normalizedAge) * 6.7f;
    
    return color;
}

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    float age = Particle.CurrentTime - input.Time;
    age *= input.Random.x * Particle.ReadyRandomTime + 1;
    //0~1까지 떨어뜨린다는 내용으로 하는거야.
    
    //레디 전에는 액티브가 안돼 레디 이후시간이라 봐야해
    //이렇게 하면 0~1까지 나와
    float normalizedAge = saturate(age / Particle.ReadyTime);
    
    //정규시간을 가지고 컬러부터 구해볼게
    output.Position = ComputePosition(input.Position.xyz, input.Velocity, age, normalizedAge);
    //시간에 대한 컬러야.
    float size = ComputeSize(input.Random.y, normalizedAge);
    float2x2 rotation = ComputeRotation(input.Random.z, age);
    
    output.Position.xy += mul(input.Corner, rotation) * size * 0.5f;
    
    output.Uv = (input.Corner + 1.0f) * 0.5f;
    output.Color = ComputeColor(output.Position, input.Random.w, normalizedAge);
    //현재 플레이 되고 있는 정규화된 시간으로 만들겠다는 뜻이야.1
    return output;
}

float4 PS(VertexOutput input) : SV_Target
{
    return ParticleMap.Sample(LinearSampler, input.Uv) * input.Color;
}

technique11 T0
{
    P_BS_VP(P0, Opaque, VS, PS)
    P_BS_VP(P1, AdditiveBlend, VS, PS)
    P_BS_VP(P2, AlphaBlend, VS, PS)
}