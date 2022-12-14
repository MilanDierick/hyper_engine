// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_VULKAN_SHADER_H
#define PLAYGROUND_VULKAN_SHADER_H

#include "hyper/renderer/shader.h"
#include "platform/vulkan/vk_device.h"

#include <vulkan/vulkan.h>
#include <VkBootstrap.h>

namespace hp
{
	class HP_API vulkan_shader : public shader
	{
	public:
		vulkan_shader(const std::string& filepath, vk_device& device);
		~vulkan_shader() override;

		vulkan_shader(const vulkan_shader& other)            = delete;
		vulkan_shader(vulkan_shader&& other) noexcept        = delete;
		vulkan_shader& operator=(const vulkan_shader& other) = delete;
		vulkan_shader& operator=(vulkan_shader&& other)      = delete;

		void bind() const override;
		void unbind() const override;
		void set_int(const std::string& name, int value) override;
		void set_int_array(const std::string& name, int* values, uint32_t count) override;
		void set_float(const std::string& name, float value) override;
		void set_float_2(const std::string& name, const glm::vec2& value) override;
		void set_float_3(const std::string& name, const glm::vec3& value) override;
		void set_float_4(const std::string& name, const glm::vec4& value) override;
		void set_mat_4(const std::string& name, const glm::mat4& value) override;
		[[nodiscard]] const std::string& get_name() const override;

		// Implicit cast operator
		explicit operator VkShaderModule() const;

	private:
		VkShaderModule m_shader_module;
		vk_device* m_device;
		std::string m_name;

		std::vector<char> load_bytecode_from_filepath(const std::string& filepath);
		VkShaderModule create_shader_module(const std::vector<char>& bytecode);
	};
} // namespace hp

#endif //PLAYGROUND_VULKAN_SHADER_H
