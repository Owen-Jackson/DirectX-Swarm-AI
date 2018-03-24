cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	matrix worldMat : WORLDMAT;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

//Vertex shader
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	//calculate the position of the vertex against the world, view and projection matrices
	matrix viewProj = mul(viewMatrix, projectionMatrix);
	output.position = mul(input.position, input.worldMat);
	output.position = mul(output.position, viewProj);

	//WORKING CODE
	//output.position = mul(input.position, worldMatrix);
	//output.position = mul(output.position, viewMatrix);
	//output.position = mul(output.position, projectionMatrix);

	//store the output color for the pixel shader to use
	output.color = input.color;

	return output;
}