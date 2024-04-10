#include "deepch.h"
#include "OpenGLTexture.h"
#include <glad/glad.h>
#include "stb_image.h"
namespace DeeDeeEngine {

	// 构造函数，用于创建一个OpenGLTexture2D对象
	DeeDeeEngine::OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		// 定义宽度、高度和通道数变量
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		// 加载图片数据
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		// 断言，确保图片数据加载成功
		DEE_CORE_ASSERT(data, "Failed to load image!");
		// 设置纹理的宽度和高度
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		DEE_CORE_ASSERT(internalFormat&dataFormat, "Failed to load image!");

		// 创建一个OpenGL纹理对象
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		// 为纹理对象分配存储空间
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		// 设置纹理过滤参数
		//决定了当纹理被映射到一个比它大或者小的形状上时，OpenGL如何处理纹理的像素（也称为纹理元素或texels）。
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// 将图片数据上传到GPU
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		// 释放加载的图片数据
		stbi_image_free(data);
	}

	// 析构函数，用于删除OpenGL纹理对象
	DeeDeeEngine::OpenGLTexture2D::~OpenGLTexture2D()
	{
		// 删除纹理
		glDeleteTextures(1, &m_RendererID);
	}

	// 绑定纹理到纹理单元
	void DeeDeeEngine::OpenGLTexture2D::Bind(uint32_t slot) const
	{
		// 绑定纹理到默认的纹理单元（0号）
		glBindTextureUnit(slot, m_RendererID);
	}
}

