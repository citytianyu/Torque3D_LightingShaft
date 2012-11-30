singleton ShaderData( TestScatteringShader )   
{   
   DXVertexShaderFile     = "shaders/common/testV.hlsl";   
   DXPixelShaderFile      = "shaders/common/testP.hlsl";      
     
   pixVersion = 3.0;   
}; 


singleton CustomMaterial(TestScattering)
{
   mapTo = "unmapped_mat";
   diffuseMap[0] = "core/art/warnmat";
   castShadows = "0";
   translucent = "1";
   materialTag0 = "Miscellaneous";
   diffuseColor[0] = "0.992157 0.992157 0.992157 1";
   
   sampler["DiffSampler"] = "core/art/warnmat";  
   shader = TestScatteringShader;
};
