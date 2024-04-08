#pragma once
#include <string>
#include <glm\glm.hpp>
#include "RenderCommand.h"
namespace DeeDeeEngine {
	class Shader {
	public:
		virtual ~Shader() = default;//������һ������ָ��ָ��һ�����������ʱ�����ɾ����ָ�룬����������ȷ��������ȷ���������������ٶ���
		//����������������������ģ���ôֻ�л�������������ᱻ���ã�����ܵ������������Դû�б���ȷ�ͷš�

		virtual void Bind() const =0;
		virtual void Unbind() const =0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	private:
		uint32_t m_RendererID;
	};
}