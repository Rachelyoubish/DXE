#include "scpch.h"
#include "RenderCommand.h"

#include "Platform/Direct3D/Direct3DRendererAPI.h"

namespace Seacrest {

    RendererAPI* RenderCommand::s_RendererAPI = new Direct3DRendererAPI;
}