#ifndef HYPER_VERTEX_ARRAY_H
#define HYPER_VERTEX_ARRAY_H

#include "hyper/renderer/Buffer.h"

#include <memory>

namespace hp
{
	class vertex_array
	{
	 public:
		virtual ~vertex_array() = default;
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
		
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
		
		static std::shared_ptr<vertex_array> Create();
	};
	
}  // namespace hp

#endif