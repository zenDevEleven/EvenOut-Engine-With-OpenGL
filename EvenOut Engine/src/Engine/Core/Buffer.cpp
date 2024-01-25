#include "epch.h"
#include "Buffer.h"
#include "OpenGLBuffer.h"

namespace Engine 
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return std::make_shared<OpenGLVertexBuffer>(size);
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		return std::make_shared<OpenGLIndexBuffer>(indices, size);
	}

}
