#include "platform/opengl/opengl_context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace hp
{
	opengl_context::opengl_context(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		HP_CORE_ASSERT(windowHandle, "Window handle is null!")
	}
	
	void opengl_context::init()
	{
		
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HP_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		log::info("OpenGL Info:");
		log::info("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		log::info("  renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		log::info("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		
		HP_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
			"Hazel requires at least OpenGL version 4.5!");
	}
	
	void opengl_context::swap_buffers()
	{
		
		glfwSwapBuffers(m_WindowHandle);
	}
}  // namespace hp
