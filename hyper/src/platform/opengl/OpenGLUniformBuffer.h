#ifndef HYPER_OPENGL_UNIFORM_BUFFER
#define HYPER_OPENGL_UNIFORM_BUFFER

#include "hyper/renderer/UniformBuffer.h"

namespace hp
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	 public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();
		
		virtual void set_data(const void* data, uint32_t size, uint32_t offset = 0) override;
	 private:
		uint32_t m_RendererID = 0;
	};
}  // namespace hp

#endif