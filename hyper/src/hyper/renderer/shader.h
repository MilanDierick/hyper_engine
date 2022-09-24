#ifndef HYPER_SHADER_H
#define HYPER_SHADER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace hp
{
	class shader
	{
	 public:
		virtual ~shader() = default;
		
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		
		virtual void set_int(const std::string& name, int value) = 0;
		virtual void set_int_array(const std::string& name, int* values, uint32_t count) = 0;
		virtual void set_float(const std::string& name, float value) = 0;
		virtual void set_float_2(const std::string& name, const glm::vec2& value) = 0;
		virtual void set_float_3(const std::string& name, const glm::vec3& value) = 0;
		virtual void set_float_4(const std::string& name, const glm::vec4& value) = 0;
		virtual void set_mat_4(const std::string& name, const glm::mat4& value) = 0;
		
		virtual const std::string& get_name() const = 0;
		
		static std::shared_ptr<shader> create(const std::string& filepath);
		static std::shared_ptr<shader> create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
	};
	
	class ShaderLibrary
	{
	 public:
		void add(const std::string& name, const std::shared_ptr<shader>& shader);
		void add(const std::shared_ptr<shader>& shader);
		std::shared_ptr<shader> load(const std::string& filepath);
		std::shared_ptr<shader> load(const std::string& name, const std::string& filepath);
		std::shared_ptr<shader> get(const std::string& name);
		bool exists(const std::string& name) const;
	 
	 private:
		std::unordered_map<std::string, std::shared_ptr<shader>> m_shaders;
	};
	
}  // namespace hp

#endif