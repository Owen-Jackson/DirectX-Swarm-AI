#include "ImageClass.h"

ImageClass::ImageClass()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	//m_Texture = nullptr;
}

ImageClass::~ImageClass()
{
}

bool ImageClass::Initialise(ID3D11Device* device, int screenWidth, int screenHeight, LPCWSTR fileName, int imageWidth, int imageHeight)
{
	bool result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_imageWidth = imageWidth;
	m_imageHeight = imageHeight;

	m_previousPosX = -1;
	m_previousPosY = -1;

	//initialise the vertex and index buffers
	result = InitialiseBuffers(device);
	if (!result)
	{
		return false;
	}


}