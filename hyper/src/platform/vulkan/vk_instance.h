#ifndef PLAYGROUND_VK_INSTANCE_H
#define PLAYGROUND_VK_INSTANCE_H

#include "hyper/core/types.h"

namespace hp
{
	namespace detail
	{
		template<typename First, typename Second>
		constexpr void check_ptr(First pointer, Second fallback)
		{
			if (pointer == nullptr && fallback == nullptr)
			{
				log::critical("Unable to recover from nullptr, fallback is nullptr as well!");
				throw;
			}

			if (pointer == nullptr)
			{
				pointer = fallback;
			}
		}

		template<typename T>
		constexpr T query_vk_func_ptr(VkInstance instance, const char* func_name)
		{
			auto func = reinterpret_cast<T>(vkGetInstanceProcAddr(instance, func_name));
			check_ptr(func, nullptr);
			return func;
		}
	} // namespace detail

	struct vk_instance_parameters
	{
		std::string application_name;
		std::string engine_name;
		u32 application_version;
		u32 engine_version;
		u32 api_version;
		b8 validation_layers_enabled;
	};

	class vk_instance
	{
	public:
		vk_instance();
		~vk_instance() = default;

		vk_instance(const vk_instance& other)                = delete;
		vk_instance(vk_instance&& other) noexcept            = delete;
		vk_instance& operator=(const vk_instance& other)     = delete;
		vk_instance& operator=(vk_instance&& other) noexcept = delete;

		VkInstance& get_instance();
		VkDebugUtilsMessengerEXT& get_debug_messenger();

	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerEXT m_debug_messenger;
	};

	void create_vk_instance(vk_instance& instance, const vk_instance_parameters& parameters);
	void destroy_vk_instance(vk_instance& instance);
} // namespace hp

#endif //PLAYGROUND_VK_INSTANCE_H
