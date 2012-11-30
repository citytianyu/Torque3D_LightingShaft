#include "torque.hlsl"     
     
// DECLARE THE SAMPLER CONTAIN THE TEXTURES     
uniform sampler2D DiffSampler: register( S0 );      
     
struct ConnectData              
{              
   float4 Position        : POSITION;         
   float2 TexCoord        : TEXCOORD0;        
};        
     
float4 main( ConnectData IN ) : COLOR       
{     
   // IMPORT ALL REQUIRED VECTORS FROM THE VERTEX SHADER     
   //float2 TexUV = IN.TexCoord.xy;     
     
   // RETRIEVE THE DIFFUSE COLOR  FOR THE CURRENT PIXEL USING THE DECLARED SAMPLER     
   //float4 Diff = tex2D(DiffSampler, TexUV);
   const float intensity = 0.8f;
   float4 Diff = float4(intensity.xxx, 1);
   
   // MERGE ALL ON THE FINAL RESULT     
   float4 OutColor = 0;         
   OutColor = Diff;          
     
   return OutColor;
   //return hdrEncode(OutColor);     
}  
