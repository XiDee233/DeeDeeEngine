#include "deepch.h"
#include "OpenGLFramebuffer.h"
#include <glad\glad.h>
namespace DeeDeeEngine {
	static const uint32_t s_MaxFrameBufferSize = 8192;

	namespace Utils {

		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}

			return false;
		}

		static GLenum DeeFBTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			DEE_CORE_ASSERT(false);
			return 0;
		}

	}
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		:m_Specification(spec)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}
	void OpenGLFramebuffer::Invalidate()
	{
		// 如果已经有一个渲染器ID，先删除现有的帧缓冲区和纹理
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID); // 删除帧缓冲区
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data()); // 删除颜色附件纹理
			glDeleteTextures(1, &m_DepthAttachment); // 删除深度附件纹理

			m_ColorAttachments.clear(); // 清除颜色附件数组
			m_DepthAttachment = 0; // 重置深度附件ID
		}

		// 创建一个新的帧缓冲区对象，并将其ID存储在m_RendererID变量中
		glCreateFramebuffers(1, &m_RendererID); // 创建帧缓冲区
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID); // 绑定帧缓冲区

		bool multisample = m_Specification.Samples > 1; // 检查是否使用多重采样

		// 如果有颜色附件规格，创建和绑定颜色附件纹理
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size()); // 根据规格调整颜色附件数组大小
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size()); // 创建纹理

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]); // 绑定纹理
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8: // 如果是RGBA8格式
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i); // 将纹理作为颜色附件
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}

		// 如果深度附件规格不是None，创建和绑定深度附件纹理
		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1); // 创建深度纹理
			Utils::BindTexture(multisample, m_DepthAttachment); // 绑定深度纹理
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8: // 如果是DEPTH24STENCIL8格式
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height); // 将纹理作为深度附件
				break;
			}
		}

		// 如果有多个颜色附件，设置绘制缓冲区
		if (m_ColorAttachments.size() > 1)
		{
			DEE_CORE_ASSERT(m_ColorAttachments.size() <= 4); // 断言颜色附件数量不超过4
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 }; // 定义绘制缓冲区
			glDrawBuffers(m_ColorAttachments.size(), buffers); // 设置绘制缓冲区
		}
		else if (m_ColorAttachments.empty()) // 如果没有颜色附件
		{
			// 只进行深度绘制
			glDrawBuffer(GL_NONE); // 设置绘制缓冲区为None
		}

		// 检查帧缓冲区状态是否完整
		DEE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 解绑帧缓冲区
	}
	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}
	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width ==0 || height ==0 ||width>s_MaxFrameBufferSize||height>s_MaxFrameBufferSize)
		{
			DEE_CORE_WARN("Attempted to resize framebuffer to {0},{1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}
	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		DEE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
		//在一个典型的场景中，每个可选择的实体在渲染到帧缓冲区时会被赋予一个唯一的ID。
		//这个ID会被编码为像素的颜色值，通常是在不可见的颜色附件中。当用户点击屏幕时，
		// 你可以使用鼠标的屏幕坐标来读取该位置的像素值，然后解码这个值来确定哪个实体被选择了。

		//例如，如果场景中有多个实体，每个实体都有一个唯一的ID（比如1, 2, 3等），
		//这些ID可以被渲染到一个颜色附件中。当用户点击屏幕时，ReadPixel 函数会被调用来读取鼠标位置的像素值，
		// 这个值就对应于被点击实体的ID。如果读取的值是 - 1，则表示没有实体被选中。

	}
	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		DEE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
			Utils::DeeFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}

}