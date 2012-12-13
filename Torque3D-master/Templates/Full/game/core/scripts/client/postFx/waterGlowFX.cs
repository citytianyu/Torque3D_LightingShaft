
singleton ShaderData(GlowCutoffShader)
{
   DXVertexShaderFile 	= "shaders/common/postFx/postFxV.hlsl";
   DXPixelShaderFile 	= "shaders/common/postFx/glow_cutoffP.hlsl";
            
   samplerNames[0] = "$src_texture";
   
   pixVersion = 2.0;
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
   
   renderPriority = 1510;
   isEnabled = true;
};

singleton ShaderData(StreakShader)
{
   DXVertexShaderFile 	= "shaders/common/postFx/postFxV.hlsl";
   DXPixelShaderFile 	= "shaders/common/postFx/streakP.hlsl";
            
   samplerNames[0] = "$src_texture";
   samplerNames[1] = "$prev_streak";
   
   pixVersion = 2.0;
};

$StreakUp0::prev_mul = 0;
$StreakUp0::offsets = "0 0 0 0	0.001 0.004 0 0	0.002 0.008 0 0	0.003 0.012 0 0	0.004 0.016 0 0	0.005 0.020 0 0	0.006 0.023 0 0	0.007 0.027 0 0";
$StreakUp0::weights = "0.315 0.315 0.315 0	0.297 0.297 0.297 0	0.281 0.281 0.281 0	0.265 0.265 0.265 0	0.250 0.250 0.250 0	0.236 0.236 0.236 0	0.223 0.223 0.223 0	0.210 0.210 0.210 0";
function StreakUp0::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakUp0::offsets);
	%this.setShaderConst("$sample_weights", $StreakUp0::weights);
	%this.setShaderConst("$prev_mul", $StreakUp0::prev_mul);
}

singleton PostEffect(StreakUp0)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "#downsampledWaterGlow";
   target = "$outTex";
   
   renderPriority = 1509;
   isEnabled = true;
};

$StreakUp1::prev_mul = 0;
$StreakUp1::offsets = "0 0 0 0	0.009 0.031 0 0	0.017 0.063 0 0	0.026 0.094 0 0	0.034 0.125 0 0	0.043 0.157 0 0	0.051 0.188 0 0	0.060 0.219 0 0";
$StreakUp1::weights = "0.608 0.608 0.608 0	0.415 0.362 0.362 0	0.274 0.221 0.221 0	0.152 0.139 0.156 0	0.088 0.088 0.109 0	0.055 0.057 0.064 0	0.035 0.039 0.035 0	0.023 0.024 0.023 0";
function StreakUp1::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakUp1::offsets);
	%this.setShaderConst("$sample_weights", $StreakUp1::weights);
	%this.setShaderConst("$prev_mul", $StreakUp1::prev_mul);
}

singleton PostEffect(StreakUp1)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "$inTex";
   target = "$outTex";
   
   renderPriority = 1508;
   isEnabled = true;
};

$StreakUp2::prev_mul = 0;
$StreakUp2::offsets = "0 0 0 0	0.001 0.004 0 0	0.002 0.008 0 0	0.003 0.012 0 0	0.004 0.016 0 0	0.005 0.020 0 0	0.006 0.023 0 0	0.007 0.027 0 0";
$StreakUp2::weights = "0.315 0.315 0.315 0	0.297 0.297 0.297 0	0.281 0.281 0.281 0	0.265 0.265 0.265 0	0.250 0.250 0.250 0	0.236 0.236 0.236 0	0.223 0.223 0.223 0	0.210 0.210 0.210 0";
function StreakUp2::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakUp2::offsets);
	%this.setShaderConst("$sample_weights", $StreakUp2::weights);
	%this.setShaderConst("$prev_mul", $StreakUp2::prev_mul);
}

singleton PostEffect(StreakUp2)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "$inTex";
   target = "#waterGlowBuffer";
   
   renderPriority = 1507;
   isEnabled = true;
};

$StreakLeft0::prev_mul = 0;
$StreakLeft0::offsets = "0 0 0 0	0.003 -0.001 0 0	0.006 -0.003 0 0	0.009 -0.004 0 0	0.012 -0.006 0 0	0.015 -0.007 0 0	0.018 -0.009 0 0	0.021 -0.010 0 0";
$StreakLeft0::weights = "0.315 0.315 0.315 0	0.297 0.297 0.297 0	0.281 0.281 0.281 0	0.265 0.265 0.265 0	0.250 0.250 0.250 0	0.236 0.236 0.236 0	0.223 0.223 0.223 0	0.210 0.210 0.210 0";
function StreakLeft0::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakLeft0::offsets);
	%this.setShaderConst("$sample_weights", $StreakLeft0::weights);
	%this.setShaderConst("$prev_mul", $StreakLeft0::prev_mul);
}

