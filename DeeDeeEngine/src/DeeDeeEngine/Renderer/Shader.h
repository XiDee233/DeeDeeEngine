#pragma once
#include <string>
#include <glm\glm.hpp>
#include "RenderCommand.h"
namespace DeeDeeEngine {
	class Shader {
	public:
		virtual ~Shader() = default;//当你有一个基类指针指向一个派生类对象时，如果删除该指针，虚析构函数确保调用正确的析构函数来销毁对象。
		//如果基类的析构函数不是虚的，那么只有基类的析构函数会被调用，这可能导致派生类的资源没有被正确释放。

		virtual void Bind() const =0;
		virtual void Unbind() const =0;

		virtual const std::string& GetName() const = 0;
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc);
	private:
		uint32_t m_RendererID;
	};

	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name,const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Get(const std::string& name);
		bool Exists(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}