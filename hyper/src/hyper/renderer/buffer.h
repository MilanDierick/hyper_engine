#ifndef HYPER_BUFFER_H
#define HYPER_BUFFER_H

#include "hyper/core/asserts.h"

namespace hp
{
	enum class shader_data_type
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint32_t shader_data_type_size(shader_data_type type)
	{
		switch (type)
		{
			case shader_data_type::Float:
				return 4;
			case shader_data_type::Float2:
				return 4 * 2;
			case shader_data_type::Float3:
				return 4 * 3;
			case shader_data_type::Float4:
				return 4 * 4;
			case shader_data_type::Mat3:
				return 4 * 3 * 3;
			case shader_data_type::Mat4:
				return 4 * 4 * 4;
			case shader_data_type::Int:
				return 4;
			case shader_data_type::Int2:
				return 4 * 2;
			case shader_data_type::Int3:
				return 4 * 3;
			case shader_data_type::Int4:
				return 4 * 4;
			case shader_data_type::Bool:
				return 1;
			case shader_data_type::None:
				break;
		}

		HP_CORE_ASSERT(false, "Unknown shader_data_type!");
		return 0;
	}

	struct buffer_element
	{
		std::string name;
		shader_data_type type;
		uint32_t size;
		size_t offset;
		bool normalized;

		buffer_element(shader_data_type type, std::string name, bool normalized = false)
		    : name(std::move(name)),
		      type(type),
		      size(shader_data_type_size(type)),
		      offset(0),
		      normalized(normalized)
		{
		}

		[[nodiscard]] uint32_t get_component_count() const
		{
			switch (type)
			{
				case shader_data_type::Float:
					return 1;
				case shader_data_type::Float2:
					return 2;
				case shader_data_type::Float3:
					return 3;
				case shader_data_type::Float4:
					return 4;
				case shader_data_type::Mat3:
					return 3; // 3* float3
				case shader_data_type::Mat4:
					return 4; // 4* float4
				case shader_data_type::Int:
					return 1;
				case shader_data_type::Int2:
					return 2;
				case shader_data_type::Int3:
					return 3;
				case shader_data_type::Int4:
					return 4;
				case shader_data_type::Bool:
					return 1;
				case shader_data_type::None:
					break;
			}

			HP_CORE_ASSERT(false, "Unknown shader_data_type!");
			return 0;
		}
	};

	class buffer_layout
	{
	public:
		buffer_layout() = default;

		buffer_layout(std::initializer_list<buffer_element> elements) : m_elements(elements)
		{
			calculate_offsets_and_stride();
		}

		[[nodiscard]] uint32_t get_stride() const
		{
			return m_stride;
		}

		[[nodiscard]] const std::vector<buffer_element>& get_elements() const
		{
			return m_elements;
		}

		[[nodiscard]] std::vector<buffer_element>::iterator begin()
		{
			return m_elements.begin();
		}

		[[nodiscard]] std::vector<buffer_element>::iterator end()
		{
			return m_elements.end();
		}

		[[nodiscard]] std::vector<buffer_element>::const_iterator begin() const
		{
			return m_elements.begin();
		}

		[[nodiscard]] std::vector<buffer_element>::const_iterator end() const
		{
			return m_elements.end();
		}

	private:
		void calculate_offsets_and_stride()
		{
			size_t offset = 0;
			m_stride      = 0;
			for (auto& element: m_elements)
			{
				element.offset = offset;
				offset         += element.size;
				m_stride       += element.size;
			}
		}

		std::vector<buffer_element> m_elements;
		uint32_t m_stride = 0;
	};

	class vertex_buffer
	{
	public:
		vertex_buffer()          = default;
		virtual ~vertex_buffer() = default;

		vertex_buffer(const vertex_buffer& other)            = delete;
		vertex_buffer(vertex_buffer&& other) noexcept        = delete;
		vertex_buffer& operator=(const vertex_buffer& other) = delete;
		vertex_buffer& operator=(vertex_buffer&& other)      = delete;

		virtual void bind() const   = 0;
		virtual void Unbind() const = 0;

		virtual void set_data(const void* data, uint32_t size) = 0;

		[[nodiscard]] virtual const buffer_layout& get_layout() const = 0;
		virtual void set_layout(const buffer_layout& layout)          = 0;

		static std::shared_ptr<vertex_buffer> create(uint32_t size);
		static std::shared_ptr<vertex_buffer> create(float* vertices, uint32_t size);
	};

	// Currently Hyper Engine only supports 32-bit index buffers
	class index_buffer
	{
	public:
		index_buffer()          = default;
		virtual ~index_buffer() = default;

		index_buffer(const index_buffer& other)            = delete;
		index_buffer(index_buffer&& other) noexcept        = delete;
		index_buffer& operator=(const index_buffer& other) = delete;
		index_buffer& operator=(index_buffer&& other)      = delete;

		virtual void bind() const   = 0;
		virtual void Unbind() const = 0;

		[[nodiscard]] virtual uint32_t get_count() const = 0;

		static std::shared_ptr<index_buffer> create(uint32_t* indices, uint32_t count);
	};
} // namespace hp

#endif
