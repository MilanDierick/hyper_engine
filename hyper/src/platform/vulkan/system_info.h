// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_SYSTEM_INFO_H
#define HYPER_SYSTEM_INFO_H

#include <vulkan/vulkan.h>

namespace hp
{
	struct system_info
	{
		system_info();

		[[nodiscard]] bool is_layer_available(const std::string& layer_name) const;
		[[nodiscard]] bool is_extension_available(const std::string& extension_name) const;

		std::vector<VkLayerProperties> available_layers;
		std::vector<VkExtensionProperties> available_extensions;
		bool validation_layers_available;
		bool debug_utils_available;
	};
} // namespace hp

#endif //HYPER_SYSTEM_INFO_H
