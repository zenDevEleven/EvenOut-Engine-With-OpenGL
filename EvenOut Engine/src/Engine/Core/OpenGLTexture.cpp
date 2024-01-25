#include "epch.h"
#include "Engine/LogSystem/Log.h"
#include "OpenGLTexture.h"

namespace Engine
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		LoadTexture(path);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, TextureData rect, TextureData posRect)
		: m_Path(path)
	{
		LoadTexture(path);
	}


	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::LoadTexture(const std::string& path)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		unsigned int internalFormat, dataFormat;
		int width, height, channels;

		stbi_set_flip_vertically_on_load(0);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr) {
			LOG_CORE("Failed to load image: " + path, LOG_ERROR);
			return;
		}


		if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}


		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}


}