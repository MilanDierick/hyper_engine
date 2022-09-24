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
		hp::orthographic_camera_controller m_CameraController;
		
		// Temp
		std::shared_ptr<hp::vertex_array> m_SquareVA;
		std::shared_ptr<hp::shader> m_FlatColorShader;
		
		std::shared_ptr<hp::Texture2D> m_CheckerboardTexture;
		
		glm::vec4 m_SquareColor = { 0.2F, 0.3F, 0.8F, 1.0F };
	};
	
}  // namespace pg

#endif //PLAYGROUND_PLAYGROUND_LAYER_H
