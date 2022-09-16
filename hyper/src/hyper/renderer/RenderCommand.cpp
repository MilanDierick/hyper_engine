#include "hyper/renderer/RenderCommand.h"

namespace hp
{
	std::unique_ptr<renderer_api> RenderCommand::s_RendererAPI = renderer_api::Create();
}