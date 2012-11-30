#include "shaders/common/hlslstructs.h"   
  
struct ConnectData   
{   
   float4 Position        : POSITION;     
   float2 TexCoord        : TEXCOORD0;   
};   
  
//-----------------------------------------------------------------------------     
// Main     
//-----------------------------------------------------------------------------     
ConnectData main( VertexIn_PT IN,        
    uniform float4x4 modelview
    //uniform float4x4 objTrans  
)       
{     
    ConnectData OUT;   
  
    //float3 WorldSpacePos = mul(objTrans, IN.pos).xyz;   
  
    // -----------------------------------------------------------------   
    // Texture Coordinate   
    // -----------------------------------------------------------------   
  
    OUT.TexCoord = IN.uv0;   

    // -----------------------------------------------------------------   
    // Vertex Position   
    // -----------------------------------------------------------------       
  
    OUT.Position = mul(modelview, float4(IN.pos.xyz,1));       
	//OUT.Position = mul(modelview, float4(WorldSpacePos.xyz,1));       
  
    return OUT;     
}  
