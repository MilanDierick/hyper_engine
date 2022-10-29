// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "vulkan_window.h"

#include "hyper/core/asserts.h"
#include "hyper/core/input.h"
#include "hyper/core/log.h"
#include "hyper/events/key_event_args.h"
#include "hyper/events/mouse_event_args.h"
//#include "platform/opengl/opengl_context.h"

namespace hp
{
	static bool s_glfw_initialized = false;

	static void glfw_error_callback(const int32_t error, const char* description)
	{
		log::error("GLFW error ({0}): {1}", error, description);
	}

	window* window::create(const window_parameters& parameters)
	{
		return new vulkan_window(parameters);
	}

	vulkan_window::vulkan_window(const window_parameters& parameters)
	    : m_data(),
	      m_window(nullptr)
	{
		init(parameters);
	}

	vulkan_window::~vulkan_window()
	{
		shutdown();
	}

	void vulkan_window::on_update()
	{
		glfwPollEvents();
		//m_context->swap_buffers(); // TODO
	}

	uint32_t vulkan_window::get_width() const
	{
		return m_data.width;
	}

	uint32_t vulkan_window::get_height() const
	{
		return m_data.height;
	}

	void vulkan_window::set_vsync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(0);
		}
		else
		{
			m_data.vsync = enabled;
		}
	}

	bool vulkan_window::get_vsync() const
	{
		return m_data.vsync;
	}

	void* vulkan_window::get_native_window() const
	{
		return m_window;
	}

	void vulkan_window::init(const window_parameters& parameters)
	{
		init_vulkan();
		init_glfw(parameters);
	}

	void vulkan_window::shutdown() const
	{
		auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_vulkan_instance, "vkDestroyDebugUtilsMessengerEXT"));

		if (func != nullptr)
		{
			func(m_vulkan_instance, m_vulkan_debug_messenger, nullptr);
		}
		else
		{
			throw std::runtime_error("Vulkan extension not present!");
		}

		vkDestroyInstance(m_vulkan_instance, nullptr);
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void vulkan_window::init_glfw(const window_parameters& parameters)
	{
		m_data.title                  = parameters.title;
		m_data.width                  = parameters.width;
		m_data.height                 = parameters.height;
		m_data.p_window_resized_event = &window_resized_event;
		m_data.p_window_closed_event  = &window_closed_event;

		log::info("Creating window {0} ({1} {2})", parameters.title, parameters.width, parameters.height);

		if (!s_glfw_initialized)
		{
			const int32_t success = glfwInit();
			HP_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(glfw_error_callback);
			s_glfw_initialized = true;
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
		glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(parameters.width, parameters.height, parameters.title, nullptr, nullptr);

		//m_context = std::make_unique<opengl_context>(m_window); // TODO
		//m_context->init();

		glfwSetWindowUserPointer(m_window, &m_data);
		set_vsync(false);

		glfwSetWindowSizeCallback(
		        m_window,
		        [](GLFWwindow* window, const int width, const int height)
		        {
			        auto* data   = static_cast<window_data*>(glfwGetWindowUserPointer(window));
			        data->width  = width;
			        data->height = height;

			        const window_resized_event_args eventArgs(width, height);
			        data->p_window_resized_event->invoke(eventArgs);
		        });

		glfwSetWindowCloseCallback(
		        m_window,
		        [](GLFWwindow* window)
		        {
			        auto* data = static_cast<window_data*>(glfwGetWindowUserPointer(window));

			        const window_closed_event_args eventArgs;
			        data->p_window_closed_event->invoke(eventArgs);
		        });

		glfwSetKeyCallback(
		        m_window,
		        [](GLFWwindow*, const int key, int, const int action, int)
		        {
			        switch (action)
			        {
				        case GLFW_PRESS:
				        {
					        const key_pressed_event_args eventArgs(static_cast<key_code>(key), 0);
					        input::key_pressed_event.invoke(eventArgs);
					        break;
				        }
				        case GLFW_RELEASE:
				        {
					        const key_released_event_args eventArgs(static_cast<key_code>(key));
					        input::key_released_event.invoke(eventArgs);
					        break;
				        }
				        case GLFW_REPEAT:
				        {
					        const key_typed_event_args eventArgs(static_cast<key_code>(key));
					        input::key_typed_event.invoke(eventArgs);
					        break;
				        }
				        default:
					        log::error("Tried to process missing GLFW key action");
					        break;
			        }
		        });

		glfwSetCharCallback(
		        m_window,
		        [](GLFWwindow*, const unsigned int keycode)
		        {
			        const key_typed_event_args eventArgs(static_cast<key_code>(keycode));

			        input::key_typed_event.invoke(eventArgs);
		        });

		glfwSetMouseButtonCallback(
		        m_window,
		        [](GLFWwindow*, const int button, const int action, int)
		        {
			        switch (action)
			        {
				        case GLFW_PRESS:
				        {
					        const mouse_button_pressed_event_args event(static_cast<mouse_code>(button));
					        input::mouse_button_pressed_event.invoke(event);
					        break;
				        }
				        case GLFW_RELEASE:
				        {
					        const mouse_button_released_event_args event(static_cast<mouse_code>(button));
					        input::mouse_button_released_event.invoke(event);
					        break;
				        }
				        default:
					        log::error("Tried to process missing GLFW key action");
					        break;
			        }
		        });

		glfwSetScrollCallback(
		        m_window,
		        [](GLFWwindow*, const double x_offset, const double y_offset)
		        {
			        const mouse_scrolled_event_args
			                mouseScrolledEventArgs(static_cast<float>(x_offset), static_cast<float>(y_offset));
			        input::mouse_scrolled_event.invoke(mouseScrolledEventArgs);
		        });

		glfwSetCursorPosCallback(
		        m_window,
		        [](GLFWwindow*, const double x_pos, const double y_pos)
		        {
			        const mouse_moved_event_args event(static_cast<float>(x_pos), static_cast<float>(y_pos));
			        input::mouse_moved_event.invoke(event);
		        });
	}

	void vulkan_window::init_vulkan()
	{
		create_vulkan_instance();
	}

	void vulkan_window::create_vulkan_instance()
	{
		auto extensions = get_required_vulkan_extensions();

#ifdef HP_VULKAN_VALIDATION_LAYERS
		if (!check_vulkan_validation_layer_support())
		{
			throw std::runtime_error("Validation layers requested but not available!");
		}
#endif

		VkApplicationInfo application_info           = {};
		application_info.sType                       = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pApplicationName            = "Playground";
		application_info.applicationVersion          = VK_MAKE_VERSION(0, 0, 1);
		application_info.pEngineName                 = "Hyper Engine";
		application_info.engineVersion               = VK_MAKE_VERSION(0, 0, 1);
		application_info.apiVersion                  = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instance_create_info    = {};
		instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pApplicationInfo        = &application_info;
		instance_create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
		instance_create_info.ppEnabledExtensionNames = extensions.data();
		instance_create_info.enabledLayerCount       = 0;

		// Consider handling VK_ERROR_INCOMPATIBLE_DRIVER exception when running on macOS
		// See: https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Instance
		if (vkCreateInstance(&instance_create_info, nullptr, &m_vulkan_instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance!");
		}
	}

#ifdef HP_VULKAN_VALIDATION_LAYERS
	bool vulkan_window::check_vulkan_validation_layer_support()
	{
		// TODO: Abstract Vulkan validation layers to clean up this method and simplify validation layer usage in the future
		const std::vector<const char*> validation_layers = {"VK_LAYER_KHRONOS_validation"};

		uint32_t layer_count                             = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> available_layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

		for (const char* layer_name: validation_layers)
		{
			bool layer_found = false;

			for (const auto& layer_properties: available_layers)
			{
				if (strcmp(layer_name, static_cast<const char*>(layer_properties.layerName)) == 0)
				{
					layer_found = true;
					break;
				}
			}

			if (!layer_found)
			{
				return false;
			}
		}

		return true;
	}

	void vulkan_window::setup_vulkan_debug_messenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT create_info{};
		create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		create_info.pfnUserCallback = debug_callback;
		create_info.pUserData       = nullptr; // Optional

		auto func                   = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_vulkan_instance, "vkCreateDebugUtilsMessengerEXT"));

		if (func != nullptr)
		{
			if (func(m_vulkan_instance, &create_info, nullptr, &m_vulkan_debug_messenger) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to set up debug messenger!");
			}
		}
		else
		{
			throw std::runtime_error("Vulkan extension not present!");
		}
	}

	VkBool32 vulkan_window::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, void* p_user_data)
	{
		UNUSED(message_type);
		UNUSED(p_user_data);

		if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			log::warning("Validation layer: {0}", p_callback_data->pMessage);
		}
		else if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			log::error("Validation layer: {0}", p_callback_data->pMessage);
		}
		else if (message_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT)
		{
			log::critical("Validation layer: {0}", p_callback_data->pMessage);
		}

		return VK_FALSE;
	}
#endif

	std::vector<const char*> vulkan_window::get_required_vulkan_extensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = nullptr;
		glfwExtensions              = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef HP_VULKAN_VALIDATION_LAYERS
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

		return extensions;
	}
} // namespace hp
