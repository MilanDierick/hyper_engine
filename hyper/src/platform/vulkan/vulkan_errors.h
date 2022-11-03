// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_VULKAN_ERRORS_H
#define PLAYGROUND_VULKAN_ERRORS_H

#include <exception>
#include <string>
#include <system_error>
#include <utility>

namespace hp
{
	enum class vulkan_instance_error
	{
		vulkan_unavailable,
		vulkan_version_unavailable,
		vulkan_version_1_1_unavailable,
		vulkan_version_1_2_unavailable,
		failed_create_instance,
		failed_create_debug_messenger,
		requested_layers_not_present,
		requested_extensions_not_present,
		windowing_extensions_not_present,
	};

	enum class vulkan_physical_device_error
	{
		no_surface_provided,
		failed_enumerate_physical_devices,
		no_physical_devices_found,
		no_suitable_device,
	};

	enum class vulkan_queue_error
	{
		present_unavailable,
		graphics_unavailable,
		compute_unavailable,
		transfer_unavailable,
		queue_index_out_of_range,
		invalid_queue_family_index
	};

	enum class vulkan_device_error
	{
		failed_create_device,
	};

	enum class vulkan_swapchain_error
	{
		surface_handle_not_provided,
		failed_query_surface_support_details,
		failed_create_swapchain,
		failed_get_swapchain_images,
		failed_create_swapchain_image_views,
		required_min_image_count_too_low,
		required_usage_not_supported
	};

	namespace detail
	{
#define CASE_TO_STRING(CATEGORY, TYPE) \
	case CATEGORY::TYPE:               \
		return #TYPE;

		const char* to_string(vulkan_instance_error err)
		{
			switch (err)
			{
				CASE_TO_STRING(vulkan_instance_error, vulkan_unavailable)
				CASE_TO_STRING(vulkan_instance_error, vulkan_version_unavailable)
				CASE_TO_STRING(vulkan_instance_error, vulkan_version_1_1_unavailable)
				CASE_TO_STRING(vulkan_instance_error, vulkan_version_1_2_unavailable)
				CASE_TO_STRING(vulkan_instance_error, failed_create_debug_messenger)
				CASE_TO_STRING(vulkan_instance_error, failed_create_instance)
				CASE_TO_STRING(vulkan_instance_error, requested_layers_not_present)
				CASE_TO_STRING(vulkan_instance_error, requested_extensions_not_present)
				CASE_TO_STRING(vulkan_instance_error, windowing_extensions_not_present)
				default:
					return "";
			}
		}
		const char* to_string(vulkan_physical_device_error err)
		{
			switch (err)
			{
				CASE_TO_STRING(vulkan_physical_device_error, no_surface_provided)
				CASE_TO_STRING(vulkan_physical_device_error, failed_enumerate_physical_devices)
				CASE_TO_STRING(vulkan_physical_device_error, no_physical_devices_found)
				CASE_TO_STRING(vulkan_physical_device_error, no_suitable_device)
				default:
					return "";
			}
		}
		const char* to_string(vulkan_queue_error err)
		{
			switch (err)
			{
				CASE_TO_STRING(vulkan_queue_error, present_unavailable)
				CASE_TO_STRING(vulkan_queue_error, graphics_unavailable)
				CASE_TO_STRING(vulkan_queue_error, compute_unavailable)
				CASE_TO_STRING(vulkan_queue_error, transfer_unavailable)
				CASE_TO_STRING(vulkan_queue_error, queue_index_out_of_range)
				CASE_TO_STRING(vulkan_queue_error, invalid_queue_family_index)
				default:
					return "";
			}
		}
		const char* to_string(vulkan_device_error err)
		{
			switch (err)
			{
				CASE_TO_STRING(vulkan_device_error, failed_create_device)
				default:
					return "";
			}
		}
		const char* to_string(vulkan_swapchain_error err)
		{
			switch (err)
			{
				CASE_TO_STRING(vulkan_swapchain_error, surface_handle_not_provided)
				CASE_TO_STRING(vulkan_swapchain_error, failed_query_surface_support_details)
				CASE_TO_STRING(vulkan_swapchain_error, failed_create_swapchain)
				CASE_TO_STRING(vulkan_swapchain_error, failed_get_swapchain_images)
				CASE_TO_STRING(vulkan_swapchain_error, failed_create_swapchain_image_views)
				CASE_TO_STRING(vulkan_swapchain_error, required_min_image_count_too_low)
				CASE_TO_STRING(vulkan_swapchain_error, required_usage_not_supported)
				default:
					return "";
			}
		}

		struct vulkan_instance_error_category : std::error_category
		{
			[[nodiscard]] const char* name() const noexcept override { return "vulkan_instance"; }
			[[nodiscard]] std::string message(int err) const override { return to_string(static_cast<vulkan_instance_error>(err)); }
		};

		vulkan_instance_error_category instance_error_category;

		struct vulkan_physical_device_error_category : std::error_category
		{
			[[nodiscard]] const char* name() const noexcept override { return "vulkan_physical_device"; }
			[[nodiscard]] std::string message(int err) const override { return to_string(static_cast<vulkan_physical_device_error>(err)); }
		};

		const vulkan_physical_device_error_category physical_device_error_category;

		struct vulkan_queue_error_category : std::error_category
		{
			[[nodiscard]] const char* name() const noexcept override { return "vulkan_queue"; }
			[[nodiscard]] std::string message(int err) const override { return to_string(static_cast<vulkan_queue_error>(err)); }
		};

		const vulkan_queue_error_category queue_error_category;

		struct vulkan_device_error_category : std::error_category
		{
			[[nodiscard]] const char* name() const noexcept override { return "vulkan_device"; }
			[[nodiscard]] std::string message(int err) const override { return to_string(static_cast<vulkan_device_error>(err)); }
		};

		const vulkan_device_error_category device_error_category;

		struct vulkan_swapchain_error_category : std::error_category
		{
			[[nodiscard]] const char* name() const noexcept override { return "vulkan_swapchain"; }
			[[nodiscard]] std::string message(int err) const override { return to_string(static_cast<vulkan_swapchain_error>(err)); }
		};

		const vulkan_swapchain_error_category swapchain_error_category;
	} // namespace detail

	std::error_code make_error_code(vulkan_instance_error instance_error)
	{
		return {static_cast<int>(instance_error), detail::instance_error_category};
	}

	std::error_code make_error_code(vulkan_physical_device_error physical_device_error)
	{
		return {static_cast<int>(physical_device_error), detail::physical_device_error_category};
	}

	std::error_code make_error_code(vulkan_queue_error queue_error)
	{
		return {static_cast<int>(queue_error), detail::queue_error_category};
	}

	std::error_code make_error_code(vulkan_device_error device_error)
	{
		return {static_cast<int>(device_error), detail::device_error_category};
	}

	std::error_code make_error_code(vulkan_swapchain_error swapchain_error)
	{
		return {static_cast<int>(swapchain_error), detail::swapchain_error_category};
	}
} // namespace hp

#endif //PLAYGROUND_VULKAN_ERRORS_H
