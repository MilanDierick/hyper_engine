#ifndef HYPER_OPENGL_CONTEXT_H
#define HYPER_OPENGL_CONTEXT_H

#include "hyper/renderer/graphics_context.h"

struct GLFWwindow;

namespace hp
{
	class opengl_context : public graphics_context
	{
	 public:
		opengl_context(GLFWwindow* windowHandle);
		
		virtual void init() override;
		virtual void swap_buffers() override;
	 private:
		GLFWwindow* m_WindowHandle;
	};
}  // namespace hp

#endif