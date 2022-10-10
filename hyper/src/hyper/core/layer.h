// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef HYPER_LAYER_H
#define HYPER_LAYER_H

#include <string>

namespace hp
{
	class HP_API layer
	{
	 public:
		explicit layer(std::string debug_name = "Layer");
		virtual ~layer() = default;

		layer(const layer& other) = delete;
		layer(layer&& other) noexcept = delete;
		layer& operator=(const layer& other) = delete;
		layer& operator=(layer&& other) = delete;

		virtual void on_attach() = 0;
		virtual void on_detach() = 0;
		virtual void on_update(uint64_t ticks) = 0;
		virtual void on_render() = 0;
		virtual void om_imgui_render() = 0;

		[[nodiscard]] const std::string& get_name() const;

	 private:
		std::string m_debug_name;
	};
}  // namespace hp

#endif //HYPER_LAYER_H
