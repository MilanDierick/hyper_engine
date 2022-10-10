// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_PLAYGROUND_LAYER_H
#define PLAYGROUND_PLAYGROUND_LAYER_H

#include "hyper.h"

namespace pg
{
	class playground_layer final : public hp::layer
	{
	public:
		playground_layer();
		~playground_layer() override = default;

		playground_layer(const playground_layer& other) = delete;
		playground_layer(playground_layer&& other) noexcept = delete;
		playground_layer& operator=(const playground_layer& other) = delete;
		playground_layer& operator=(playground_layer&& other) = delete;

		void on_attach() override;
		void on_detach() override;
		void on_update(uint64_t ticks) override;
		void on_render() override;
		void om_imgui_render() override;

	private:
		glm::vec4 m_SquareColor;
	};

} // namespace pg

#endif //PLAYGROUND_PLAYGROUND_LAYER_H
