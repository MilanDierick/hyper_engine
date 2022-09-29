#include "hyper/renderer/renderer_2d.h"

#include "hyper/renderer/vertex_array.h"
#include "hyper/renderer/shader.h"
#include "hyper/renderer/UniformBuffer.h"
#include "hyper/renderer/render_command.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hp
{
	struct quad_vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 tex_coord;
		float tex_index;
		float tiling_factor;
		
		// Editor-only
		int entity_id;
	};
	
	struct circle_vertex
	{
		glm::vec3 world_position;
		glm::vec3 local_position;
		glm::vec4 color;
		float thickness;
		float fade;
		
		// Editor-only
		int entity_id;
	};
	
	struct line_vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		
		// Editor-only
		int entity_id;
	};
	
	struct renderer_2d_data
	{
		static const uint32_t max_quads = 20000;
		static const uint32_t max_vertices = max_quads * 4;
		static const uint32_t max_indices = max_quads * 6;
		static const uint32_t max_texture_slots = 32; // TODO: RenderCaps
		
		std::shared_ptr<vertex_array> quad_vertex_array;
		std::shared_ptr<vertex_buffer> quad_vertex_buffer;
		std::shared_ptr<shader> quad_shader;
		std::shared_ptr<texture_2d> white_texture;
		
		std::shared_ptr<vertex_array> circle_vertex_array;
		std::shared_ptr<vertex_buffer> circle_vertex_buffer;
		std::shared_ptr<shader> circle_shader;
		
		std::shared_ptr<vertex_array> line_vertex_array;
		std::shared_ptr<vertex_buffer> line_vertex_buffer;
		std::shared_ptr<shader> line_shader;
		
		uint32_t quad_index_count = 0;
		quad_vertex* quad_vertex_buffer_base = nullptr;
		quad_vertex* quad_vertex_buffer_ptr = nullptr;
		
		uint32_t circle_index_count = 0;
		circle_vertex* circle_vertex_buffer_base = nullptr;
		circle_vertex* circle_vertex_buffer_ptr = nullptr;
		
		uint32_t line_vertex_count = 0;
		line_vertex* line_vertex_buffer_base = nullptr;
		line_vertex* line_vertex_buffer_ptr = nullptr;
		
		float line_width = 2.0f;
		
		std::array<std::shared_ptr<texture_2d>, max_texture_slots> texture_slots;
		uint32_t texture_slot_index = 1; // 0 = white texture
		
		glm::vec4 quad_vertex_positions[4];
		
		renderer_2d::statistics stats;
		
		struct camera_data
		{
			glm::mat4 view_projection;
		};
		
		camera_data camera_buffer;
		std::shared_ptr<UniformBuffer> camera_uniform_buffer;
	};
	
	static renderer_2d_data s_data;
	
	void renderer_2d::init()
	{
		s_data.quad_vertex_array = vertex_array::create();
		
		s_data.quad_vertex_buffer = vertex_buffer::create(s_data.max_vertices * sizeof(quad_vertex));
		s_data.quad_vertex_buffer->set_layout(
			{{ shader_data_type::Float3, "a_Position" },
			 { shader_data_type::Float4, "a_Color" },
			 { shader_data_type::Float2, "a_TexCoord" },
			 { shader_data_type::Float,  "a_TexIndex" },
			 { shader_data_type::Float,  "a_TilingFactor" },
			 { shader_data_type::Int,    "a_EntityID" }});
		
		s_data.quad_vertex_array->add_vertex_buffer(s_data.quad_vertex_buffer);
		
		s_data.quad_vertex_buffer_base = new quad_vertex[s_data.max_vertices];
		
		uint32_t* quad_indices = new uint32_t[s_data.max_indices];
		
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.max_indices; i += 6)
		{
			quad_indices[i + 0] = offset + 0;
			quad_indices[i + 1] = offset + 1;
			quad_indices[i + 2] = offset + 2;
			
			quad_indices[i + 3] = offset + 2;
			quad_indices[i + 4] = offset + 3;
			quad_indices[i + 5] = offset + 0;
			
			offset += 4;
		}
		
		std::shared_ptr<index_buffer> quad_ib = index_buffer::create(quad_indices, s_data.max_indices);
		s_data.quad_vertex_array->set_index_buffer(quad_ib);
		delete[] quad_indices;
		
		// Circles
		s_data.circle_vertex_array = vertex_array::create();
		
		s_data.circle_vertex_buffer = vertex_buffer::create(s_data.max_vertices * sizeof(circle_vertex));
		s_data.circle_vertex_buffer->set_layout(
			{{ shader_data_type::Float3, "a_WorldPosition" },
			 { shader_data_type::Float3, "a_LocalPosition" },
			 { shader_data_type::Float4, "a_Color" },
			 { shader_data_type::Float,  "a_Thickness" },
			 { shader_data_type::Float,  "a_Fade" },
			 { shader_data_type::Int,    "a_EntityID" }});
		
		s_data.circle_vertex_array->add_vertex_buffer(s_data.circle_vertex_buffer);
		s_data.circle_vertex_array->set_index_buffer(quad_ib); // Use quad IB
		s_data.circle_vertex_buffer_base = new circle_vertex[s_data.max_vertices];
		
		// Lines
		s_data.line_vertex_array = vertex_array::create();
		
		s_data.line_vertex_buffer = vertex_buffer::create(s_data.max_vertices * sizeof(line_vertex));
		s_data.line_vertex_buffer->set_layout(
			{{ shader_data_type::Float3, "a_Position" },
			 { shader_data_type::Float4, "a_Color" },
			 { shader_data_type::Int,    "a_EntityID" }});
		s_data.line_vertex_array->add_vertex_buffer(s_data.line_vertex_buffer);
		s_data.line_vertex_buffer_base = new line_vertex[s_data.max_vertices];
		
		s_data.white_texture = texture_2d::Create(1, 1);
		uint32_t white_texture_data = 0xffffffff;
		s_data.white_texture->SetData(&white_texture_data, sizeof(uint32_t));
		
		int32_t samplers[s_data.max_texture_slots];
		for (uint32_t i = 0; i < s_data.max_texture_slots; i++)
		{
			samplers[i] = i;
		}
		
		s_data.quad_shader = shader::create("assets/shaders/Renderer2D_Quad.glsl");
		s_data.circle_shader = shader::create("assets/shaders/Renderer2D_Circle.glsl");
		s_data.line_shader = shader::create("assets/shaders/Renderer2D_Line.glsl");
		
		// Set first texture slot to 0
		s_data.texture_slots[0] = s_data.white_texture;
		
		s_data.quad_vertex_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quad_vertex_positions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quad_vertex_positions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_data.quad_vertex_positions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
		
		s_data.camera_uniform_buffer = UniformBuffer::Create(sizeof(renderer_2d_data::camera_data), 0);
	}
	
	void renderer_2d::shutdown()
	{
		delete[] s_data.quad_vertex_buffer_base;
	}
	
	void renderer_2d::begin_scene(const orthographic_camera& camera)
	{
		s_data.camera_buffer.view_projection = camera.get_view_projection_matrix();
		s_data.camera_uniform_buffer->set_data(&s_data.camera_buffer, sizeof(renderer_2d_data::camera_data));
		
		start_batch();
	}
	
	void renderer_2d::begin_scene(const camera& camera, const glm::mat4& transform)
	{
		s_data.camera_buffer.view_projection = camera.get_projection() * glm::inverse(transform);
		s_data.camera_uniform_buffer->set_data(&s_data.camera_buffer, sizeof(renderer_2d_data::camera_data));
		
		start_batch();
	}
	
	void renderer_2d::begin_scene(const editor_camera& camera)
	{
		s_data.camera_buffer.view_projection = camera.get_view_projection();
		s_data.camera_uniform_buffer->set_data(&s_data.camera_buffer, sizeof(renderer_2d_data::camera_data));
		
		start_batch();
	}
	
	void renderer_2d::end_scene()
	{
		flush();
	}
	
	void renderer_2d::start_batch()
	{
		s_data.quad_index_count = 0;
		s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;
		
		s_data.circle_index_count = 0;
		s_data.circle_vertex_buffer_ptr = s_data.circle_vertex_buffer_base;
		
		s_data.line_vertex_count = 0;
		s_data.line_vertex_buffer_ptr = s_data.line_vertex_buffer_base;
		
		s_data.texture_slot_index = 1;
	}
	
	void renderer_2d::flush()
	{
		if (s_data.quad_index_count)
		{
			uint32_t data_size = (uint32_t)((uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.quad_vertex_buffer_base);
			s_data.quad_vertex_buffer->set_data(s_data.quad_vertex_buffer_base, data_size);
			
			// bind textures
			for (uint32_t i = 0; i < s_data.texture_slot_index; i++)
			{
				s_data.texture_slots[i]->bind(i);
			}
			
			s_data.quad_shader->bind();
			render_command::draw_indexed(s_data.quad_vertex_array, s_data.quad_index_count);
			s_data.stats.draw_calls++;
		}
		
		if (s_data.circle_index_count)
		{
			uint32_t data_size = (uint32_t)((uint8_t*)s_data.circle_vertex_buffer_ptr - (uint8_t*)s_data.circle_vertex_buffer_base);
			s_data.circle_vertex_buffer->set_data(s_data.circle_vertex_buffer_base, data_size);
			
			s_data.circle_shader->bind();
			render_command::draw_indexed(s_data.circle_vertex_array, s_data.circle_index_count);
			s_data.stats.draw_calls++;
		}
		
		if (s_data.line_vertex_count)
		{
			uint32_t data_size = (uint32_t)((uint8_t*)s_data.line_vertex_buffer_ptr - (uint8_t*)s_data.line_vertex_buffer_base);
			s_data.line_vertex_buffer->set_data(s_data.line_vertex_buffer_base, data_size);
			
			s_data.line_shader->bind();
			render_command::set_line_width(s_data.line_width);
			render_command::draw_lines(s_data.line_vertex_array, s_data.line_vertex_count);
			s_data.stats.draw_calls++;
		}
	}
	
	void renderer_2d::next_batch()
	{
		flush();
		start_batch();
	}
	
	void renderer_2d::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		draw_quad({ position.x, position.y, 0.0f }, size, color);
	}
	
	void renderer_2d::draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		draw_quad(transform, color);
	}
	
	void renderer_2d::draw_quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<texture_2d>& texture, float tiling_factor, const glm::vec4& tint_color)
	{
		draw_quad({ position.x, position.y, 0.0f }, size, texture, tiling_factor, tint_color);
	}
	
	void renderer_2d::draw_quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<texture_2d>& texture, float tiling_factor, const glm::vec4& tint_color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		draw_quad(transform, texture, tiling_factor, tint_color);
	}
	
	void renderer_2d::draw_quad(const glm::mat4& transform, const glm::vec4& color, int entity_id)
	{
		constexpr size_t quad_vertex_count = 4;
		const float texture_index = 0.0f; // White Texture
		const float tiling_factor = 1.0f;
		constexpr glm::vec2 texture_coords[] = {{ 0.0f, 0.0f },
		                                        { 1.0f, 0.0f },
		                                        { 1.0f, 1.0f },
		                                        { 0.0f, 1.0f }};
		
		if (s_data.quad_index_count >= renderer_2d_data::max_indices)
		{
			next_batch();
		}
		
		for (size_t i = 0; i < quad_vertex_count; i++)
		{
			s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
			s_data.quad_vertex_buffer_ptr->color = color;
			s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
			s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
			s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
			s_data.quad_vertex_buffer_ptr->entity_id = entity_id;
			s_data.quad_vertex_buffer_ptr++;
		}
		
		s_data.quad_index_count += 6;
		s_data.stats.quad_count++;
	}
	
	void renderer_2d::draw_quad(const glm::mat4& transform, const std::shared_ptr<texture_2d>& texture, float tiling_factor, const glm::vec4& tint_color, int entity_id)
	{
		
		constexpr size_t quad_vertex_count = 4;
		constexpr glm::vec2 texture_coords[] = {{ 0.0f, 0.0f },
		                                        { 1.0f, 0.0f },
		                                        { 1.0f, 1.0f },
		                                        { 0.0f, 1.0f }};
		
		if (s_data.quad_index_count >= renderer_2d_data::max_indices)
		{
			next_batch();
		}
		
		float texture_index = 0.0f;
		for (uint32_t i = 1; i < s_data.texture_slot_index; i++)
		{
			if (*s_data.texture_slots[i] == *texture)
			{
				texture_index = (float)i;
				break;
			}
		}
		
		if (texture_index == 0.0f)
		{
			if (s_data.texture_slot_index >= renderer_2d_data::max_texture_slots)
			{
				next_batch();
			}
			
			texture_index = (float)s_data.texture_slot_index;
			s_data.texture_slots[s_data.texture_slot_index] = texture;
			s_data.texture_slot_index++;
		}
		
		for (size_t i = 0; i < quad_vertex_count; i++)
		{
			s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
			s_data.quad_vertex_buffer_ptr->color = tint_color;
			s_data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
			s_data.quad_vertex_buffer_ptr->tex_index = texture_index;
			s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
			s_data.quad_vertex_buffer_ptr->entity_id = entity_id;
			s_data.quad_vertex_buffer_ptr++;
		}
		
		s_data.quad_index_count += 6;
		s_data.stats.quad_count++;
	}
	
	void renderer_2d::draw_rotated_quad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		draw_rotated_quad({ position.x, position.y, 0.0f }, size, rotation, color);
	}
	
	void renderer_2d::draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		draw_quad(transform, color);
	}
	
	void renderer_2d::draw_rotated_quad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<texture_2d>& texture, float tiling_factor, const glm::vec4& tint_color)
	{
		draw_rotated_quad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling_factor, tint_color);
	}
	
	void renderer_2d::draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<texture_2d>& texture, float tiling_factor, const glm::vec4& tint_color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		draw_quad(transform, texture, tiling_factor, tint_color);
	}
	
	void renderer_2d::draw_circle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entity_id /*= -1*/)
	{
		// TODO: implement for circles
		// if (s_data.quad_index_count >= renderer_2d_data::max_indices)
		// 	next_batch();
		
		for (size_t i = 0; i < 4; i++)
		{
			s_data.circle_vertex_buffer_ptr->world_position = transform * s_data.quad_vertex_positions[i];
			s_data.circle_vertex_buffer_ptr->local_position = s_data.quad_vertex_positions[i] * 2.0f;
			s_data.circle_vertex_buffer_ptr->color = color;
			s_data.circle_vertex_buffer_ptr->thickness = thickness;
			s_data.circle_vertex_buffer_ptr->fade = fade;
			s_data.circle_vertex_buffer_ptr->entity_id = entity_id;
			s_data.circle_vertex_buffer_ptr++;
		}
		
		s_data.circle_index_count += 6;
		s_data.stats.quad_count++;
	}
	
	void renderer_2d::draw_line(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entity_id)
	{
		s_data.line_vertex_buffer_ptr->position = p0;
		s_data.line_vertex_buffer_ptr->color = color;
		s_data.line_vertex_buffer_ptr->entity_id = entity_id;
		s_data.line_vertex_buffer_ptr++;
		
		s_data.line_vertex_buffer_ptr->position = p1;
		s_data.line_vertex_buffer_ptr->color = color;
		s_data.line_vertex_buffer_ptr->entity_id = entity_id;
		s_data.line_vertex_buffer_ptr++;
		
		s_data.line_vertex_count += 2;
	}
	
	void renderer_2d::draw_rect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entity_id)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		
		draw_line(p0, p1, color, entity_id);
		draw_line(p1, p2, color, entity_id);
		draw_line(p2, p3, color, entity_id);
		draw_line(p3, p0, color, entity_id);
	}
	
	void renderer_2d::draw_rect(const glm::mat4& transform, const glm::vec4& color, int entity_id)
	{
		glm::vec3 line_vertices[4];
		for (size_t i = 0; i < 4; i++)
		{
			line_vertices[i] = transform * s_data.quad_vertex_positions[i];
		}
		
		draw_line(line_vertices[0], line_vertices[1], color, entity_id);
		draw_line(line_vertices[1], line_vertices[2], color, entity_id);
		draw_line(line_vertices[2], line_vertices[3], color, entity_id);
		draw_line(line_vertices[3], line_vertices[0], color, entity_id);
	}
	
	//	void renderer_2d::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	//	{
	//		if (src.Texture)
	//			draw_quad(transform, src.Texture, src.tiling_factor, src.color, entityID);
	//		else
	//			draw_quad(transform, src.color, entityID);
	//	}
	
	float renderer_2d::get_line_width()
	{
		return s_data.line_width;
	}
	
	void renderer_2d::set_line_width(float width)
	{
		s_data.line_width = width;
	}
	
	void renderer_2d::reset_stats()
	{
		memset(&s_data.stats, 0, sizeof(statistics));
	}
	
	renderer_2d::statistics renderer_2d::get_stats()
	{
		return s_data.stats;
	}
}
