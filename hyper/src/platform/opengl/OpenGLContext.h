#ifndef HYPER_OPENGL_CONTEXT_H
#define HYPER_OPENGL_CONTEXT_H

#include "hyper/renderer/GraphicsContext.h"

struct GLFWwindow;

namespace hp
{
	class OpenGLContext : public GraphicsContext
	{
	 public:
		OpenGLContext(GLFWwindow* windowHandle);
		
		virtual void Init() override;
		virtual void SwapBuffers() override;
	 private:
		GLFWwindow* m_WindowHandle;
	};
}  // namespace hp

#endif