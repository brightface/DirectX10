#define VS_GENERATE \
output.oPosition = input.Position.xyz; \
output.Position = WorldPosition(input.Position); \
output.wPosition = output.Position.xyz; \
\
output.Position = ViewProjection(output.Position); \
output.wvpPosition = output.Position; \
output.wvpPosition_Sub = output.Position; \
\
output.sPosition = WorldPosition(input.Position); \
output.sPosition = mul(output.sPosition, ShadowView); \
output.sPosition = mul(output.sPosition, ShadowProjection); \
\
output.Normal = WorldNormal(input.Normal); \
output.Tangent = WorldTangent(input.Tangent); \
output.Uv = input.Uv; \
output.Color = input.Color; \
\
output.Culling.x = dot(float4(output.wPosition, 1), Culling[0]); \
output.Culling.y = dot(float4(output.wPosition, 1), Culling[1]); \
output.Culling.z = dot(float4(output.wPosition, 1), Culling[2]); \
output.Culling.w = dot(float4(output.wPosition, 1), Culling[3]);


#define VS_DEPTH_GENERATE \
output.Position = WorldPosition(input.Position); \
output.Position = mul(output.Position, ShadowView); \
output.Position = mul(output.Position, ShadowProjection); \
\
output.sPosition = output.Position;


#define VS_REFLECTION_GENERATE \
output.oPosition = input.Position.xyz; \
output.Position = WorldPosition(input.Position); \
output.wPosition = output.Position.xyz; \
\
output.Position = mul(output.Position, ReflectionView); \
output.Position = mul(output.Position, Projection); \
output.wvpPosition = output.Position; \
output.wvpPosition_Sub = output.Position; \
\
output.sPosition = WorldPosition(input.Position); \
output.sPosition = mul(output.sPosition, ShadowView); \
output.sPosition = mul(output.sPosition, ShadowProjection); \
\
output.Normal = WorldNormal(input.Normal); \
output.Tangent = WorldTangent(input.Tangent); \
output.Uv = input.Uv; \
output.Color = input.Color; \
\
output.Culling.x = dot(float4(output.wPosition, 1), Culling[0]); \
output.Culling.y = dot(float4(output.wPosition, 1), Culling[1]); \
output.Culling.z = dot(float4(output.wPosition, 1), Culling[2]); \
output.Culling.w = dot(float4(output.wPosition, 1), Culling[3]);

///////////////////////////////////////////////////////////////////////////////

struct VertexMesh
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
    float3 Tangent : Tangent;
    
    matrix Transform : Inst1_Transform; //vertexMesh 가 인스턴싱용 하나 더 추가가된다.
    float4 Color : Inst2_Color;
};
//inout 레퍼런스와 같다.
void SetMeshWorld(inout matrix world, VertexMesh input)
{
    world = input.Transform; //월드가 인풋으로 들어오잖아. 
}

//여기에서는 월드계산 하는거 딱 하나 바뀌어야 하는거 있지
MeshOutput VS_Mesh(VertexMesh input)
{
    MeshOutput output;
    SetMeshWorld(World, input);//인스턴싱에 있는 월드를 사용할거니까. 트랜스폼 따로 주진않겠네 , input넘겨주면 되겠고
    //이제 메시는 끝났지 . 메시데모로 가
    VS_GENERATE
    
    return output;
}

MeshDepthOutput VS_Mesh_Depth(VertexMesh input)
{
    MeshDepthOutput output;
    SetMeshWorld(World, input);
    
    VS_DEPTH_GENERATE
    
    return output;
}

MeshOutput VS_Mesh_Projector(VertexMesh input)
{
    MeshOutput output = VS_Mesh(input);
    
    
    return output;
}
//프로젝터 대신 리플렉션
MeshOutput VS_Mesh_Reflection(VertexMesh input)
{
    MeshOutput output;
    SetMeshWorld(World, input);
    
    VS_REFLECTION_GENERATE
    //월드 뷰 프로젝션 다 계산됬잖아 그후 포지션을 따로 계산할거야.
    return output;
}

///////////////////////////////////////////////////////////////////////////////