singleton PostEffect(StreakLeft0)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "#downsampledWaterGlow";
   target = "$outTex";
   
   renderPriority = 1506;
   isEnabled = true;
};

$StreakLeft1::prev_mul = 0;
$StreakLeft1::offsets = "0 0 0 0	0.023 -0.011 0 0	0.047 -0.023 0 0	0.070 -0.034 0 0	0.094 -0.046 0 0	0.117 -0.057 0 0	0.141 -0.068 0 0	0.164 -0.080 0 0";
$StreakLeft1::weights = "0.608 0.608 0.608 0	0.415 0.362 0.362 0	0.274 0.221 0.221 0	0.152 0.139 0.156 0	0.088 0.088 0.109 0	0.055 0.057 0.064 0	0.035 0.039 0.035 0	0.023 0.024 0.023 0";
function StreakLeft1::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakLeft1::offsets);
	%this.setShaderConst("$sample_weights", $StreakLeft1::weights);
	%this.setShaderConst("$prev_mul", $StreakLeft1::prev_mul);
}

singleton PostEffect(StreakLeft1)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "$inTex";
   target = "$outTex";
   
   renderPriority = 1505;
   isEnabled = true;
};

$StreakLeft2::prev_mul = 1;
$StreakLeft2::offsets = "0 0 0 0	0.188 -0.091 0 0	0.376 -0.182 0 0	0.564 -0.271 0 0	0.752 -0.365 0 0	0 0 0 0	0 0 0 0	0 0 0 0";
$StreakLeft2::weights = "0.880 0.880 0.880 0	0.025 0.019 0.019 0	0.001 0 0 0	0 0 0 0	0 0 0 0	0 0 0 0	0 0 0 0	0 0 0 0";
function StreakLeft2::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakLeft2::offsets);
	%this.setShaderConst("$sample_weights", $StreakLeft2::weights);
	%this.setShaderConst("$prev_mul", $StreakLeft2::prev_mul);
}

singleton PostEffect(StreakLeft2)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "$inTex";
   texture[1] = "#waterGlowBuffer";
   target = "#waterGlowBuffer1";
   
   renderPriority = 1504;
   isEnabled = true;
};

$StreakDown0::prev_mul = 0;
$StreakDown0::offsets = "0 0 0 0	-0.001 -0.004 0 0	-0.002 -0.008 0 0	-0.003 -0.012 0 0	-0.004 -0.016 0 0	-0.005 -0.020 0 0	-0.006 -0.023 0 0	-0.007 -0.027 0 0";
$StreakDown0::weights = "0.315 0.315 0.315 0	0.297 0.297 0.297 0	0.281 0.281 0.281 0	0.265 0.265 0.265 0	0.250 0.250 0.250 0	0.236 0.236 0.236 0	0.223 0.223 0.223 0	0.210 0.210 0.210 0";
function StreakDown0::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakDown0::offsets);
	%this.setShaderConst("$sample_weights", $StreakDown0::weights);
	%this.setShaderConst("$prev_mul", $StreakDown0::prev_mul);
}

singleton PostEffect(StreakDown0)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "#downsampledWaterGlow";

   target = "$outTex";
   
   renderPriority = 1503;
   isEnabled = true;
};

$StreakDown1::prev_mul = 0;
$StreakDown1::offsets = "0 0 0 0	-0.009 -0.031 0 0	-0.017 -0.063 0 0	-0.026 -0.094 0 0	-0.034 -0.125 0 0	-0.043 -0.157 0 0	-0.051 -0.188 0 0	-0.060 -0.219 0 0";
$StreakDown1::weights = "0.608 0.608 0.608 0	0.415 0.362 0.362 0	0.274 0.221 0.221 0	0.152 0.139 0.156 0	0.088 0.088 0.109 0	0.055 0.057 0.064 0	0.035 0.039 0.035 0	0.023 0.024 0.023 0";
function StreakDown1::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakDown1::offsets);
	%this.setShaderConst("$sample_weights", $StreakDown1::weights);
	%this.setShaderConst("$prev_mul", $StreakDown1::prev_mul);
}

singleton PostEffect(StreakDown1)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "$inTex";

   target = "$outTex";
   
   renderPriority = 1502;
   isEnabled = true;
};

