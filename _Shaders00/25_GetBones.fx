#include "00_Global.fx"
//랜더 아직 안만들었구나 00_Render
//00_Light는 만들었구나.


//구조체 자료형은 자료형 하나여도 무조건 구조체로 만들어야돼
//그래야 받을수 있다. 구조체 만들어어라 .쉐이더에서 받을때 //Create ComputeShader에서 computeBoneBuffer = new structuredbuffer( sizeof(matrix))
struct BoneDesc
{
    matrix Transform;
};
//본DESC 아웃풋도 똑같은 자료형으로 내보낼거야.
StructuredBuffer<BoneDesc> InputBones;
RWStructuredBuffer<BoneDesc> OutputBones; //여기에다 연결할것.

//21_Animation.fx와 비교
//정점쉐이더? 얘는 정점처리할거 아니니까 필요없지
//VertexModel. 몇번본 계산할건지가 필요하지
//애니메이션 계산할거니까 필요하지
//트위닝, 트랜스폼은 해야지

#define MAX_MODEL_TRANSFORMS 250

struct AnimationFrame
{
    int Clip;

    uint CurrFrame;
    uint NextFrame;

    float Time;
    float Running;

    float3 Padding;
};

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
//아이디는 일차원 배열이야
//기준본 가져다가 애니메이션 계산해서 자기 위치에다 넣어주면 돼
//기준본을 가져다가 애니메이션 계산해서 자기 위치에다 이때 애니메이션과 월드까지 계산해서 넣을거야
//그 값이 ID 위치에 들어갈것이다.
//3개 다 받을게 uint3 id


//해당 쉐이더의 아이디! id! vertexModel input이 아니라!
void SetAnimationWorld(inout matrix world, uint3 id)
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
    
    matrix curr = 0, currAnim = 0;
    matrix next = 0, nextAnim = 0;
    
    //indices를가지고 계산하겠어? 아니 id를 가지고 꺼내오는거잖아. 애니메이션 행렬꺼내오는것. x가 트랜스폼 번호잖아.
   
    //curFrame과 clip 은 y, z 이고 cBuffer로 따로 들어간다(쓰레드로). 그래서 x만 바꿔주면 된다.
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
    //가중치를 구할필요 없어졌다. 포문 필요없어졌고
    //해당본만 구하면 되지. //개를 기준으로 
        
    [flatten]
    if (clip[1] > -1)
    {
        c0 = TransformsMap.Load(int4(id.x * 4 + 0, currFrame[1], clip[1], 0));
        c1 = TransformsMap.Load(int4(id.x * 4 + 1, currFrame[1], clip[1], 0));
        c2 = TransformsMap.Load(int4(id.x * 4 + 2, currFrame[1], clip[1], 0));
        c3 = TransformsMap.Load(int4(id.x * 4 + 3, currFrame[1], clip[1], 0));
        curr = matrix(c0, c1, c2, c3);
        
        n0 = TransformsMap.Load(int4(id.x * 4 + 0, nextFrame[1], clip[1], 0));
        n1 = TransformsMap.Load(int4(id.x * 4 + 1, nextFrame[1], clip[1], 0));
        n2 = TransformsMap.Load(int4(id.x * 4 + 2, nextFrame[1], clip[1], 0));
        n3 = TransformsMap.Load(int4(id.x * 4 + 3, nextFrame[1], clip[1], 0));
        next = matrix(n0, n1, n2, n3);
        
        nextAnim = lerp(curr, next, time[1]);
      
        currAnim = lerp(currAnim, nextAnim, TweenFrames.TweenTime);
    }
      //현재 애니메이션 될행렬, 거기에 월드가 곱해져있다. 이제 기준만 곱해주면 되겠네.
    world = mul(currAnim, world);
}

struct BlendFrame
{
    uint Mode;
    float Alpha;
    float2 Padding;
    
    AnimationFrame Clip[3];
};

cbuffer CB_BlendFrame
{
    BlendFrame BlendFrames;
};

void SetBlendingWorld(inout matrix world, uint3 id)
{
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix curr = 0, next = 0;
    matrix anim = 0;
    matrix currAnim[3];
    
    
    [unroll(3)]
    for (int k = 0; k < 3; k++)
    {
        c0 = TransformsMap.Load(int4(id.x * 4 + 0, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
        c1 = TransformsMap.Load(int4(id.x * 4 + 1, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
        c2 = TransformsMap.Load(int4(id.x * 4 + 2, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
        c3 = TransformsMap.Load(int4(id.x * 4 + 3, BlendFrames.Clip[k].CurrFrame, BlendFrames.Clip[k].Clip, 0));
        curr = matrix(c0, c1, c2, c3);
        
        n0 = TransformsMap.Load(int4(id.x * 4 + 0, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
        n1 = TransformsMap.Load(int4(id.x * 4 + 1, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
        n2 = TransformsMap.Load(int4(id.x * 4 + 2, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
        n3 = TransformsMap.Load(int4(id.x * 4 + 3, BlendFrames.Clip[k].NextFrame, BlendFrames.Clip[k].Clip, 0));
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
    
    world = mul(anim, world);
}

//컴퓨트 세이더 작동시키면 되겠네. 250개 쓰레드 , y가 1 x가 1이겠지.
[numthreads(MAX_MODEL_TRANSFORMS, 1, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
    matrix world = World;
    
    if (BlendFrames.Mode == 0)
        SetAnimationWorld(world, id);
    else
        SetBlendingWorld(world, id);
    ///////////////////////////////////////////////////////////
    //최종 본 공간 = 기준 본 공간 * 애니메이션 공간 * 월드 공간(이동)
    ///////////////////////////////////////////////////////////
    world = mul(InputBones[id.x].Transform, world);
    //Trnasform 이 기준본 공간.  <여기에 기준 본공간 넣었다> InputBones[id.x]
    
    //애니메이션 공간 * 월드 = 이 2개는 이미 위에서 되어왔고 (world)
    
    
    //OutputBones 에다가 다시 넣어주면 돼.
    OutputBones[id.x].Transform = world;
}
//이제 구동시켜서 받아와서 테스트를 해봐야겠지
technique11 T0
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);

        SetComputeShader(CompileShader(cs_5_0, CS()));
    }
}