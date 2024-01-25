#include "epch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "ShaderRenderer.h"
#include "RenderCommand.h"

namespace Engine 
{
	static float MapClampRanged(const float& value, const float& oldMin, const float& oldMax,
		const float& newMin, const float& newMax)
	{
		float oldRange = (oldMax - oldMin);
		float NewRange = (newMax - newMin);
		float newValue = (((value - oldMin) * NewRange) / oldRange) + newMin;
		return newValue;
	}

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TextCoord;
		float TexIndex;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 5000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<Texture2D> whiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotsIndex = 1;

		Renderer2D::Statistics Stats;

		glm::vec4 QuadVertexPosition[4];
	};

	static Renderer2DData s_Data;


	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		BufferLayout layout =
		{
			{ ShaderDataType::Vec3, "a_Position" },
			{ ShaderDataType::Vec2, "a_TexCoord" },
			{ ShaderDataType::Vec, "a_TexIndex" }
		};

		s_Data.QuadVertexBuffer->SetLayout(layout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i+=6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> m_IndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(m_IndexBuffer);
		delete[] quadIndices;

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		ShaderRenderer::CreateVertexShader();
		ShaderRenderer::CreateFragmentShader();
		ShaderRenderer::CreateProgram();

		ShaderRenderer::Bind();
		ShaderRenderer::SetIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.whiteTexture;

		s_Data.QuadVertexPosition[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[1] = { 1.0f, 0.0f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[2] = { 1.0f, 1.0f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[3] = { 0.0f, 1.0f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		//delete[] quadIndices;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		ShaderRenderer::Bind();
		ShaderRenderer::SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		BeginBatch();
	}

	void Renderer2D::EndScene()
	{
		EndBatch();
	}

	void Renderer2D::BeginBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotsIndex = 1;
	}

	void Renderer2D::EndBatch()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotsIndex = 1;
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 1; i < s_Data.TextureSlotsIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;

	}

	void Renderer2D::DrawQuad(const glm::vec2& postion, const glm::vec2& size)
	{
		DrawQuad({postion.x, postion.y, 0.0f }, size);
	}

	void Renderer2D::DrawQuad(const glm::vec3& postion, const glm::vec2& size)
	{

		s_Data.QuadVertexBufferPtr->Position = postion;
		s_Data.QuadVertexBufferPtr->TextCoord = {0.0f, 0.0f};
		s_Data.QuadVertexBufferPtr->TexIndex = 0;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { postion.x + size.x, postion.y, 0.0f };
		s_Data.QuadVertexBufferPtr->TextCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { postion.x + size.x, postion.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->TextCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { postion.x, postion.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->TextCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = 0;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;


		/*glm::mat4 transform = glm::translate(glm::mat4(1.0f), postion) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		ShaderRenderer::SetMat4("u_Transform", transform);

		ShaderRenderer::Bind();
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& postion, const glm::vec2& size, SDL_FRect* sourceRect, const std::shared_ptr<Texture2D>& texture)
	{
		DrawQuad({ postion.x, postion.y, 0.0f }, size, sourceRect, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, SDL_FRect* sourceRect, const std::shared_ptr<Texture2D>& texture)
	{

		float spriteSheetWidth, spritesheetHeight;

		spriteSheetWidth = texture->GetWidth();
		spritesheetHeight = texture->GetHeight();

		int x = sourceRect->x / sourceRect->w;
		int y = sourceRect->y / sourceRect->h;

		int drawWidth = sourceRect->w * size.x;
		int drawHeight = sourceRect->h * size.y;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotsIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get()) 
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) 
		{
			textureIndex = (float)s_Data.TextureSlotsIndex;
			s_Data.TextureSlots[s_Data.TextureSlotsIndex] = texture;
			s_Data.TextureSlotsIndex++;
		}

		int drawPosX = position.x;
		int drawPosY = position.y;

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotsIndex > 31)
		{
			FlushAndReset();
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { drawPosX, drawPosY, 0.0f} ) * glm::scale(glm::mat4(1.0f), { drawWidth, drawHeight, 0.0f });


		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->TextCoord = { (x * sourceRect->w) / spriteSheetWidth,  (y * sourceRect->h) / spritesheetHeight };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->TextCoord = { ((x + 1) * sourceRect->w) / spriteSheetWidth,  (y * sourceRect->h) / spritesheetHeight };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->TextCoord = { ((x + 1) * sourceRect->w) / spriteSheetWidth,  ((y + 1) * sourceRect->h) / spritesheetHeight };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->TextCoord = { (x * sourceRect->w) / spriteSheetWidth,  ((y + 1) * sourceRect->h) / spritesheetHeight };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& postion, const glm::vec2& size, float rotation, SDL_FRect* sourceRect, const std::shared_ptr<Texture2D>& texture)
	{
		DrawRotatedQuad({ postion.x, postion.y, 0.0f }, size, rotation, sourceRect, texture);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& postion, const glm::vec2& size, float rotation, SDL_FRect* sourceRect, const std::shared_ptr<Texture2D>& texture)
	{

		float spriteSheetWidth, spritesheetHeight;

		spriteSheetWidth = texture->GetWidth();
		spritesheetHeight = texture->GetHeight();

		int x = sourceRect->x / sourceRect->w;
		int y = sourceRect->y / sourceRect->h;

		int drawWidth = sourceRect->w * size.x;
		int drawHeight = sourceRect->h * size.y;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotsIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotsIndex;
			s_Data.TextureSlots[s_Data.TextureSlotsIndex] = texture;
			s_Data.TextureSlotsIndex++;
		}

		int drawPosX = postion.x;
		int drawPosY = postion.y;

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotsIndex > 31)
		{
			FlushAndReset();
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { drawPosX, drawPosY, 0.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { drawWidth, drawHeight, 0.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->TextCoord = { (x * sourceRect->w) / spriteSheetWidth,  (y * sourceRect->h) / spritesheetHeight };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->TextCoord = { ((x + 1) * sourceRect->w) / spriteSheetWidth,  (y * sourceRect->h) / spritesheetHeight };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->TextCoord = { ((x + 1) * sourceRect->w) / spriteSheetWidth,  ((y + 1) * sourceRect->h) / spritesheetHeight };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->TextCoord = { (x * sourceRect->w) / spriteSheetWidth,  ((y + 1) * sourceRect->h) / spritesheetHeight };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}


	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics)); 
	}

}
