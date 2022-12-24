#include "vk_instance.h"

#include <vulkan/vulkan.h>

namespace hp
{
	namespace detail
	{
		std::vector<const char*> get_vk_instance_extensions(b8 validation_layers_enabled)
		{
			u32 glfw_extension_count     = 0;
			const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

			std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

			if (validation_layers_enabled)
			{
				extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			}

			return extensions;
		}

		inline VKAPI_ATTR VkBool32 VKAPI_CALL default_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		                                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
		                                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		                                                             void* pUserData)
		{
			hp_unused(messageType);
			hp_unused(pUserData);

			switch (messageSeverity)
			{
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
					log::trace("Vulkan: {}", pCallbackData->pMessage);
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
					log::info("Vulkan: {}", pCallbackData->pMessage);
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
					log::warning("Vulkan: {}", pCallbackData->pMessage);
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
					log::error("Vulkan: {}", pCallbackData->pMessage);
					break;
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
					break;
			}

			return VK_FALSE;
		}

		void create_debug_messenger(VkInstance instance, PFN_vkDebugUtilsMessengerCallbackEXT debug_callback, VkDebugUtilsMessengerEXT& debug_messenger)
		{
			check_ptr(debug_callback, default_debug_callback);

			VkDebugUtilsMessengerCreateInfoEXT create_info = {};
			create_info.sType                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			create_info.messageSeverity                    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			create_info.messageType                        = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			create_info.pfnUserCallback                    = debug_callback;

			auto func = query_vk_func_ptr<PFN_vkCreateDebugUtilsMessengerEXT>(instance, "vkCreateDebugUtilsMessengerEXT");

			if (func(instance, &create_info, nullptr, &debug_messenger) != VK_SUCCESS)
			{
				log::error("Failed to set up debug messenger!");
			}
		}

		void destroy_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger)
		{
			auto func = query_vk_func_ptr<PFN_vkDestroyDebugUtilsMessengerEXT>(instance, "vkDestroyDebugUtilsMessengerEXT");
			func(instance, debug_messenger, nullptr);
		}
	} // namespace detail

	vk_instance::vk_instance()
	    : m_instance(VK_NULL_HANDLE),
	      m_debug_messenger(VK_NULL_HANDLE)
	{
	}

	VkInstance& vk_instance::get_instance()
	{
		return m_instance;
	}

	VkDebugUtilsMessengerEXT& vk_instance::get_debug_messenger()
	{
		return m_debug_messenger;
	}

	void create_vk_instance(vk_instance& instance, const vk_instance_parameters& parameters)
	{
		VkApplicationInfo const app_info = {
		        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		        .pApplicationName   = parameters.application_name.c_str(),
		        .applicationVersion = parameters.application_version,
		        .pEngineName        = parameters.engine_name.c_str(),
		        .engineVersion      = parameters.engine_version,
		        .apiVersion         = parameters.api_version,
		};

		auto extensions = detail::get_vk_instance_extensions(parameters.validation_layers_enabled);

		if (!parameters.validation_layers_enabled)
		{
			VkInstanceCreateInfo const create_info = {
			        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			        .pApplicationInfo        = &app_info,
			        .enabledExtensionCount   = static_cast<u32>(extensions.size()),
			        .ppEnabledExtensionNames = extensions.data(),
			};

			if (vkCreateInstance(&create_info, nullptr, &instance.get_instance()) != VK_SUCCESS)
			{
				log::error("Failed to create instance!");
			}

			return;
		}

		std::vector<const char*> validation_layers = {"VK_LAYER_KHRONOS_validation"};

		VkDebugUtilsMessengerCreateInfoEXT const debug_create_info = {
		        .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		        .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		        .pfnUserCallback = detail::default_debug_callback,
		};

		VkInstanceCreateInfo const create_info = {
		        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		        .pNext                   = &debug_create_info,
		        .pApplicationInfo        = &app_info,
		        .enabledLayerCount       = static_cast<uint32_t>(validation_layers.size()),
		        .ppEnabledLayerNames     = validation_layers.data(),
		        .enabledExtensionCount   = static_cast<u32>(extensions.size()),
		        .ppEnabledExtensionNames = extensions.data(),
		};

		if (vkCreateInstance(&create_info, nullptr, &instance.get_instance()) != VK_SUCCESS)
		{
			log::error("Failed to create instance!");
		}

		if (parameters.validation_layers_enabled)
		{
			detail::create_debug_messenger(instance.get_instance(), detail::default_debug_callback, instance.get_debug_messenger());
		}
	}

	void destroy_vk_instance(vk_instance& instance)
	{
		if (instance.get_debug_messenger() != nullptr)
		{
			detail::destroy_debug_messenger(instance.get_instance(), instance.get_debug_messenger());
		}

		vkDestroyInstance(instance.get_instance(), nullptr);
	}
} // namespace hp