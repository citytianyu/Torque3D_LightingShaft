
//------------------------------------------------------------------------------
// UnderwaterScatteringFX
//------------------------------------------------------------------------------

$UwScatteringFx::attenuation_c = "0.451 0.171 0.112";
$UwScatteringFx::water_altitude = 3.831;
$UwScatteringFx::HGg = "0.75 1.25 1";
$UwScatteringFx::intensity = "0.032 0.028 0.024";

function UwScatteringFx::setShaderConsts( %this )
{
	%this.setShaderConst("$attenuation_c", $UwScatteringFx::attenuation_c);
	%this.setShaderConst("$water_altitude", $UwScatteringFx::water_altitude);
	%this.setShaderConst("$HGg", $UwScatteringFx::HGg);
	%this.setShaderConst("$intensity", $UwScatteringFx::intensity);
}

singleton ShaderData( UwScatteringFxShader )
{   
   DXVertexShaderFile 	= "shaders/common/postFx/uw_scatteringV.hlsl";
   DXPixelShaderFile 	= "shaders/common/postFx/uw_scatteringP.hlsl";
            
   samplerNames[0] = "$shafts_texture";
   
   pixVersion = 2.0;
};


singleton GFXStateBlockData( UwScatteringFxStateBlock : PFX_DefaultStateBlock )
{   
   blendDefined = true;
   blendEnable = true; 
   blendSrc = GFXBlendOne;
   blendDest = GFXBlendOne;
};


singleton PostEffect( UwScatteringFx )
{   
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = UwScatteringFxShader;
   stateBlock = PFX_DefaultStateBlock;
   texture[0] = "#underwaterScatteringBuffer";
   target = "#blurredUnderwaterScattering";
  
   renderPriority = 1402;
   isEnabled = true;
 /* 
   new PostEffect()
   {
	   internalName = "BlendScattering";
	   shader = PFX_PassthruShader;
	   stateBlock = UwScatteringFxStateBlock;
	   texture[0] = "$inTex";
	   target = "$backBuffer";
   };
 */
};


singleton ShaderData(uw_FullscreenBlendShader)
{
	DXVertexShaderFile 	= "shaders/common/postFx/postFxV.hlsl";
   DXPixelShaderFile 	= "shaders/common/postFx/fullscreenBlendP.hlsl";
            
   samplerNames[0] = "$BlendTexture";
   
   pixVersion = 2.0;
};

singleton GFXStateBlockData( underwaterScatteringCompositeBlock : PFX_DefaultStateBlock )
{   
   blendDefined = true;
   blendEnable = true; 
   blendSrc = GFXBlendOne;
   blendDest = GFXBlendOne;
};

$BlendUnderwaterScatteringFx::strength = 1.1;
function BlendUnderwaterScatteringFx::setShaderConsts(%this)
{
	%this.setShaderConst("$Strength", $BlendUnderwaterScatteringFx::strength);
}
singleton PostEffect(BlendUnderwaterScatteringFx)
{
   allowReflectPass = false;
      
   renderTime = "PFXAfterDiffuse";
   
   shader = uw_FullscreenBlendShader;
   stateBlock = underwaterScatteringCompositeBlock;
   texture[0] = "#blurredUnderwaterScattering";
   target = "$backBuffer";

   renderPriority = 1401;
   isEnabled = true;
};
