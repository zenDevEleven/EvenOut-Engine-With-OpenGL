#pragma once

#include "RendererAPI.h"

namespace Engine
{
	class OpenGLRendererAPI : public RendererAPI
	{

	public:
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

	};
}

