struct PixelInputType
{
	float4 position : SV_POSITION;
	//float3 worldPosition : WORLD;
	float4 color : COLOR;
};

//pixel shader
float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	return input.color;
}