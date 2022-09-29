#ifndef HYPER_RENDERER_2D
#define HYPER_RENDERER_2D

#include "hyper/renderer/camera.h"
#include "hyper/renderer/editor_camera.h"
#include "hyper/renderer/orthographic_camera.h"
#include "hyper/renderer/Texture.h"

//#include "hyper/scene/components.h"

namespace hp
{
	class HP_API renderer_2d
	{
	 public:
		static void init();
		static void shutdown();
		
		static void begin_scene(const camera& camera, const glm::mat4& transform);
		static void begin_scene(const editor_camera& camera);
		static void begin_scene(const orthographic_camera& camera); // TODO: Remove
		static void end_scene();
		static void flush();
		
		// Primitives
		static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void draw_quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<texture_2d>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
		
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<texture_2d>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
		static void draw_quad(const glm::mat4& transform, const glm::vec4& color, int entity_id = -1);
		static void draw_quad(const glm::mat4& transform, const std::shared_ptr<texture_2d>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f), int entity_id = -1);
		
		static void draw_rotated_quad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void draw_rotated_quad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<texture_2d>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
		static void draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<texture_2d>& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
		
		static void draw_circle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entity_id = -1);
		static void draw_line(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entity_id = -1);
		static void draw_rect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entity_id = -1);
		static void draw_rect(const glm::mat4& transform, const glm::vec4& color, int entity_id = -1);
		
		//static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);
		
		static float get_line_width();
		static void set_line_width(float width);
		
		// stats
		struct statistics
		{
			uint32_t draw_calls = 0;
			uint32_t quad_count = 0;
			
			uint32_t get_total_vertex_count() const
			{
				return quad_count * 4;
			}
			
			uint32_t get_total_index_count() const
			{
				return quad_count * 6;
			}
		};
		
		static void reset_stats();
		static statistics get_stats();
	 
	 private:
		static void start_batch();
		static void next_batch();
	};
}  // namespace hp

#endif