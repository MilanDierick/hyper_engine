// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#ifndef PLAYGROUND_OPENGL_GRAPHICS_CONTEXT_H
#define PLAYGROUND_OPENGL_GRAPHICS_CONTEXT_H

#include "renderer/graphics_context.h"

struct GLFWwindow;

namespace hp
{
	class opengl_graphics_context final : public graphics_context
	{
	 public:
		explicit opengl_graphics_context(GLFWwindow* window_handle);
		
		void init() override;
		void swap_buffers() override;
		
	 private:
		GLFWwindow* m_window_handle;
	};
	
}  // namespace hp

#endif //PLAYGROUND_OPENGL_GRAPHICS_CONTEXT_H
