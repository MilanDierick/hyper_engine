#ifndef HYPER_OPENGL_SHADER_H
#define HYPER_OPENGL_SHADER_H

#include "hyper/renderer/shader.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace hp
{
	class opengl_shader : public shader
	{
	 public:
		opengl_shader(const std::string& filepath);
		opengl_shader(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
		~opengl_shader() override;
		
		void bind() const override;
		void unbind() const override;
		
		void set_int(const std::string& name, int value) override;
		void set_int_array(const std::string& name, int* values, uint32_t count) override;
		void set_float(const std::string& name, float value) override;
		void set_float_2(const std::string& name, const glm::vec2& value) override;
		void set_float_3(const std::string& name, const glm::vec3& value) override;
		void set_float_4(const std::string& name, const glm::vec4& value) override;
		void set_mat_4(const std::string& name, const glm::mat4& value) override;
		
		const std::string& get_name() const override;
		
		void upload_uniform_int(const std::string& name, int value);
		void upload_uniform_int_array(const std::string& name, int* values, uint32_t count);
		void upload_uniform_float(const std::string& name, float value);
		void upload_uniform_float_2(const std::string& name, const glm::vec2& value);
		void upload_uniform_float_3(const std::string& name, const glm::vec3& value);
		void upload_uniform_float_4(const std::string& name, const glm::vec4& value);
		void upload_uniform_mat_3(const std::string& name, const glm::mat3& matrix);
		void upload_uniform_mat_4(const std::string& name, const glm::mat4& matrix);
	 
	 private:
		uint32_t m_renderer_id;
		std::string m_file_path;
		std::string m_name;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_vulkan_spirv;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_opengl_spirv;
		std::unordered_map<GLenum, std::string> m_opengl_source_code;
		
		std::string read_file(const std::string& filepath);
		std::unordered_map<GLenum, std::string> pre_process(const std::string& source);
		void compile_or_get_vulkan_binaries(const std::unordered_map<GLenum, std::string>& shader_sources);
		void compile_or_get_opengl_binaries();
		void create_program();
		void reflect(GLenum stage, const std::vector<uint32_t>& shader_data);
	};
	
}  // namespace hp

#endif