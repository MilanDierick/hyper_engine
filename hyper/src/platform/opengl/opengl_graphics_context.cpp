// Copyright (c) 2022 Milan Dierick | This source file is licensed under GNU GPLv3.
// A copy of this license has been included in this project's root directory.

#include "opengl_graphics_context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace hp
{
	opengl_graphics_context::opengl_graphics_context(GLFWwindow* window_handle) : m_window_handle(window_handle)
	{
	}
	
	void opengl_graphics_context::init()
	{
		glfwMakeContextCurrent(m_window_handle);
		const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		HP_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		log::info("OpenGL Info:");
		log::info("    Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		log::info("    Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		log::info("    Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		
		#ifdef HP_ENABLE_ASSERTS
		int versionMajor = -1;
		int versionMinor = -1;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		HP_CORE_ASSERT(versionMajor > 4 || versionMajor == 4 && versionMinor >= 6,
					   "Hyper requires at least OpenGL version 4.6!");
		#endif
	}
	
	void opengl_graphics_context::swap_buffers()
	{
	
	}
}  // namespace hp