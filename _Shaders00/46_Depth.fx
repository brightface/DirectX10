#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"


//Texture2D DepthMap; //���̿� �ؽ�ó
float4 PS(MeshOutput input) : SV_Target
{
    //������ ������ �ϴ°ž� ���̿� �ؽ�ó �ƴϰ�
    float4 position = input.wvpPosition.xyz / input.wvpPosition.w;
    //����� ���´�. �տ� 10% ���μ��� �Ҵ�, �ڿ� 90%�Ҵ�
    
    //���е��� �տ� �Ҵ��Ѵ�. 
    //if(position.z < 0.9f)
    //    return float4(1, 0, 0, 1);
    
    
    //MatrixFOV;
    //��Ʈ���� ���� �� �Լ� ������� �����ž�.
    //���� ��½�
    float n = 0.1f;//near
    float f = 1000.0f;//far
    float depth = (2.0f * n) / (f + n - position.z + (f - n);
    
    //return float4( depth, depth,depth, 1.0f);
    //return float4(position.z, /position.z, position.z, 1.0f);
    
    //float depth = (1.0f - position.z) * -10.0f;
    //if(depth<5)
    //    return float4(0, 1, 0, 1);
    
    //return float4(0, 0, 1, 1.0f);
   
    
    //    if(position.z >0.9f)
    //    return float4(0, 1, 0, 1);
    //if(position.z>0.925f)
    //{
    //    return float4(0, 0, 1, 1);
    //}
        
    //return float4(position.z, position.z, position.z, 1.0f);

}

technique11 T0
{
    P_VP(P0, VS_Depth_Mesh, PS_Depth)
    P_VP(P1, VS_Depth_Model, PS_Depth)
    P_VP(P2, VS_Depth_Animation, PS_Depth)
}