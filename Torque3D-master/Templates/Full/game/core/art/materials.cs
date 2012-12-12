//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

singleton Material( DefaultDecalRoadMaterial )
{
   diffuseMap[0] = "core/art/defaultRoadTextureTop.png";
   mapTo = "unmapped_mat";
   materialTag0 = "RoadAndPath";
};

singleton Material( BlankWhite )
{
   diffuseMap[0] = "core/art/white";
   mapTo = "white";
   materialTag0 = "Miscellaneous";
};

singleton Material( Empty )
{
};

singleton Material(DefaultRoadMaterialTop)
{
   mapTo = "unmapped_mat";
   diffuseMap[0] = "core/art/defaultRoadTextureTop.png";
   materialTag0 = "RoadAndPath";
};

singleton Material(DefaultRoadMaterialOther)
{
   mapTo = "unmapped_mat";
   diffuseMap[0] = "core/art/defaultRoadTextureOther.png";
   materialTag0 = "RoadAndPath";
};

singleton GFXStateBlockData( TestScatteringBlock )
{
   blendDefined = true;
   blendEnable = true;
   blendSrc = GFXBlendOne;
   blendDest = GFXBlendOne;
   blendOp = GFXBlendOpAdd;
   
   zDefined = true;
   zEnable = false;
   zWriteEnable = false;
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
   stateBlock = TestScatteringBlock;
};

/*
singleton CustomMaterial(DownsampleCutoffMat)
{
	mapTo = "unmapped_mat";
	texture[0] = "$backBuffer";
	shader = DownsampleCutoffShader;
}

singleton CustomMaterial(UpStreak0Mat)
{
	mapTo = "unmapped_mat";
	texture[0] = "#waterGlowBuffer";
	shader = StreakShader;
}

singleton CustomMaterial(WaterGlowMat)
{
   mapTo = "unmapped_mat";
   diffuseMap[0] = "core/art/warnmat";
   castShadows = "0";
   translucent = "0";
   materialTag0 = "Miscellaneous";
   diffuseColor[0] = "0.992157 0.992157 0.992157 1";
   
   sampler["DiffSampler"] = "core/art/warnmat";  
   shader = TestScatteringShader;
   stateBlock = TestScatteringBlock;
   
   pass[0] = DownsampleCutoffMat;
   
}
*/