$StreakDown2::prev_mul = 1;
$StreakDown2::offsets = "0 0 0 0	-0.068 -0.251 0 0	-0.137 -0.501 0 0	-0.205 -0.752 0 0	0 -0.125 0 0	0 0 0 0	0 0 0 0	0 0 0 0";
$StreakDown2::weights = "0.880 0.880 0.880 0	0.025 0.019 0.019 0	0.001 0 0 0	0 0 0 0	0 0 0 0	0 0 0 0	0 0 0 0	0 0 0 0";
function StreakDown2::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakDown2::offsets);
	%this.setShaderConst("$sample_weights", $StreakDown2::weights);
	%this.setShaderConst("$prev_mul", $StreakDown2::prev_mul);
}

singleton PostEffect(StreakDown2)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "$inTex";
   texture[1] = "#waterGlowBuffer1";
   target = "#waterGlowBuffer2";
   
   renderPriority = 1501;
   isEnabled = true;
};


$StreakRight0::prev_mul = 0;
$StreakRight0::offsets = "0 0 0 0	-0.003 0.001 0 0	-0.006 0.003 0 0	-0.009 0.004 0 0	-0.012 0.006 0 0	-0.015 0.007 0 0	-0.018 0.009 0 0	-0.021 0.010 0 0";
$StreakRight0::weights = "0.315 0.315 0.315 0	0.297 0.297 0.297 0	0.281 0.281 0.281 0	0.265 0.265 0.265 0	0.250 0.250 0.250 0	0.236 0.236 0.236 0	0.223 0.223 0.223 0	0.210 0.210 0.210 0";
function StreakRight0::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakRight0::offsets);
	%this.setShaderConst("$sample_weights", $StreakRight0::weights);
	%this.setShaderConst("$prev_mul", $StreakRight0::prev_mul);
}

singleton PostEffect(StreakRight0)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "#downsampledWaterGlow";
   target = "$outTex";
   
   renderPriority = 1500;
   isEnabled = true;
};

$StreakRight1::prev_mul = 0;
$StreakRight1::offsets = "0 0 0 0	-0.023 0.011 0 0	-0.047 0.023 0 0	-0.070 0.034 0 0	-0.094 0.046 0 0	-0.117 0.057 0 0	-0.141 0.068 0 0	-0.164 0.080 0 0";
$StreakRight1::weights = "0.608 0.608 0.608 0	0.415 0.362 0.362 0	0.274 0.221 0.221 0	0.152 0.139 0.156 0	0.088 0.088 0.109 0	0.055 0.057 0.064 0	0.035 0.039 0.035 0	0.023 0.024 0.023 0";
function StreakRight1::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakRight1::offsets);
	%this.setShaderConst("$sample_weights", $StreakRight1::weights);
	%this.setShaderConst("$prev_mul", $StreakRight1::prev_mul);
}

singleton PostEffect(StreakRight1)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "$inTex";
   target = "$outTex";
   
   renderPriority = 1499;
   isEnabled = true;
};

$StreakRight2::prev_mul = 1;
$StreakRight2::offsets = "0 0 0 0	-0.188 0.091 0 0	-0.376 0.182 0 0	-0.564 0.274 0 0	-0.752 0.365 0 0	0 0 0 0	0 0 0 0	0 0 0 0";
$StreakRight2::weights = "0.880 0.880 0.880 0	0.025 0.019 0.019 0	0.001 0 0 0	0 0 0 0	0 0 0 0	0 0 0 0	0 0 0 0	0 0 0 0";

singleton GFXStateBlockData( waterGlowCompositeBlock : PFX_DefaultStateBlock )
{   
   blendDefined = true;
   blendEnable = true; 
   blendSrc = GFXBlendOne;
   blendDest = GFXBlendOne;
};


function StreakRight2::setShaderConsts(%this)
{
	%this.setShaderConst("$sample_offsets", $StreakRight2::offsets);
	%this.setShaderConst("$sample_weights", $StreakRight2::weights);
	%this.setShaderConst("$prev_mul", $StreakRight2::prev_mul);
}

singleton PostEffect(StreakRight2)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = StreakShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "$inTex";
   texture[1] = "#waterGlowBuffer2";
   target = "#waterGlowBuffer3";

   renderPriority = 1498;
   isEnabled = true;
   
   new PostEffect()
   {
		shader = PFX_PassthruShader;
		stateBlock = waterGlowCompositeBlock;
		texture[0] = "#waterGlowBuffer3";
		target = "$backBuffer";
   };
};