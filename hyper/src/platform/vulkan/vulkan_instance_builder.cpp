// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "platform/vulkan/vulkan_instance_builder.h"

#include "platform/vulkan/system_info.h"
#include "platform/vulkan/vulkan_errors.h"

namespace hp
{
	namespace detail
	{
		template<typename T>
		void setup_p_next_chain(T& structure, std::vector<VkBaseOutStructure*> const& structs)
		{
			structure.pNext = nullptr;
			if (structs.empty())
				return;

			for (size_t i = 0; i < structs.size() - 1; i++)
			{
				structs.at(i)->pNext = structs.at(i + 1);
			}

			structure.pNext = structs.at(0);
		}

		const char* validation_layer_name = "VK_LAYER_KHRONOS_validation";
	} // namespace detail

	vulkan_instance_builder::vulkan_instance_builder() = default;

	result<vulkan_instance> vulkan_instance_builder::build() const
	{
		const system_info system_info;

		uint32_t instance_version = VK_API_VERSION_1_0;

		if (m_info.minimum_instance_version > VK_API_VERSION_1_0 || m_info.required_api_version > VK_API_VERSION_1_0 || m_info.desired_api_version > VK_API_VERSION_1_0)
		{
			const VkResult result = vkEnumerateInstanceVersion(&instance_version);
			if (result != VK_SUCCESS && m_info.required_api_version > 0)
			{
				return {make_error_code(vulkan_instance_error::vulkan_version_unavailable)};
			}

			if (instance_version < m_info.minimum_instance_version || m_info.minimum_instance_version == 0 && instance_version < m_info.required_api_version)
			{
				if (VK_VERSION_MINOR(m_info.required_api_version) == 2)
				{
					return {make_error_code(vulkan_instance_error::vulkan_version_1_2_unavailable)};
				}

				if (VK_VERSION_MINOR(m_info.required_api_version))
				{
					return {make_error_code(vulkan_instance_error::vulkan_version_1_1_unavailable)};
				}

				return static_cast<::hp::result<vulkan_instance>>(make_error_code(vulkan_instance_error::vulkan_version_unavailable));
			}
		}

		uint32_t api_version = instance_version < VK_VERSION_1_1 ? instance_version : m_info.required_api_version;

		if (m_info.desired_api_version > VK_API_VERSION_1_0 && instance_version >= m_info.desired_api_version)
		{
			instance_version = m_info.desired_api_version;
			api_version      = m_info.desired_api_version;
		}

		VkApplicationInfo app_info  = {};
		app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pNext              = nullptr;
		app_info.pApplicationName   = !m_info.app_name.empty() ? m_info.app_name.c_str() : "";
		app_info.applicationVersion = m_info.application_version;
		app_info.pEngineName        = !m_info.engine_name.empty() ? m_info.engine_name.c_str() : "";
		app_info.engineVersion      = m_info.engine_version;
		app_info.apiVersion         = api_version;

		std::vector<const char*> extensions;
		std::vector<const char*> layers;

		for (const auto& ext: m_info.enabled_extensions)
		{
			extensions.push_back(ext);
		}

		if (m_info.debug_callback != nullptr && system_info.debug_utils_available)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		const bool supports_properties2_ext = detail::check_extension_supported(system_info.available_extensions, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

		if (supports_properties2_ext && api_version < VK_API_VERSION_1_1)
		{
			extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		}

#if defined(VK_KHR_portability_enumeration)
		const bool portability_enumeration_support = detail::check_extension_supported(system_info.available_extensions, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

		if (portability_enumeration_support)
		{
			extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		}
#else
		bool portability_enumeration_support = false;
#endif
		if (!m_info.headless_context)
		{
			auto check_add_window_ext = [&](const char* name) -> bool
			{
				if (!detail::check_extension_supported(system_info.available_extensions, name))
					return false;
				extensions.push_back(name);
				return true;
			};

			const bool khr_surface_added = check_add_window_ext("VK_KHR_surface");
#if defined(_WIN32)
			const bool added_window_exts = check_add_window_ext("VK_KHR_win32_surface");
#elif defined(__ANDROID__)
			bool added_window_exts = check_add_window_ext("VK_KHR_android_surface");
#elif defined(_DIRECT2DISPLAY)
			bool added_window_exts = check_add_window_ext("VK_KHR_display");
#elif defined(__linux__)
			// make sure all three calls to check_add_window_ext, don't allow short circuiting
			bool added_window_exts = check_add_window_ext("VK_KHR_xcb_surface");
			added_window_exts      = check_add_window_ext("VK_KHR_xlib_surface") || added_window_exts;
			added_window_exts      = check_add_window_ext("VK_KHR_wayland_surface") || added_window_exts;
#elif defined(__APPLE__)
			bool added_window_exts = check_add_window_ext("VK_EXT_metal_surface");
#endif
			if (!khr_surface_added || !added_window_exts)
			{
				return {make_error_code(vulkan_instance_error::windowing_extensions_not_present)};
			}
		}

		const bool all_extensions_supported = detail::check_extensions_supported(system_info.available_extensions, extensions);

		if (!all_extensions_supported)
		{
			return {make_error_code(vulkan_instance_error::requested_extensions_not_present)};
		}

		for (const auto& layer: m_info.enabled_layers)
		{
			layers.push_back(layer);
		}

		if (m_info.enable_validation_layers || (m_info.request_validation_layers && system_info.validation_layers_available))
		{
			layers.push_back(detail::validation_layer_name);
		}

		const bool all_layers_supported = detail::check_layers_supported(system_info.available_layers, layers);

		if (!all_layers_supported)
		{
			return {make_error_code(vulkan_instance_error::requested_layers_not_present)};
		}

		std::vector<VkBaseOutStructure*> p_next_chain;

		VkDebugUtilsMessengerCreateInfoEXT messenger_create_info = {};
		if (m_info.use_default_debug_messenger)
		{
			messenger_create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			messenger_create_info.pNext           = nullptr;
			messenger_create_info.messageSeverity = m_info.debug_message_severity;
			messenger_create_info.messageType     = m_info.debug_message_type;
			messenger_create_info.pfnUserCallback = m_info.debug_callback;
			messenger_create_info.pUserData       = m_info.p_user_data;
			p_next_chain.push_back(reinterpret_cast<VkBaseOutStructure*>(&messenger_create_info));
		}

		VkValidationFeaturesEXT features{};
		if (!m_info.enabled_validation_features.empty() || !m_info.disabled_validation_features.empty())
		{
			features.sType                          = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
			features.pNext                          = nullptr;
			features.enabledValidationFeatureCount  = static_cast<uint32_t>(m_info.enabled_validation_features.size());
			features.pEnabledValidationFeatures     = m_info.enabled_validation_features.data();
			features.disabledValidationFeatureCount = static_cast<uint32_t>(m_info.disabled_validation_features.size());
			features.pDisabledValidationFeatures    = m_info.disabled_validation_features.data();
			p_next_chain.push_back(reinterpret_cast<VkBaseOutStructure*>(&features));
		}

		VkValidationFlagsEXT checks{};
		if (!m_info.disabled_validation_checks.empty())
		{
			checks.sType                        = VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT;
			checks.pNext                        = nullptr;
			checks.disabledValidationCheckCount = static_cast<uint32_t>(m_info.disabled_validation_checks.size());
			checks.pDisabledValidationChecks    = m_info.disabled_validation_checks.data();
			p_next_chain.push_back(reinterpret_cast<VkBaseOutStructure*>(&checks));
		}

		VkInstanceCreateInfo instance_create_info = {};
		instance_create_info.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		detail::setup_p_next_chain(instance_create_info, p_next_chain);
#if !defined(NDEBUG)
		for (auto& node: p_next_chain)
		{
			assert(node->sType != VK_STRUCTURE_TYPE_APPLICATION_INFO);
		}
#endif
		instance_create_info.flags                   = m_info.flags;
		instance_create_info.pApplicationInfo        = &app_info;
		instance_create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
		instance_create_info.ppEnabledExtensionNames = extensions.data();
		instance_create_info.enabledLayerCount       = static_cast<uint32_t>(layers.size());
		instance_create_info.ppEnabledLayerNames     = layers.data();
#if defined(VK_KHR_portability_enumeration)
		if (portability_enumeration_support)
		{
			instance_create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
		}
#endif

		vulkan_instance instance;
		VkResult res = vkCreateInstance(&instance_create_info, nullptr, &instance.instance);

		if (res != VK_SUCCESS)
		{
			return {make_error_code(vulkan_instance_error::failed_create_instance), res};
		}

		if (m_info.use_default_debug_messenger)
		{
			res = create_debug_utils_messenger(instance.instance,
			                                   m_info.debug_callback,
			                                   m_info.debug_message_severity,
			                                   m_info.debug_message_type,
			                                   m_info.p_user_data,
			                                   &instance.debug_messenger,
			                                   m_info.p_allocation_callbacks);
			if (res != VK_SUCCESS)
			{
				return {make_error_code(vulkan_instance_error::failed_create_debug_messenger), res};
			}
		}

		instance.headless                 = m_info.headless_context;
		instance.supports_properties2_ext = supports_properties2_ext;
		instance.allocation_callbacks     = m_info.p_allocation_callbacks;
		instance.instance_version         = instance_version;
		instance.api_version              = api_version;
		return instance;
	}
} // namespace hp