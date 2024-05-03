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

		// ����һ��OpenGL�������
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		// Ϊ����������洢�ռ�
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		// ����������˲���
		//�����˵�����ӳ�䵽һ�����������С����״��ʱ��OpenGL��δ�����������أ�Ҳ��Ϊ����Ԫ�ػ�texels����
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	// ���캯�������ڴ���һ��OpenGLTexture2D����
	DeeDeeEngine::OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		DEE_PROFILE_FUNCTION();

		// �����ȡ��߶Ⱥ�ͨ��������
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			DEE_PROFILE_SCOPE("stbi_load -OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
			// ����ͼƬ����
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
		// ��ͼ�������ϴ���GPU
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	// ��������������ɾ��OpenGL�������
	DeeDeeEngine::OpenGLTexture2D::~OpenGLTexture2D()
	{
		DEE_PROFILE_FUNCTION();

		// ɾ������
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		DEE_PROFILE_FUNCTION();

		// �������ݸ�ʽȷ��ÿ���ص��ֽڴ�С��bpp��ʾbytes per pixel��
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3; // ������ݸ�ʽ��GL_RGBA����bppΪ4������Ϊ3

		// ����ȷ���ṩ�����ݴ�С�������Ԥ�ڴ�С��ƥ��
		DEE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");

		// �����������ͼ�񣬼������ݸ��Ƶ�GPU�������ڴ���
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
		// �������ͣ�
		// m_RendererID - �����Ψһ��ʶ��
		// 0 - Mipmap�ļ���0��ʾ��������
		// 0, 0 - x��y��ƫ�������������������½ǿ�ʼ����
		// m_Width, m_Height - Ҫ���µ�����Ŀ�Ⱥ͸߶�
		// m_DataFormat - �������ݵĸ�ʽ
		// GL_UNSIGNED_BYTE - ���ݵ����ͣ�����ָ��Ϊ�޷����ֽ�
		// data - ָ�����ݵ�ָ��
	}

	// ����������Ԫ
	void DeeDeeEngine::OpenGLTexture2D::Bind(uint32_t slot) const
	{
		DEE_PROFILE_FUNCTION();

		// ������Ĭ�ϵ�����Ԫ��0�ţ�
		glBindTextureUnit(slot, m_RendererID);
	}
}

