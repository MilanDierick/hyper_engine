// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "vulkan_shader.h"

#include "hyper/core/application.h"
#include "vulkan_window.h"

namespace hp
{
	vulkan_shader::vulkan_shader(const std::string& filepath) : m_device()
	{
		m_shader_module = create_shader_module(load_bytecode_from_filepath(filepath));
	}

	vulkan_shader::vulkan_shader(const std::string& filepath, vkb::Device* device) : m_device(device->device)
	{
		m_shader_module = create_shader_module(load_bytecode_from_filepath(filepath));
	}

	vulkan_shader::~vulkan_shader()
	{
		vkDestroyShaderModule(m_device, m_shader_module, nullptr);
	}

	vulkan_shader::operator VkShaderModule() const
	{
		return m_shader_module;
	}

	std::vector<char> vulkan_shader::load_bytecode_from_filepath(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open file: " + filepath);
		}

		const auto file_size = static_cast<std::streamsize>(file.tellg());
		std::vector<char> buffer(file_size);

		file.seekg(0);
		file.read(buffer.data(), file_size);

		file.close();

		return buffer;
	}

	VkShaderModule vulkan_shader::create_shader_module(const std::vector<char>& bytecode)
	{
		if (m_device == nullptr)
		{
			auto* vulkan_window = dynamic_cast<class vulkan_window*>(&application::instance()->get_window());
			m_device = vulkan_window->get_context()->get_device().device;
		}

		VkShaderModuleCreateInfo create_info{};
		create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = bytecode.size();
		create_info.pCode    = reinterpret_cast<const uint32_t*>(bytecode.data());

		VkShaderModule shader_module = VK_NULL_HANDLE;
		if (vkCreateShaderModule(m_device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module!");
		}

		return shader_module;
	}

	void vulkan_shader::bind() const
	{
	}

	void vulkan_shader::unbind() const
	{
	}

	void vulkan_shader::set_int(const std::string& name, int value)
	{
		UNUSED(name);
		UNUSED(value);
	}

	void vulkan_shader::set_int_array(const std::string& name, int* values, uint32_t count)
	{
		UNUSED(name);
		UNUSED(values);
		UNUSED(count);
	}

	void vulkan_shader::set_float(const std::string& name, float value)
	{
		UNUSED(name);
		UNUSED(value);
	}

	void vulkan_shader::set_float_2(const std::string& name, const glm::vec2& value)
	{
		UNUSED(name);
		UNUSED(value);
	}

	void vulkan_shader::set_float_3(const std::string& name, const glm::vec3& value)
	{
		UNUSED(name);
		UNUSED(value);
	}

	void vulkan_shader::set_float_4(const std::string& name, const glm::vec4& value)
	{
		UNUSED(name);
		UNUSED(value);
	}

	void vulkan_shader::set_mat_4(const std::string& name, const glm::mat4& value)
	{
		UNUSED(name);
		UNUSED(value);
	}

	const std::string& vulkan_shader::get_name() const
	{
		return m_name;
	}
} // namespace hp