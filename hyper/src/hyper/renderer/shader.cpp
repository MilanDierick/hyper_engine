#include "hyper/renderer/shader.h"

#include "hyper/renderer/renderer.h"
#include "platform/opengl/opengl_shader.h"

namespace hp
{
	std::shared_ptr<shader> shader::create(const std::string& filepath)
	{
		switch (renderer::get_api())
		{
			case renderer_api::API::None:
			HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
				return nullptr;
			case renderer_api::API::OpenGL: return std::make_shared<opengl_shader>(filepath);
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
	
	std::shared_ptr<shader> shader::create(
		const std::string& name, const std::string& vertex_src, const std::string& fragment_src)
	{
		switch (renderer::get_api())
		{
			case renderer_api::API::None:
			HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
				return nullptr;
			case renderer_api::API::OpenGL: return std::make_shared<opengl_shader>(name, vertex_src, fragment_src);
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
	
	void ShaderLibrary::add(const std::string& name, const std::shared_ptr<shader>& shader)
	{
		HP_CORE_ASSERT(!exists(name), "shader already exists!");
		m_shaders[name] = shader;
	}
	
	void ShaderLibrary::add(const std::shared_ptr<shader>& shader)
	{
		auto& name = shader->get_name();
		add(name, shader);
	}
	
	std::shared_ptr<shader> ShaderLibrary::load(const std::string& filepath)
	{
		auto shader = shader::create(filepath);
		add(shader);
		return shader;
	}
	
	std::shared_ptr<shader> ShaderLibrary::load(const std::string& name, const std::string& filepath)
	{
		auto shader = shader::create(filepath);
		add(name, shader);
		return shader;
	}
	
	std::shared_ptr<shader> ShaderLibrary::get(const std::string& name)
	{
		HP_CORE_ASSERT(exists(name), "shader not found!");
		return m_shaders[name];
	}
	
	bool ShaderLibrary::exists(const std::string& name) const
	{
		return m_shaders.find(name) != m_shaders.end();
	}
	
}