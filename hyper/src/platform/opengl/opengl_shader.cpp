#include "platform/opengl/opengl_shader.h"

#include "hyper/core/timer.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv-cross/spirv_cross.hpp>
#include <spirv-cross/spirv_glsl.hpp>

namespace hp
{
	namespace utils
	{
		static GLenum shader_type_from_string(const std::string& type)
		{
			if (type == "vertex")
			{
				return GL_VERTEX_SHADER;
			}
			if (type == "fragment" || type == "pixel")
			{
				return GL_FRAGMENT_SHADER;
			}
			
			HP_CORE_ASSERT(false, "Unknown shader type!")
			return 0;
		}
		
		static shaderc_shader_kind gl_shader_stage_to_shader_c(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			HP_CORE_ASSERT(false)
			return (shaderc_shader_kind)0;
		}
		
		static const char* gl_shader_stage_to_string(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			HP_CORE_ASSERT(false)
			return nullptr;
		}
		
		static const char* get_cache_directory()
		{
			// TODO: make sure the assets directory is valid
			return "assets/cache/shader/opengl";
		}
		
		static void create_cache_directory_if_needed()
		{
			std::string cache_directory = get_cache_directory();
			if (!std::filesystem::exists(cache_directory))
			{
				std::filesystem::create_directories(cache_directory);
			}
		}
		
