// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "system_info.h"

#include <span>

namespace hp
{
	system_info::system_info() : validation_layers_available(false),
	                             debug_utils_available(false)
	{
		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		available_extensions.resize(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());

		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		available_layers.resize(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
	}

	bool system_info::is_layer_available(const std::string& layer_name) const
	{
		return std::any_of(available_layers.begin(), available_layers.end(), [&](const auto& layer)
		                   {
			                   std::span<const char> const layer_name_span(layer.layerName);
			                   return std::equal(layer_name.begin(), layer_name.end(), layer_name_span.begin(), layer_name_span.end());
		                   });
	}

	bool system_info::is_extension_available(const std::string& extension_name) const
	{
		return std::any_of(available_layers.begin(), available_layers.end(), [&](const auto& layer)
		                   {
			                   std::span<const char> const extension_name_span(layer.layerName);
			                   return std::equal(extension_name.begin(), extension_name.end(), extension_name_span.begin(), extension_name_span.end());
		                   });
	}
} // namespace hp