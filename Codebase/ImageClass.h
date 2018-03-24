#pragma once
#ifndef _IMAGECLASS_
#define _IMAGECLASS_

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class ImageClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	ImageClass();
	~ImageClass();

	bool Initialise(ID3D11Device*, int, int, LPCWSTR, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, LPCWSTR);
	void ReleaseTexture();

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount, m_indexCount;
	//TextureClass* m_Texture;

	int m_screenWidth, m_screenHeight;
	int m_imageWidth, m_imageHeight;
	int m_previousPosX, m_previousPosY;
};

#endif // !_IMAGECLASS_
