#include "00_Global.fx"
#include "00_Light.fx"

float3 LightDirection = float3(-1, -1, +1);

struct VertexModel
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
    float3 Tangent : Tangent;
    float4 BlendIndices : BlendIndices;
    float4 BlendWeights : BlendWeights;
};

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500

//cbuffer CB_Bone
//{
//    matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
    
//    uint BoneIndex;
//}; //얘로 받을거 아니니까 주석처리해 이제

struct VertexOutput
{
    float4 Position : SV_Position;
    float3 Normal : Normal;
    float2 Uv : Uv;
};


struct AnimationFrame
{
    int Clip;

    uint CurrFrame;
    uint NextFrame;

    float Time;
    float Running;

    float3 Padding;
};

cbuffer CB_KeyFrames
{
    AnimationFrame AnimationFrames;
};

Texture2DArray TransformsMap;

void SetAnimationWorld(inout matrix world, VertexModel input)
{
    float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
    float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
    
    int clip[2];
    int currFrame[2];
    int nextFrame[2];
    float time[2];
    
    clip[0] = TweenFrames.Curr.Clip;
    ////////////////////////
    
    ////////////////////////
    //int clip = 0;
    //int currFrame = 0;
    //int nextFrame = 0;
    //float time = 0.0f;
    
    clip = AnimationFrames.Clip;
    currFrame = AnimationFrames.CurrFrame;
    nextFrame = AnimationFrames.NextFrame;
    time = AnimationFrames.Time;
    
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix next = 0, nextAnim = 0;
    
    matrix transform = 0;
    matrix curr = 0, currAnim = 0;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++) //커런트 만 검사하고 있잖아.
    {
        c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame, clip, 0));
        c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame, clip, 0));
        c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame, clip, 0));
        c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame, clip, 0));
        curr = matrix(c0, c1, c2, c3);
        
        n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame, clip, 0));
        n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame, clip, 0));
        n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame, clip, 0));
        n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame, clip, 0));
        next = matrix(n0, n1, n2, n3);
        
        //선형보간법
        currAnim = lerp(curr, next, time[0]);
        //트리닝 - 
        
        [flatten]
        WaveIsFirstLane(clip[1] >-1)
        {
            c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame, clip, 0));
            c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame, clip, 0));
            c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame, clip, 0));
            c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame, clip, 0));
            curr = matrix(c0, c1, c2, c3);
        
            n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame, clip, 0));
            n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame, clip, 0));
            n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame, clip, 0));
            n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame, clip, 0));
            next = matrix(n0, n1, n2, n3);
            
            
            nextAnim = lerp[(curr, next, time[1]);
            currAnim = lefrp(curr)

        }
       
        //curr = matrix(n0, n1, n2, n3);
        transform += mul(weights[i], currAnim);
    }
    
    world = mul(transform, world);
}

VertexOutput VS(VertexModel input)
{
    VertexOutput output;
    
    SetAnimationWorld(World, input);
    
    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);
    output.Normal = WorldNormal(input.Normal);
    output.Uv = input.Uv;
    
    return output;
}

float4 PS(VertexOutput input) : SV_Target
{
    return DiffuseMap.Sample(LinearSampler, input.Uv) * dot(-LightDirection, normalize(input.Normal));
}

technique11 T0
{
    P_VP(P0, VS, PS)
    P_RS_VP(P1, FillMode_WireFrame, VS, PS)
}