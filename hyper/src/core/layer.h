// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_LAYER_H
#define PLAYGROUND_LAYER_H

namespace hp
{
	class layer
	{
	 public:
		explicit layer(std::string  debug_name = "Layer");
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
}

#endif //PLAYGROUND_LAYER_H
