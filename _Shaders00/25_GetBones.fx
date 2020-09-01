#include "00_Global.fx"
//���� ���� �ȸ�������� 00_Render
//00_Light�� ���������.


//����ü �ڷ����� �ڷ��� �ϳ����� ������ ����ü�� �����ߵ�
//�׷��� ������ �ִ�. ����ü ������� .���̴����� ������ //Create ComputeShader���� computeBoneBuffer = new structuredbuffer( sizeof(matrix))
struct BoneDesc
{
    matrix Transform;
};
//��DESC �ƿ�ǲ�� �Ȱ��� �ڷ������� �������ž�.
StructuredBuffer<BoneDesc> InputBones;
RWStructuredBuffer<BoneDesc> OutputBones; //���⿡�� �����Ұ�.

//21_Animation.fx�� ��
//�������̴�? ��� ����ó���Ұ� �ƴϴϱ� �ʿ����
//VertexModel. ����� ����Ұ����� �ʿ�����
//�ִϸ��̼� ����ҰŴϱ� �ʿ�����
//Ʈ����, Ʈ�������� �ؾ���

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
//���̵�� ������ �迭�̾�
//���غ� �����ٰ� �ִϸ��̼� ����ؼ� �ڱ� ��ġ���� �־��ָ� ��
//���غ��� �����ٰ� �ִϸ��̼� ����ؼ� �ڱ� ��ġ���� �̶� �ִϸ��̼ǰ� ������� ����ؼ� �����ž�
//�� ���� ID ��ġ�� �����̴�.
//3�� �� ������ uint3 id


//�ش� ���̴��� ���̵�! id! vertexModel input�� �ƴ϶�!
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
    
    //indices�������� ����ϰھ�? �ƴ� id�� ������ �������°��ݾ�. �ִϸ��̼� ��Ĳ������°�. x�� Ʈ������ ��ȣ�ݾ�.
   
    //curFrame�� clip �� y, z �̰� cBuffer�� ���� ����(�������). �׷��� x�� �ٲ��ָ� �ȴ�.
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
    //����ġ�� �����ʿ� ��������. ���� �ʿ��������
    //�ش纻�� ���ϸ� ����. //���� �������� 
        
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
      //���� �ִϸ��̼� �����, �ű⿡ ���尡 �������ִ�. ���� ���ظ� �����ָ� �ǰڳ�.
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

//��ǻƮ ���̴� �۵���Ű�� �ǰڳ�. 250�� ������ , y�� 1 x�� 1�̰���.
[numthreads(MAX_MODEL_TRANSFORMS, 1, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
    matrix world = World;
    
    if (BlendFrames.Mode == 0)
        SetAnimationWorld(world, id);
    else
        SetBlendingWorld(world, id);
    ///////////////////////////////////////////////////////////
    //���� �� ���� = ���� �� ���� * �ִϸ��̼� ���� * ���� ����(�̵�)
    ///////////////////////////////////////////////////////////
    world = mul(InputBones[id.x].Transform, world);
    //Trnasform �� ���غ� ����.  <���⿡ ���� ������ �־���> InputBones[id.x]
    
    //�ִϸ��̼� ���� * ���� = �� 2���� �̹� ������ �Ǿ�԰� (world)
    
    
    //OutputBones ���ٰ� �ٽ� �־��ָ� ��.
    OutputBones[id.x].Transform = world;
}
//���� �������Ѽ� �޾ƿͼ� �׽�Ʈ�� �غ��߰���
technique11 T0
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);

        SetComputeShader(CompileShader(cs_5_0, CS()));
    }
}