#include "deepch.h"
#include "OpenGLTexture.h"
#include <glad/glad.h>
#include "stb_image.h"
namespace DeeDeeEngine {

	// ���캯�������ڴ���һ��OpenGLTexture2D����
	DeeDeeEngine::OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		// �����ȡ��߶Ⱥ�ͨ��������
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		// ����ͼƬ����
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		// ���ԣ�ȷ��ͼƬ���ݼ��سɹ�
		DEE_CORE_ASSERT(data, "Failed to load image!");
		// ��������Ŀ�Ⱥ͸߶�
		m_Width = width;
		m_Height = height;

		// ����һ��OpenGL�������
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		// Ϊ����������洢�ռ�
		glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

		// ����������˲���
		//�����˵�����ӳ�䵽һ�����������С����״��ʱ��OpenGL��δ�����������أ�Ҳ��Ϊ����Ԫ�ػ�texels����
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// ��ͼƬ�����ϴ���GPU
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

		// �ͷż��ص�ͼƬ����
		stbi_image_free(data);
	}

	// ��������������ɾ��OpenGL�������
	DeeDeeEngine::OpenGLTexture2D::~OpenGLTexture2D()
	{
		// ɾ������
		glDeleteTextures(1, &m_RendererID);
	}

	// ����������Ԫ
	void DeeDeeEngine::OpenGLTexture2D::Bind(uint32_t slot) const
	{
		// ������Ĭ�ϵ�����Ԫ��0�ţ�
		glBindTextureUnit(slot, m_RendererID);
	}
}

