#include "hyper/renderer/buffer.h"
#include "hyper/renderer/renderer.h"
#include "platform/opengl/OpenGLBuffer.h"

namespace hp
{
	
	std::shared_ptr<vertex_buffer> vertex_buffer::create(uint32_t size)
	{
		switch (renderer::get_api())
		{
		case renderer_api::API::None:
		HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
			return nullptr;
		case renderer_api::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(size);
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
	
	std::shared_ptr<vertex_buffer> vertex_buffer::create(float* vertices, uint32_t size)
	{
		switch (renderer::get_api())
		{
		case renderer_api::API::None:
		HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
			return nullptr;
		case renderer_api::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
	
	std::shared_ptr<index_buffer> index_buffer::create(uint32_t* indices, uint32_t count)
	{
		switch (renderer::get_api())
		{
		case renderer_api::API::None:
		HP_CORE_ASSERT(false, "renderer_api::None is currently not supported!");
			return nullptr;
		case renderer_api::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}
		
		HP_CORE_ASSERT(false, "Unknown renderer_api!");
		return nullptr;
	}
	
}