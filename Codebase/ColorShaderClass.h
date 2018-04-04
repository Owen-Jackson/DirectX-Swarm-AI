#pragma once
#ifndef _COLORSHADERCLASS_
#define _COLORSHADERCLASS_

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

class ColorShaderClass
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	ColorShaderClass();
	~ColorShaderClass();

	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();
	bool SetShaderParameters(ID3D11DeviceContext*, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&);
	bool Render(ID3D11DeviceContext*, int, int);

private:
	bool InitialiseShader(ID3D11Device*, HWND, LPCWSTR, LPCWSTR);
	void ShutdownShader();

	void RenderShader(ID3D11DeviceContext*, int, int);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};
#endif // !_COLORSHADERCLASS_
