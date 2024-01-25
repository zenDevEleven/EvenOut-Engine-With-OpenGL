#include "epch.h"
#include "VertexArray.h"

#include "OpenGLVertexArray.h"

namespace Engine
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		return std::make_shared<OpenGLVertexArray>();
	}

}