struct VertexModel
{
    float4 Position : Position;
    float2 Uv : Uv;
    float3 Normal : Normal;
    float3 Tangent : Tangent;
    float4 BlendIndices : BlendIndices;
    float4 BlendWeights : BlendWeights;
    
    //이 밑 3개가 모델도 필요하다.
    matrix Transform : Inst1_Transform;
    float4 Color : Inst2_Color;
    
    uint InstanceId : SV_InstanceID;
};

Texture2DArray TransformsMap;
#define MAX_MODEL_TRANSFORMS 250

cbuffer CB_Bones
{
    //matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
    //텍스처로 들어오니까 다 죽였다. 필요없다.
    uint BoneIndex;
};

void SetModelWorld(inout matrix world, VertexModel input)
{
    //World = mul(BoneTransforms[BoneIndex], World);
    //월드 변환이 이런식으로 일어났잖아
    
    //트랜스폼맵이 됬잖아. 그걸로부터 가져와야지 , x는 본인덱스, y는 instance, z는 없다.면 없다 0 , 그리고 w 없자나 : w는 민맵,
    float4 m0 = TransformsMap.Load(int4(BoneIndex * 4 + 0, input.InstanceId, 0, 0));
    float4 m1 = TransformsMap.Load(int4(BoneIndex * 4 + 1, input.InstanceId, 0, 0));
    float4 m2 = TransformsMap.Load(int4(BoneIndex * 4 + 2, input.InstanceId, 0, 0));
    float4 m3 = TransformsMap.Load(int4(BoneIndex * 4 + 3, input.InstanceId, 0, 0));

    //최종 매트릭스 만들거야.
    matrix transform = matrix(m0, m1, m2, m3);
    world = mul(transform, input.Transform); //world를 여기에서 가져오는게 아니라 input.world에서 가져와
}

MeshOutput VS_Model(VertexModel input)
{
    MeshOutput output;
    SetModelWorld(World, input); //월드 콜해주면 되겠지 - 메시는 끝났네/ 애니메이션 들어가면 된다.
    
    VS_GENERATE
    
    return output;
}

MeshDepthOutput VS_Model_Depth(VertexModel input)
{
    MeshDepthOutput output;
    SetModelWorld(World, input);
    
    VS_DEPTH_GENERATE
    
    return output;
}

MeshOutput VS_Model_Projector(VertexModel input)
{
    MeshOutput output = VS_Model(input);
    VS_Projector(output.wvpPosition_Sub, input.Position);
    
    return output;
}

MeshOutput VS_Model_Reflection(VertexModel input)
{
    MeshOutput output;
    SetModelWorld(World, input);
    
    VS_REFLECTION_GENERATE
    
    return output;
}

///////////////////////////////////////////////////////////////////////////////

#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500

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
    TweenFrame TweenFrames[MAX_MODEL_INSTANCE];
};

void SetAnimationWorld(inout matrix world, VertexModel input)
{
    float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
    float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
    
    
    int clip[2];
    int currFrame[2];
    int nextFrame[2];
    float time[2];
    
    clip[0] = TweenFrames[input.InstanceId].Curr.Clip;
    currFrame[0] = TweenFrames[input.InstanceId].Curr.CurrFrame;
    nextFrame[0] = TweenFrames[input.InstanceId].Curr.NextFrame;
    time[0] = TweenFrames[input.InstanceId].Curr.Time;
    
    clip[1] = TweenFrames[input.InstanceId].Next.Clip;
    currFrame[1] = TweenFrames[input.InstanceId].Next.CurrFrame;
    nextFrame[1] = TweenFrames[input.InstanceId].Next.NextFrame;
    time[1] = TweenFrames[input.InstanceId].Next.Time;
    
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix transform = 0;
    matrix curr = 0, currAnim = 0;
    matrix next = 0, nextAnim = 0;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[0], clip[0], 0));
        c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[0], clip[0], 0));
        c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[0], clip[0], 0));
        c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[0], clip[0], 0));
        curr = matrix(c0, c1, c2, c3);
        
        n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
        n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
        n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
        n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
        next = matrix(n0, n1, n2, n3);
        
        currAnim = lerp(curr, next, time[0]);
        
        
        [flatten]
        if (clip[1] > -1)
        {
            c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[1], clip[1], 0));
            c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[1], clip[1], 0));
            c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[1], clip[1], 0));
            c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[1], clip[1], 0));
            curr = matrix(c0, c1, c2, c3);
        
            n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0));
            n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0));
            n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0));
            n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0));
            next = matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(curr, next, time[1]);
            
            currAnim = lerp(currAnim, nextAnim, TweenFrames[input.InstanceId].TweenTime);
        }
        
        transform += mul(weights[i], currAnim);
    }
    
    world = mul(transform, world);
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
    BlendFrame BlendFrames[MAX_MODEL_INSTANCE];
};

