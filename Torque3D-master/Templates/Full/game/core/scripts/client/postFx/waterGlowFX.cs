
singleton ShaderData(GlowCutoffShader)
{
   DXVertexShaderFile 	= "shaders/common/postFx/postFxV.hlsl";
   DXPixelShaderFile 	= "shaders/common/postFx/glow_cutoffP.hlsl";
            
   samplerNames[0] = "$src_texture";
   
   pixVersion = 2.0;
};


singleton ShaderData(StreakShader)
{
   DXVertexShaderFile 	= "shaders/common/postFx/postFxV.hlsl";
   DXPixelShaderFile 	= "shaders/common/postFx/streakP.hlsl";
            
   samplerNames[0] = "$src_texture";
   samplerNames[1] = "$prev_streak";
   
   pixVersion = 2.0;
};

singleton ShaderData(UpStreak0Shader : StreakShader)
{
	defines = "int NUM_SAMPLES = 8;float2 sample_offsets[NUM_SAMPLES];float3 sample_weights[NUM_SAMPLES];sample_offsets[0] = {0, 0};sample_offsets[1] = {0.001, 0.004};sample_offsets[2] = {0.002, 0.008};sample_offsets[3] = {0.003, 0.012};sample_offsets[4] = {0.004, 0.016};sample_offsets[5] = {0.005, 0.020};sample_offsets[6] = {0.006, 0.023};sample_offsets[7] = {0.007, 0.027};sample_weights[0] = {0.315, 0.315, 0.315};sample_weights[1] = {0.297, 0.297, 0.297};sample_weights[2] = {0.281, 0.281, 0.281};sample_weights[3] = {0.265, 0.265, 0.265};sample_weights[4] = {0.250, 0.250, 0.250};sample_weights[5] = {0.236, 0.236, 0.236};sample_weights[6} = {0.223, 0.223, 0.223};sample_weights{7} = {0.210, 0.210, 0.210};";
};

$WaterGlowFx::cutoff = 0.85;
function WaterGlowFx::setShaderConsts(%this)
{
	%this.setShaderConst("$cutoff", $WaterGlowFx::cutoff);
}

singleton PostEffect(WaterGlowFx)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = GlowCutoffShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "$backBuffer";
   target = "#downsampledWaterGlow";
   targetScale = "0.25 0.25";
   
   renderPriority = 4;
   isEnabled = true;

   new PostEffect()
   {
		internalName = "BlurUp_0";
		shader = UpStreak0Shader;
		stateBlock = PFX_DefaultStateBlock;
		texture[0] = "#downsampledWaterGlow";
		target = "$outTex";
   };
  /*
   new PostEffect()
   {
		internalName = "Composite";
		shader = PFX_PassthruShader;
		stateBlock = PFX_DefaultStateBlock;
		texture[0] = "#downsampledWaterGlow";
		target = "$backBuffer";
   };
*/
};