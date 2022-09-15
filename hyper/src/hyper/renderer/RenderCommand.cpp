#include "hyper/renderer/RenderCommand.h"

namespace hp
{
	std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}