void SetBlendingWorld(inout matrix world, VertexModel input)
{
    float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
    float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
    
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix curr = 0, next = 0;
    matrix transform = 0;
    matrix anim = 0;
    matrix currAnim[3];
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        [unroll(3)]
        for (int k = 0; k < 3; k++)
        {
            c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendFrames[input.InstanceId].Clip[k].CurrFrame, BlendFrames[input.InstanceId].Clip[k].Clip, 0));
            c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendFrames[input.InstanceId].Clip[k].CurrFrame, BlendFrames[input.InstanceId].Clip[k].Clip, 0));
            c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendFrames[input.InstanceId].Clip[k].CurrFrame, BlendFrames[input.InstanceId].Clip[k].Clip, 0));
            c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendFrames[input.InstanceId].Clip[k].CurrFrame, BlendFrames[input.InstanceId].Clip[k].Clip, 0));
            curr = matrix(c0, c1, c2, c3);
        
            n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendFrames[input.InstanceId].Clip[k].NextFrame, BlendFrames[input.InstanceId].Clip[k].Clip, 0));
            n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendFrames[input.InstanceId].Clip[k].NextFrame, BlendFrames[input.InstanceId].Clip[k].Clip, 0));
            n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendFrames[input.InstanceId].Clip[k].NextFrame, BlendFrames[input.InstanceId].Clip[k].Clip, 0));
            n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendFrames[input.InstanceId].Clip[k].NextFrame, BlendFrames[input.InstanceId].Clip[k].Clip, 0));
            next = matrix(n0, n1, n2, n3);
        
            currAnim[k] = lerp(curr, next, BlendFrames[input.InstanceId].Clip[k].Time);
        }
            
        int clipA = floor(BlendFrames[input.InstanceId].Alpha);
        int clipB = clipA + 1;
        
        float alpha = BlendFrames[input.InstanceId].Alpha;
        if (alpha >= 1.0f)
        {
            alpha = BlendFrames[input.InstanceId].Alpha - 1.0f;
            
            if (BlendFrames[input.InstanceId].Alpha >= 2.0f)
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

MeshOutput VS_Animation(VertexModel input)
{
    MeshOutput output;
    
    World = input.Transform;
    
    if (BlendFrames[input.InstanceId].Mode == 0)
        SetAnimationWorld(World, input);
    else
        SetBlendingWorld(World, input);
    
    VS_GENERATE
    
    return output;
}

MeshDepthOutput VS_Animation_Depth(VertexModel input)
{
    MeshDepthOutput output;
    
    World = input.Transform;
    
    if (BlendFrames[input.InstanceId].Mode == 0)
        SetAnimationWorld(World, input);
    else
        SetBlendingWorld(World, input);
    
    VS_DEPTH_GENERATE
    
    return output;
}

MeshOutput VS_Animation_Projector(VertexModel input)
{
    MeshOutput output = VS_Animation(input);
    VS_Projector(output.wvpPosition_Sub, input.Position);
    
    return output;
}

MeshOutput VS_Animation_Reflection(VertexModel input)
{
    MeshOutput output;
    
    World = input.Transform;
    
    if (BlendFrames[input.InstanceId].Mode == 0)
        SetAnimationWorld(World, input);
    else
        SetBlendingWorld(World, input);
    
    VS_REFLECTION_GENERATE
    
    return output;

    return output;
}