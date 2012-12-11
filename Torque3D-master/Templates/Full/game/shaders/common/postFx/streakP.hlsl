// Samplers
sampler src_texture;
sampler prev_streak;

//#define NUM_SAMPLES  8    // Maximum texture grabs

// Constants
float  prev_mul;
// Contains sampling offsets used by the techniques
//float2 sample_offsets[NUM_SAMPLES];
//float3 sample_weights[NUM_SAMPLES];  // coloured!


//-----------------------------------------------------------------------------
// Blur the source image along one axis using a gaussian
// distribution. Since gaussian blurs are separable, this shader is called 
// twice; first along the horizontal axis, then along the vertical axis.
//-----------------------------------------------------------------------------
float4 main(in float2 uv : TEXCOORD0) : COLOR
{
  float4 sample = 0;

  for (int i=0; i < NUM_SAMPLES; i++)	{
    sample += float4(sample_weights[i],1) * tex2D(src_texture, uv + sample_offsets[i]);
  }
	sample += prev_mul*tex2D(prev_streak, uv);

  return sample;
}
