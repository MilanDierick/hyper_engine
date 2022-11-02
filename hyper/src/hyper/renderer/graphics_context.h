#ifndef HYPER_GRAPHICS_CONTEXT_H
#define HYPER_GRAPHICS_CONTEXT_H

namespace hp
{
	class graphics_context
	{
	public:
		graphics_context()          = default;
		virtual ~graphics_context() = default;

		graphics_context(const graphics_context& other)            = delete;
		graphics_context(graphics_context&& other) noexcept        = delete;
		graphics_context& operator=(const graphics_context& other) = delete;
		graphics_context& operator=(graphics_context&& other)      = delete;

		virtual void init()         = 0;
		virtual void swap_buffers() = 0;

		static std::unique_ptr<graphics_context> create(void* window);
	};
} // namespace hp

#endif //HYPER_GRAPHICS_CONTEXT_H