		static const char* gl_shader_stage_cached_opengl_file_extension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER: return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
			}
			HP_CORE_ASSERT(false)
			return "";
		}
		
		static const char* gl_shader_stage_cached_vulkan_file_extension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
				case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
			}
			HP_CORE_ASSERT(false)
			return "";
		}
		
	}
	
	opengl_shader::opengl_shader(const std::string& filepath) : m_file_path(filepath)
	{
		utils::create_cache_directory_if_needed();
		
		std::string source = read_file(filepath);
		auto shader_sources = pre_process(source);
		
		{
			timer timer;
			compile_or_get_vulkan_binaries(shader_sources);
			compile_or_get_opengl_binaries();
			create_program();
			log::warning("shader creation took {0} ms", timer.elapsed_milliseconds());
		}
		
		// Extract name from filepath
		auto last_slash = filepath.find_last_of("/\\");
		last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
		auto last_dot = filepath.rfind('.');
		auto count = last_dot == std::string::npos ? filepath.size() - last_slash : last_dot - last_slash;
		m_name = filepath.substr(last_slash, count);
	}
	
	opengl_shader::opengl_shader(const std::string& name, const std::string& vertex_src, const std::string& fragment_src) : m_name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertex_src;
		sources[GL_FRAGMENT_SHADER] = fragment_src;
		
		compile_or_get_vulkan_binaries(sources);
		compile_or_get_opengl_binaries();
		create_program();
	}
	
	opengl_shader::~opengl_shader()
	{
		glDeleteProgram(m_renderer_id);
	}
	
	void opengl_shader::bind() const
	{
		glUseProgram(m_renderer_id);
	}
	
	void opengl_shader::unbind() const
	{
		glUseProgram(0);
	}
	
	void opengl_shader::set_int(const std::string& name, int value)
	{
		upload_uniform_int(name, value);
	}
	
	void opengl_shader::set_int_array(const std::string& name, int* values, uint32_t count)
	{
		upload_uniform_int_array(name, values, count);
	}
	
	void opengl_shader::set_float(const std::string& name, float value)
	{
		upload_uniform_float(name, value);
	}
	
	void opengl_shader::set_float_2(const std::string& name, const glm::vec2& value)
	{
		upload_uniform_float_2(name, value);
	}
	
	void opengl_shader::set_float_3(const std::string& name, const glm::vec3& value)
	{
		upload_uniform_float_3(name, value);
	}
	
	void opengl_shader::set_float_4(const std::string& name, const glm::vec4& value)
	{
		upload_uniform_float_4(name, value);
	}
	
	void opengl_shader::set_mat_4(const std::string& name, const glm::mat4& value)
	{
		upload_uniform_mat_4(name, value);
	}
	
	const std::string& opengl_shader::get_name() const
	{
		return m_name;
	}
	
	void opengl_shader::upload_uniform_int(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform1i(location, value);
	}
	
	void opengl_shader::upload_uniform_int_array(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform1iv(location, count, values);
	}
	
	void opengl_shader::upload_uniform_float(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform1f(location, value);
	}
	
	void opengl_shader::upload_uniform_float_2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	
	void opengl_shader::upload_uniform_float_3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	
	void opengl_shader::upload_uniform_float_4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	
	void opengl_shader::upload_uniform_mat_3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	
	void opengl_shader::upload_uniform_mat_4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	
	std::string opengl_shader::read_file(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			std::string::size_type size = static_cast<std::string::size_type>(in.tellg());
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				log::error("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			log::error("Could not open file '{0}'", filepath);
		}
		
		return result;
	}
	
	std::unordered_map<GLenum, std::string> opengl_shader::pre_process(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shader_sources;
		
		const char* type_token = "#type";
		size_t type_token_length = strlen(type_token);
		size_t pos = source.find(type_token, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			HP_CORE_ASSERT(eol != std::string::npos, "Syntax error")
			size_t begin = pos + type_token_length + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			HP_CORE_ASSERT(utils::shader_type_from_string(type), "Invalid shader type specified")
			
			size_t next_line_pos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			HP_CORE_ASSERT(next_line_pos != std::string::npos, "Syntax error")
			pos = source.find(type_token, next_line_pos); //Start of next shader type declaration line
			
			shader_sources[utils::shader_type_from_string(type)] =
				(pos == std::string::npos) ? source.substr(next_line_pos) : source.substr(next_line_pos, pos - next_line_pos);
		}
		
		return shader_sources;
	}
	
	void opengl_shader::compile_or_get_vulkan_binaries(const std::unordered_map<GLenum, std::string>& shader_sources)
	{
//		GLuint program = glCreateProgram();
		
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
		{
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}
		
		std::filesystem::path cache_directory = utils::get_cache_directory();
		
		auto& shader_data = m_vulkan_spirv;
		shader_data.clear();
		for (auto&& [stage, source] : shader_sources)
		{
			std::filesystem::path shader_file_path = m_file_path;
			std::filesystem::path
				cachedPath = cache_directory / (shader_file_path.filename().string() + utils::gl_shader_stage_cached_vulkan_file_extension(stage));
			
			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				std::string::size_type size = static_cast<std::string::size_type>(in.tellg());
				in.seekg(0, std::ios::beg);
				
				auto& data = shader_data[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult
					module = compiler.CompileGlslToSpv(source, utils::gl_shader_stage_to_shader_c(stage), m_file_path.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					log::error(module.GetErrorMessage());
					HP_CORE_ASSERT(false)
				}
				
				shader_data[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
				
				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shader_data[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
		
		for (auto&& [stage, data] : shader_data)
		{
			reflect(stage, data);
		}
	}
	
	void opengl_shader::compile_or_get_opengl_binaries()
	{
		auto& shader_data = m_opengl_spirv;
		
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = true;
		if (optimize)
		{
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}
		
		std::filesystem::path cache_directory = utils::get_cache_directory();
		
		shader_data.clear();
		m_opengl_source_code.clear();
		for (auto&& [stage, spirv] : m_vulkan_spirv)
		{
			std::filesystem::path shader_file_path = m_file_path;
			std::filesystem::path
				cachedPath = cache_directory / (shader_file_path.filename().string() + utils::gl_shader_stage_cached_opengl_file_extension(stage));
			
			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				std::string::size_type size = static_cast<std::string::size_type>(in.tellg());
				in.seekg(0, std::ios::beg);
				
				auto& data = shader_data[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glsl_compiler(spirv);
				m_opengl_source_code[stage] = glsl_compiler.compile();
				auto& source = m_opengl_source_code[stage];
				
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, utils::gl_shader_stage_to_shader_c(stage), m_file_path.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					log::error(module.GetErrorMessage());
					HP_CORE_ASSERT(false)
				}
				
				shader_data[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
				
				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shader_data[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}
	
	void opengl_shader::create_program()
	{
		GLuint program = glCreateProgram();
		
		std::vector<GLuint> shader_ids;
		for (auto&& [stage, spirv] : m_opengl_spirv)
		{
			GLuint shader_id = shader_ids.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shader_id, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shader_id, "main", 0, nullptr, nullptr);
			glAttachShader(program, shader_id);
		}
		
		glLinkProgram(program);
		
		GLint is_linked;
		glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
		if (is_linked == GL_FALSE)
		{
			GLint max_length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
			
			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(program, max_length, &max_length, info_log.data());
			log::error("shader linking failed ({0}):\n{1}", m_file_path, info_log.data());
			
			glDeleteProgram(program);
			
			for (auto id : shader_ids)
			{
				glDeleteShader(id);
			}
		}
		
		for (auto id : shader_ids)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		
		m_renderer_id = program;
	}
	
	void opengl_shader::reflect(GLenum stage, const std::vector<uint32_t>& shader_data)
	{
		spirv_cross::Compiler compiler(shader_data);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();
		
		log::trace("opengl_shader::reflect - {0} {1}", utils::gl_shader_stage_to_string(stage), m_file_path);
		log::trace("    {0} uniform buffers", resources.uniform_buffers.size());
		log::trace("    {0} resources", resources.sampled_images.size());
		
		log::trace("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& buffer_type = compiler.get_type(resource.base_type_id);
			uint32_t buffer_size = compiler.get_declared_struct_size(buffer_type);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int member_count = buffer_type.member_types.size();
			
			log::trace("  {0}", resource.name);
			log::trace("    size = {0}", buffer_size);
			log::trace("    Binding = {0}", binding);
			log::trace("    Members = {0}", member_count);
		}
	}
}  // namespace hp
