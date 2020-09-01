#include "00_Global.fx"
#define 250 //하기 
struct BoneDesc
{
    matrix Trnasform;
    
};
StructuredBuffer<BoneDesc> inputBones;
RWStructuredBuffer<BoneDesc> outputBones;

struct AnimationFrame
{
    int Clip;

    uint CurrFrame;
    uint NextFrame;

    float Time;
    float Running;

    float3 Padding;
};

//cbuffer CB_KeyFrames
//{
//    AnimationFrame AnimationFrames;
//};

struct TweenFrame
{
    float TakeTime;
    float TweenTime;
    float RunningTime;
    float Padding;

    AnimationFrame Curr;
    AnimationFrame Next;
};

cbuffer CB_AnimationFrame
{
    TweenFrame TweenFrames;
};

Texture2DArray TransformsMap;

void SetAnimationWorld(inout matrix world,int3 id)
{
  
    
    
    int clip[2];
    int currFrame[2];
    int nextFrame[2];
    float time[2];
    
    clip[0] = TweenFrames.Curr.Clip;
    currFrame[0] = TweenFrames.Curr.CurrFrame;
    nextFrame[0] = TweenFrames.Curr.NextFrame;
    time[0] = TweenFrames.Curr.Time;
    
    clip[1] = TweenFrames.Next.Clip;
    currFrame[1] = TweenFrames.Next.CurrFrame;
    nextFrame[1] = TweenFrames.Next.NextFrame;
    time[1] = TweenFrames.Next.Time;
    
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix transform = 0;
    matrix curr = 0, currAnim = 0;
    matrix next = 0, nextAnim = 0;
    
    [unroll(4)]
   
        c0 = TransformsMap.Load(int4(id.x * 4 + 0, currFrame[0], clip[0], 0));
        c1 = TransformsMap.Load(int4(id.x * 4 + 1, currFrame[0], clip[0], 0));
        c2 = TransformsMap.Load(int4(id.x * 4 + 2, currFrame[0], clip[0], 0));
        c3 = TransformsMap.Load(int4(id.x * 4 + 3, currFrame[0], clip[0], 0));
        curr = matrix(c0, c1, c2, c3);
        
        n0 = TransformsMap.Load(int4(id.x * 4 + 0, nextFrame[0], clip[0], 0));
        n1 = TransformsMap.Load(int4(id.x * 4 + 1, nextFrame[0], clip[0], 0));
        n2 = TransformsMap.Load(int4(id.x * 4 + 2, nextFrame[0], clip[0], 0));
        n3 = TransformsMap.Load(int4(id.x * 4 + 3, nextFrame[0], clip[0], 0));
        next = matrix(n0, n1, n2, n3);
        
        currAnim = lerp(curr, next, time[0]);
        
        
        [flatten]
        if (clip[1] > -1)
        {
            c0 = TransformsMap.Load(int4(id.x* 4 + 0, currFrame[1], clip[1], 0));
            c1 = TransformsMap.Load(int4(id.x* 4 + 1, currFrame[1], clip[1], 0));
            c2 = TransformsMap.Load(int4(id.x* 4 + 2, currFrame[1], clip[1], 0));
            c3 = TransformsMap.Load(int4(id.x* 4 + 3, currFrame[1], clip[1], 0));
            curr = matrix(c0, c1, c2, c3);
        
            n0 = TransformsMap.Load(int4(id.x * 4 + 0, nextFrame[1], clip[1], 0));
            n1 = TransformsMap.Load(int4(id.x * 4 + 1, nextFrame[1], clip[1], 0));
            n2 = TransformsMap.Load(int4(id.x * 4 + 2, nextFrame[1], clip[1], 0));
            n3 = TransformsMap.Load(int4(id.x * 4 + 3, nextFrame[1], clip[1], 0));
            next = matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(curr, next, time[1]);
            
            currAnim = lerp(currAnim, nextAnim, TweenFrames.TweenTime);
        }
        //가중치 필용없어져 , //transfrome 가중치 합산된것
    transform += mul(currAnim, world);
    
    
    world = mul(transform, world);
}
void SetBlendingWorld(inout matrix world, uint3 id)
{
    float id = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
    float id = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
    
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix curr = 0, next = 0;
    //matrix transform = 0;
    matrix anim = 0;
    matrix currAnim[3];
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        [unroll(3)]
        for (int k = 0; k < 3; k++)
        {
            c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
            c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
            c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
            c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
            curr = matrix(c0, c1, c2, c3);
        
            n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
            n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
            n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
            n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
            next = matrix(n0, n1, n2, n3);
        
            currAnim[k] = lerp(curr, next, BlendFrames.Clip[k].Time);
        }
            
        int clipA = floor(BlendFrames.Alpha);
        int clipB = clipA + 1;
        
        float alpha = BlendFrames.Alpha;
        if (alpha >= 1.0f)
        {
            alpha = BlendFrames.Alpha - 1.0f;
            
            if (BlendFrames.Alpha >= 2.0f)
            {
                clipA = 1;
                clipB = 2;
            }
        }
        
        anim = lerp(currAnim[clipA], currAnim[clipB], alpha);
        
        transform += mul(weights[i], anim);
    }
    
    world = mul(transform, world);
}

//dispatch 하나면 쓰레드 id나 dispatch id나 동일하자나.
[numtreads(MAX_MODEL_TRNASFORMS,1,1]
void CS(uint3 id : SV_DIspatchTrhadID)
{
    matrix world = World;
    
    if(BlendFrames.Mode == 0)
        SetAnimationWorld(world, id);
    else 
        SetBlendingWorld(world, id);
    
    //최종 본 공간 = 기분 본공간 * 애니메이션 공간 * 월드 공간
    world = mul(inputBones[id.x].Trnasform, world);
    
    outputBones[id.x] = world;
    
}

