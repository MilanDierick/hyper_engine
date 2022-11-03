// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_VULKAN_INSTANCE_BUILDER_H
#define HYPER_VULKAN_INSTANCE_BUILDER_H

#include "hyper/core/result.h"
#include "platform/vulkan/vulkan_instance.h"

#include <span>

namespace hp
{
	const char* to_string_message_severity(VkDebugUtilsMessageSeverityFlagBitsEXT string)
	{
		switch (string)
		{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				return "VERBOSE";
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				return "ERROR";
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				return "WARNING";
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				return "INFO";
			default:
				return "UNKNOWN";
		}
	}

	const char* to_string_message_type(VkDebugUtilsMessageTypeFlagsEXT string)
	{
		if (string == 7)
			return "General | Validation | Performance";
		if (string == 6)
			return "Validation | Performance";
		if (string == 5)
			return "General | Performance";
		if (string == 4 /*VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT*/)
			return "Performance";
		if (string == 3)
			return "General | Validation";
		if (string == 2 /*VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT*/)
			return "Validation";
		if (string == 1 /*VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT*/)
			return "General";
		return "Unknown";
	}

	inline VKAPI_ATTR VkBool32 VKAPI_CALL default_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	                                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
	                                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	                                                             void* /*unused*/)
	{
		const auto* message_severity = to_string_message_severity(messageSeverity);
		const auto* message_type     = to_string_message_type(messageType);

		log::error("Vulkan Validation Layer: [{}] [{}] {}", message_severity, message_type, pCallbackData->pMessage);
		return VK_FALSE; // Application should return VK_FALSE if it does not want to abort the call
	}

	VkResult create_debug_utils_messenger(VkInstance instance,
	                                      PFN_vkDebugUtilsMessengerCallbackEXT debug_callback,
	                                      VkDebugUtilsMessageSeverityFlagsEXT severity,
	                                      VkDebugUtilsMessageTypeFlagsEXT type,
	                                      void* user_data_pointer,
	                                      VkDebugUtilsMessengerEXT* pDebugMessenger,
	                                      VkAllocationCallbacks* allocation_callbacks)
	{
		if (debug_callback == nullptr)
		{
			debug_callback = default_debug_callback;
		}

		VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {};
		messengerCreateInfo.sType                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messengerCreateInfo.pNext                              = nullptr;
		messengerCreateInfo.messageSeverity                    = severity;
		messengerCreateInfo.messageType                        = type;
		messengerCreateInfo.pfnUserCallback                    = debug_callback;
		messengerCreateInfo.pUserData                          = user_data_pointer;

		auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

		if (func != nullptr)
		{
			return func(instance, &messengerCreateInfo, allocation_callbacks, pDebugMessenger);
		}

		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void destroy_debug_utils_messenger(
	        VkInstance instance,
	        VkDebugUtilsMessengerEXT debugMessenger,
	        VkAllocationCallbacks* allocation_callbacks)
	{
		auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

		if (func != nullptr)
		{
			func(instance, debugMessenger, allocation_callbacks);
		}
	}

	namespace detail
	{
		bool check_layer_supported(std::vector<VkLayerProperties> const& available_layers, const char* layer_name)
		{
			if (layer_name == nullptr)
			{
				return false;
			}

			for (const auto& layer_properties: available_layers)
			{
				const std::span<const char> layer_name_span(layer_name, strlen(layer_name));
				const std::span<const char> layer_properties_span(layer_properties.layerName, strlen(layer_properties.layerName)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
				return std::equal(layer_name_span.begin(), layer_name_span.end(), layer_properties_span.begin());
			}

			return false;
		}

		bool check_layers_supported(std::vector<VkLayerProperties> const& available_layers, std::vector<const char*> const& layer_names)
		{
			bool all_found = true;

			for (const auto& layer_name: layer_names)
			{
				const bool found = check_layer_supported(available_layers, layer_name);

				if (!found)
				{
					all_found = false;
				}
			}

			return all_found;
		}

		bool check_extension_supported(std::vector<VkExtensionProperties> const& available_extensions, const char* extension_name)
		{
			if (extension_name == nullptr)
			{
				return false;
			}

			for (const auto& extension_properties: available_extensions)
			{
				const std::span<const char> extension_name_span(extension_name, strlen(extension_name));
				const std::span<const char> extension_properties_span(extension_properties.extensionName, strlen(extension_properties.extensionName)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
				return std::equal(extension_name_span.begin(), extension_name_span.end(), extension_properties_span.begin());
			}

			return false;
		}

		bool check_extensions_supported(std::vector<VkExtensionProperties> const& available_extensions, std::vector<const char*> const& extension_names)
		{
			bool all_found = true;

			for (const auto& extension_name: extension_names)
			{
				const bool found = check_extension_supported(available_extensions, extension_name);

				if (!found)
				{
					all_found = false;
				}
			}

			return all_found;
		}
	} // namespace detail

	class vulkan_instance_builder
	{
		struct instance_info
		{
			// VkApplicationInfo
			std::string app_name;
			std::string engine_name;
			uint32_t application_version;
			uint32_t engine_version;
			uint32_t minimum_instance_version;
			uint32_t required_api_version;
			uint32_t desired_api_version;

			// VkInstanceCreateInfo
			std::vector<const char*> enabled_layers;
			std::vector<const char*> enabled_extensions;
			VkInstanceCreateFlags flags;
			std::vector<VkBaseOutStructure*> p_next_elements;

			// Debug messenger
			bool use_default_debug_messenger                           = false;
			PFN_vkDebugUtilsMessengerCallbackEXT debug_callback        = default_debug_callback;
			VkDebugUtilsMessageSeverityFlagsEXT debug_message_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			                                                             VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			VkDebugUtilsMessageTypeFlagsEXT debug_message_type = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			                                                     VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			                                                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

			// User data
			void* p_user_data = nullptr;

			// Validation layers
			bool enable_validation_layers  = false;
			bool request_validation_layers = false;
			std::vector<VkValidationCheckEXT> disabled_validation_checks;
			std::vector<VkValidationFeatureEnableEXT> enabled_validation_features;
			std::vector<VkValidationFeatureDisableEXT> disabled_validation_features;

			// Allocation callbacks
			VkAllocationCallbacks* p_allocation_callbacks = nullptr;

			// Headless
			bool headless_context = false;
		};

	public:
		vulkan_instance_builder();

		result<vulkan_instance> build() const;

		vulkan_instance_builder& set_app_name(const std::string& app_name);
		vulkan_instance_builder& set_engine_name(const std::string& engine_name);
		vulkan_instance_builder& set_app_version(uint32_t app_version);
		vulkan_instance_builder& set_app_version(uint32_t major, uint32_t minor, uint32_t patch);
		vulkan_instance_builder& set_engine_version(uint32_t engine_version);
		vulkan_instance_builder& set_engine_version(uint32_t major, uint32_t minor, uint32_t patch);
		vulkan_instance_builder& require_api_version(uint32_t api_version);
		vulkan_instance_builder& require_api_version(uint32_t major, uint32_t minor, uint32_t patch);
		vulkan_instance_builder& set_minimum_instance_version(uint32_t instance_version);
		vulkan_instance_builder& set_minimum_instance_version(uint32_t major, uint32_t minor, uint32_t patch);
		vulkan_instance_builder& enable_layer(const std::string& layer_name);
		vulkan_instance_builder& enable_extension(const std::string& extension_name);
		vulkan_instance_builder& set_headless(bool headless = true);
		vulkan_instance_builder& enable_validation_layers(bool enable = true);
		vulkan_instance_builder& request_validation_layers(bool request = true);
		vulkan_instance_builder& use_default_debug_messenger();
		vulkan_instance_builder& set_debug_messenger_callback(VkDebugUtilsMessengerCallbackDataEXT callback);
		vulkan_instance_builder& set_debug_callback_user_data(void* p_user_data);
		vulkan_instance_builder& set_debug_callback_severity_flags(VkDebugUtilsMessageSeverityFlagsEXT severity_flags);
		vulkan_instance_builder& add_debug_callback_severity_flag(VkDebugUtilsMessageSeverityFlagBitsEXT severity_flag);
		vulkan_instance_builder& set_debug_messenger_type_flags(VkDebugUtilsMessageTypeFlagsEXT type_flags);
		vulkan_instance_builder& add_debug_messenger_type_flag(VkDebugUtilsMessageTypeFlagBitsEXT type_flag);
		vulkan_instance_builder& disable_validation_layer(VkValidationCheckEXT check);
		vulkan_instance_builder& enable_validation_feature(VkValidationFeatureEnableEXT feature);
		vulkan_instance_builder& disable_validation_feature(VkValidationFeatureDisableEXT feature);
		vulkan_instance_builder& set_allocation_callbacks(VkAllocationCallbacks* p_allocation_callbacks);

	private:
		instance_info m_info;
	};
} // namespace hp
#endif //HYPER_VULKAN_INSTANCE_BUILDER_H
