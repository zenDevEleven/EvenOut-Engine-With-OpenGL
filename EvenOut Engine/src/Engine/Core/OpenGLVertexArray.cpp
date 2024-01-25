#include "epch.h"
#include "OpenGLVertexArray.h"


namespace Engine
{
	static GLenum ShaderDatatTypeToOpenGLDataType(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::Vec:	return GL_FLOAT;
		case Engine::ShaderDataType::Vec2:	return GL_FLOAT;
		case Engine::ShaderDataType::Vec3:	return GL_FLOAT;
		case Engine::ShaderDataType::Vec4:	return GL_FLOAT;
		case Engine::ShaderDataType::Int:	return GL_INT;
		case Engine::ShaderDataType::Int2:	return GL_INT;
		case Engine::ShaderDataType::Int3:	return GL_INT;
		case Engine::ShaderDataType::Int4:	return GL_INT;
		case Engine::ShaderDataType::Bool:	return GL_BOOL;
		}

		LOG_CORE("Unknown ShaderDataType!", LOG_ERROR);

		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetElementCount(),
				ShaderDatatTypeToOpenGLDataType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(), (const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
