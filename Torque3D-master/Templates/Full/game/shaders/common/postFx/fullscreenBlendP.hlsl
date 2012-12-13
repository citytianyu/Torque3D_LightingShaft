
uniform float Strength;

sampler2D BlendTexture;

float4 main(in float2 uv : TEXCOORD0) : COLOR
{
	float4 OutColor = tex2D(BlendTexture, uv) * Strength;
	return OutColor;
}