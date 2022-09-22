#ifndef HYPER_BUFFER_H
#define HYPER_BUFFER_H

#include <cstdint>
#include <string>
#include "asserts.h"

namespace hp
{
	
	enum class shader_data_type
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
	
	static uint32_t shader_data_type_size(shader_data_type type)
	{
		switch (type)
		{
			case shader_data_type::Float: return 4;
			case shader_data_type::Float2: return 4 * 2;
			case shader_data_type::Float3: return 4 * 3;
			case shader_data_type::Float4: return 4 * 4;
			case shader_data_type::Mat3: return 4 * 3 * 3;
			case shader_data_type::Mat4: return 4 * 4 * 4;
			case shader_data_type::Int: return 4;
			case shader_data_type::Int2: return 4 * 2;
			case shader_data_type::Int3: return 4 * 3;
			case shader_data_type::Int4: return 4 * 4;
			case shader_data_type::Bool: return 1;
		}
		
		HP_CORE_ASSERT(false, "Unknown shader_data_type!");
		return 0;
	}
	
	struct BufferElement
	{
		std::string      Name;
		shader_data_type Type;
		uint32_t         Size;
		size_t           offset;
		bool             normalized;
		
		BufferElement() = default;
		
		BufferElement(shader_data_type type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(shader_data_type_size(type)), offset(0), normalized(normalized)
		{
		}
		
		uint32_t get_component_count() const
		{
			switch (Type)
			{
				case shader_data_type::Float: return 1;
				case shader_data_type::Float2: return 2;
				case shader_data_type::Float3: return 3;
				case shader_data_type::Float4: return 4;
				case shader_data_type::Mat3: return 3; // 3* float3
				case shader_data_type::Mat4: return 4; // 4* float4
				case shader_data_type::Int: return 1;
				case shader_data_type::Int2: return 2;
				case shader_data_type::Int3: return 3;
				case shader_data_type::Int4: return 4;
				case shader_data_type::Bool: return 1;
			}
			
			HP_CORE_ASSERT(false, "Unknown shader_data_type!");
			return 0;
		}
	};
	
	class BufferLayout
	{
	 public:
		BufferLayout()
		{
		}
		
		BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}
		
		uint32_t get_stride() const
		{
			return m_Stride;
		}
		
		const std::vector<BufferElement>& get_elements() const
		{
			return m_Elements;
		}
		
		std::vector<BufferElement>::iterator begin()
		{
			return m_Elements.begin();
		}
		
		std::vector<BufferElement>::iterator end()
		{
			return m_Elements.end();
		}
		
		std::vector<BufferElement>::const_iterator begin() const
		{
			return m_Elements.begin();
		}
		
		std::vector<BufferElement>::const_iterator end() const
		{
			return m_Elements.end();
		}
	 
	 private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	 
	 private:
		std::vector<BufferElement> m_Elements;
		uint32_t                   m_Stride = 0;
	};
	
	class vertex_buffer
	{
	 public:
		virtual ~vertex_buffer() = default;
		
		virtual void bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual void SetData(const void* data, uint32_t size) = 0;
		
		virtual const BufferLayout& get_layout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		
		static std::shared_ptr<vertex_buffer> Create(uint32_t size);
		static std::shared_ptr<vertex_buffer> Create(float* vertices, uint32_t size);
	};
	
	// Currently Hazel only supports 32-bit index buffers
	class index_buffer
	{
	 public:
		virtual ~index_buffer() = default;
		
		virtual void bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual uint32_t GetCount() const = 0;
		
		static std::shared_ptr<index_buffer> Create(uint32_t* indices, uint32_t count);
	};
	
}

#endif
