// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_VULKAN_INSTANCE_H
#define HYPER_VULKAN_INSTANCE_H

#include "platform/vulkan/vulkan_instance_builder.h"

#include <vulkan/vulkan.h>

namespace hp
{
	struct vulkan_instance
	{
		vulkan_instance();

		VkInstance instance;
		VkDebugUtilsMessengerEXT debug_messenger;
		VkAllocationCallbacks* allocation_callbacks;

		operator VkInstance() const;

	private:
		bool headless;
		bool supports_properties2_ext;
		uint32_t instance_version;
		uint32_t api_version;

		friend class vulkan_instance_builder;
	};
} // namespace hp

#endif //HYPER_VULKAN_INSTANCE_H
