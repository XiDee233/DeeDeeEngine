#include "deepch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
namespace DeeDeeEngine {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_Width(width), m_Height(height)
	{
		DEE_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		// 创建一个OpenGL纹理对象
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		// 为纹理对象分配存储空间
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		// 设置纹理过滤参数
		//决定了当纹理被映射到一个比它大或者小的形状上时，OpenGL如何处理纹理的像素（也称为纹理元素或texels）。
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	// 构造函数，用于创建一个OpenGLTexture2D对象
	DeeDeeEngine::OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		DEE_PROFILE_FUNCTION();

		// 定义宽度、高度和通道数变量
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			DEE_PROFILE_SCOPE("stbi_load -OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
			// 加载图片数据
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
			m_IsLoaded = true;
		m_Width = width;
		m_Height = height;
		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		DEE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// 将图像数据上传到GPU
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	// 析构函数，用于删除OpenGL纹理对象
	DeeDeeEngine::OpenGLTexture2D::~OpenGLTexture2D()
	{
		DEE_PROFILE_FUNCTION();

		// 删除纹理
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		DEE_PROFILE_FUNCTION();

		// 根据数据格式确定每像素的字节大小（bpp表示bytes per pixel）
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3; // 如果数据格式是GL_RGBA，则bpp为4，否则为3

		// 断言确保提供的数据大小与纹理的预期大小相匹配
		DEE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");

		// 更新纹理的子图像，即将数据复制到GPU的纹理内存中
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		// 参数解释：
		// m_RendererID - 纹理的唯一标识符
		// 0 - Mipmap的级别，0表示基本级别
		// 0, 0 - x和y的偏移量，这里从纹理的左下角开始更新
		// m_Width, m_Height - 要更新的纹理的宽度和高度
		// m_DataFormat - 纹理数据的格式
		// GL_UNSIGNED_BYTE - 数据的类型，这里指定为无符号字节
		// data - 指向数据的指针
	}

	// 绑定纹理到纹理单元
	void DeeDeeEngine::OpenGLTexture2D::Bind(uint32_t slot) const
	{
		DEE_PROFILE_FUNCTION();

		// 绑定纹理到默认的纹理单元（0号）
		glBindTextureUnit(slot, m_RendererID);
	}
}

