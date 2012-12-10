
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
		shader = StreakShader;
		stateBlock = PFX_DefaultStateBlock;
		texture[0] = "#downsampledWaterGlow";
		target = "inTex";
   };
   
   new PostEffect()
   {
		internalName = "Composite";
		shader = PFX_PassthruShader;
		stateBlock = PFX_DefaultStateBlock;
		texture[0] = "#downsampledWaterGlow";
		target = "$backBuffer";
   };
};