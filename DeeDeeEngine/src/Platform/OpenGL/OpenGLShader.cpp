#include "deepch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <fstream>
#include <DeeDeeEngine/Core/Log.h>
#include <glm/gtc/type_ptr.hpp>
namespace DeeDeeEngine {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		DEE_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		DEE_PROFILE_FUNCTION();
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}
	OpenGLShader::OpenGLShader(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc)
	:m_Name(name){
		DEE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		DEE_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
		DEE_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		DEE_CORE_ASSERT(shaderSources.size() <= 2, "We Only support 2 shaders for now!");
		std::array<GLenum,2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			//获取着色器对象的编译状态
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				//用于获取着色器对象的信息日志的长度
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				//如果查询成功，信息日志将被存储在infoLog所指向的缓冲区中
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);
				DEE_CORE_ERROR("Vertex shader compilation failure!");
				DEE_CORE_ERROR("{0}", infoLog.data());
				// Use the infoLog as you see fit.

				// In this simple program, we'll just leave
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] =shader;
		}
		 

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);
		
			DEE_CORE_ERROR("Shader link failure!");
			DEE_CORE_ERROR("{0}", infoLog.data());
			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;


	}
	void OpenGLShader::Bind()const {
		DEE_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind()const {
		DEE_PROFILE_FUNCTION();

		glUseProgram(0);
	}
	void OpenGLShader::SetInt(const std::string name, const int value)
	{
		DEE_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}
	void OpenGLShader::SetIntArray(const std::string name, int* values, uint32_t count)
	{
		DEE_PROFILE_FUNCTION();

		UploadUniformIntArray(name, values,count);
	}
	void OpenGLShader::SetFloat(const std::string& name, const float value)
	{
		DEE_PROFILE_FUNCTION();
		UploadUniformFloat(name, value);
	}
	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		DEE_PROFILE_FUNCTION();
		UploadUniformFloat2(name, value);
	}
	void OpenGLShader::SetFloat3(const std::string& name,const glm::vec3& value)
	{
		DEE_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		DEE_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		DEE_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		DEE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		DEE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location,count, values);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		DEE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		DEE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		DEE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		DEE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		DEE_PROFILE_FUNCTION();

		const char* nameCString = name.c_str();
		GLint location = glGetUniformLocation(m_RendererID, nameCString);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		DEE_PROFILE_FUNCTION();

		const char* nameCString = name.c_str();
		GLint location = glGetUniformLocation(m_RendererID, nameCString);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));//glm::value_ptr(matrix)用于获取矩阵数据的指针，这是因为OpenGL函数需要接收指向数据的指针。
	}
	// 定义OpenGLShader类的ReadFile函数，它接受一个文件路径作为参数并返回文件内容的字符串。
	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		DEE_PROFILE_FUNCTION();

		// 创建一个用于存储文件内容的字符串。
		std::string result;

		// 打开文件：创建一个输入文件流'in'，以二进制读取模式打开路径为'filepath'的文件。
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // 注意：这里应该是位或运算符'|'，而不是逗号','。

		// 检查文件是否成功打开。
		if (in) {
			// 将文件指针移动到文件末尾。
			in.seekg(0, std::ios::end);

			// 根据文件指针当前位置（即文件大小）调整结果字符串的大小。
			result.resize(in.tellg());

			// 将文件指针重新定位到文件的开始。
			in.seekg(0, std::ios::beg);

			// 从文件中读取内容到'result'字符串中，读取的大小为'result'的大小。
			in.read(&result[0], result.size());

			// 关闭文件流。
			in.close();
		}
		else {
			// 如果文件无法打开，记录错误信息。注意：这里的字符串格式化似乎不完整。
			DEE_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		// 返回文件内容。
		return result;
	}


	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		DEE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			DEE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			DEE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			DEE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}

}