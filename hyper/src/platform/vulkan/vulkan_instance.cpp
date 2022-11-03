// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "vulkan_instance.h"

namespace hp
{
	vulkan_instance::vulkan_instance()
	    : instance(nullptr),
	      debug_messenger(nullptr),
	      allocation_callbacks(nullptr),
	      headless(false),
	      supports_properties2_ext(false),
	      instance_version(VK_MAKE_API_VERSION(0, 1, 0, 0)),
	      api_version(VK_MAKE_API_VERSION(0, 1, 0, 0))
	{
	}

	vulkan_instance::operator VkInstance() const
	{
		return instance;
	}
} // namespace hp