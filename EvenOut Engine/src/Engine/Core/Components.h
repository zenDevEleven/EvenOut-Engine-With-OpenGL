#pragma once
#include "epch.h"
#include "Component.h"

#include "Engine/Core/Renderer.h"
#include "Engine/Core/Texture.h"

#include "Engine/Core/PhysicsWorld.h"
#include "GameEngine.h"
#include "Renderer2D.h"
#include "ShaderRenderer.h"

using namespace std::chrono_literals;

namespace Engine {

	struct TransformComponent : public Component{
		glm::vec2 Position;

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec2& position)
			: Position(position)
		{
		}

		~TransformComponent() {
		}

		void Start() override
		{
			
		}

		void UpdateComponent(float deltaTime) override {

		}

		void DrawComponent() override
		{

		}

		void SetPosition(float x, float y) {
			Position.x = x;
			Position.y = y;
		}

		void AddPos(float x, float y) {
 			Position.x += x;
			Position.y += y;
		}


	};

	struct AnimatorComponent : public Component{

		struct Animation;
		struct AnimationFrame;

		AnimatorComponent() = default;
		AnimatorComponent(const AnimatorComponent& other) = default;

		AnimatorComponent(std::shared_ptr<Texture2D>& texture)
			: m_Texture(texture)
		{
		}

		~AnimatorComponent() {
		}

		void Start() override
		{
		}

		void UpdateComponent(float deltaTime) override
		{

		}

		void DrawComponent() override
		{
			
		}

		void SetStartFrame(AnimationFrame&& frame) {
			if (m_Texture->t_PixelSize == -1) {
				std::cout << "Specify a pixel size when creating a SpriteRenderer2D" << std::endl;
			}
			else {
				int frameX = frame.f_x * m_Texture->t_PixelSize;
				int frameY = frame.f_y * m_Texture->t_PixelSize;
				m_Texture->t_ScreenRect->x = frameX;
				m_Texture->t_ScreenRect->y = frameY;
			}
		}

		void CreateAnimation(Animation* animation) {
			m_Animations.emplace_back(animation);
		}		
		
		void PlayAnimationContiniousToFrame(std::string animationName, float frameRate, float deltaTime, bool looping)
		{
			Animation* a = FindAnimation(animationName);

			m_Time += deltaTime;

			if (!a->a_Finished) {
				if (m_Time >= frameRate)
				{
					m_Time = 0;

					if (a->a_CurrentFrame.f_x * m_Texture->t_Size.x >= m_Texture->GetWidthF()) {
						a->a_CurrentFrame.f_x = 0;
						a->a_CurrentFrame.f_y += 1;
						if (a->a_CurrentFrame.f_y * m_Texture->t_Size.y >= m_Texture->GetHeightF()) {
							if (looping) {
								a->a_CurrentFrame = a->a_Frames[0];
							}
							else {
								a->a_Finished = true;
							}
						}
					}

					m_Texture->t_ScreenRect->x = a->a_CurrentFrame.f_x * m_Texture->t_Size.x;
					m_Texture->t_ScreenRect->y = a->a_CurrentFrame.f_y * m_Texture->t_Size.y;
					a->a_CurrentFrame.f_x += 1;
				}
			}

		}

		void PlayAnimation(std::string animationName, float frameRate, float deltaTime, bool looping) {
			Animation* a = FindAnimation(animationName);
			if (!a->a_Finished) {
				m_Time += deltaTime;
				if (m_Time >= frameRate) {
					int nextFrame = GetNextFrame(*a, m_Texture->t_ScreenRect->x, m_Texture->t_ScreenRect->y);

					if (a->a_Frames.size() == nextFrame) {
						if (looping) {
							nextFrame = 0;
							m_Texture->t_ScreenRect->x = a->a_Frames[nextFrame].f_x * m_Texture->t_Size.x;
							m_Texture->t_ScreenRect->y = a->a_Frames[nextFrame].f_y * m_Texture->t_Size.y;
							a->a_Finished = false;
						}
					}
					else {

						m_Time = 0;
						m_Texture->t_ScreenRect->x = a->a_Frames[nextFrame].f_x * m_Texture->t_Size.x;
						m_Texture->t_ScreenRect->y = a->a_Frames[nextFrame].f_y * m_Texture->t_Size.y;
					}
				}
			}
		}

		bool IsPlayingAnimation(std::string animationName) {
			Animation* a = FindAnimation(animationName);
			return !a->a_Finished;
		}

	private:
		Animation* FindAnimation(std::string& name) {
			for (Animation* a : m_Animations)
			{
				if (a->a_Name == name) {
					return a;
				}
			}

			//not found animation
		}

		int GetNextFrame(Animation a, int x, int y)
		{
			for (int i = 0; i < a.a_Frames.size(); ++i)
			{
				if ((a.a_Frames[i].f_x * m_Texture->t_Size.x) == x && (a.a_Frames[i].f_y * m_Texture->t_Size.y) == y) {
					if (i + 1 < a.a_Frames.size()) {
						return i + 1;
					}
					else {
						return a.a_Frames.size();
					}
				}
			}

			return 0;
		}

	public:
		std::shared_ptr<Texture2D> m_Texture;

		std::vector<Animation*> m_Animations = {};

		struct AnimationFrame {
			AnimationFrame() {}
			AnimationFrame(int x, int y)
			{
				f_x = x - 1;
				f_y = y - 1;
			}

			int f_x;
			int f_y;
		};

		struct Animation {
			Animation(std::string name, std::vector<AnimationFrame> frames)
				: a_Name(name), a_Frames(frames)
			{
				a_CurrentFrame = frames[0];
			}

			std::string a_Name;
			std::vector<AnimationFrame> a_Frames;
			AnimationFrame a_CurrentFrame;
			bool a_Finished = false;
		};

	private:
		float m_Time = 0;

	};

	struct SpriteRenderer2D : public Component{
		SpriteRenderer2D() {};

		SpriteRenderer2D(const char* filepath, int sortingLayer)
		{
			m_SpriteRect = TextureData(0.0f, 0.0f, 0.0f, 0.0f);
			m_SpritePos = TextureData(0.0f, 0.0f, 0.0f, 0.0f);
			m_SpriteTexture = Texture2D::Create(filepath);
			m_SpriteTexture->CreateAndLoadRects(m_SpriteRect, m_SpritePos);

			m_SpriteTexture->t_PixelSize = -1;
			m_SortingLayer = sortingLayer;
		}

		SpriteRenderer2D(const char* filepath, int SpritePixelSize, int sortingLayer)
		{
			m_SpriteRect = TextureData(0.0f, 0.0f, SpritePixelSize, SpritePixelSize);
			m_SpritePos = TextureData(0.0f, 0.0f, SpritePixelSize, SpritePixelSize);
			m_SpriteTexture = Texture2D::Create(filepath);
			m_SpriteTexture->CreateAndLoadRects(m_SpriteRect, m_SpritePos);

			m_SpriteTexture->t_PixelSize = SpritePixelSize;
			m_SpriteTexture->t_Size = { SpritePixelSize, SpritePixelSize };
			m_SortingLayer = sortingLayer;
		}

		SpriteRenderer2D(const char* filepath, int SpritePixelSize, int scale, int sortingLayer)
		{
			m_SpriteRect = TextureData(0.0f, 0.0f, SpritePixelSize, SpritePixelSize);
			m_SpritePos = TextureData(0.0f, 0.0f, SpritePixelSize * scale, SpritePixelSize * scale);
			m_SpriteTexture = Texture2D::Create(filepath);
			m_SpriteTexture->CreateAndLoadRects(m_SpriteRect, m_SpritePos);

			m_SpriteTexture->t_PixelSize = SpritePixelSize;
			m_SpriteTexture->t_Size = { SpritePixelSize, SpritePixelSize };
			m_SortingLayer = sortingLayer;
		}

		SpriteRenderer2D(const char* filepath, glm::vec2 size, int sortingLayer)
		{
			m_SpriteRect = TextureData(0.0f, 0.0f, size.x, size.y);
			m_SpritePos = TextureData(0.0f, 0.0f, size.x, size.y);
			m_SpriteTexture = Texture2D::Create(filepath);
			m_SpriteTexture->CreateAndLoadRects(m_SpriteRect, m_SpritePos);


			m_SpriteTexture->t_Size = size;
			m_SortingLayer = sortingLayer;
		}

		~SpriteRenderer2D() {
		}

		void Start()
		{
			m_Transform = &m_Actor->GetComponent<TransformComponent>();
		}

		void UpdateComponent(float deltaTime)  
		{
			m_SpritePos.d_X = m_Transform->Position.x;
			m_SpritePos.d_Y = m_Transform->Position.y;
			m_SpriteTexture->LoadPositionRect(&m_SpritePos);
		}

		void SetRotation(float rotationInDegrees) 
		{
			m_RotateQuad = true;
			m_Rotation = rotationInDegrees;
		}

		void SetIsActive(bool active) {
			IsActive = active;
		}

		void DrawComponent()
		{
			if (IsActive) {
				glm::vec2 scale = { m_SpriteTexture->t_ScreenPosition->w / m_SpriteTexture->t_Size.x,
									m_SpriteTexture->t_ScreenPosition->h / m_SpriteTexture->t_Size.y };
				if (m_RotateQuad) {
					Renderer2D::DrawRotatedQuad({ m_SpriteTexture->t_ScreenPosition->x, m_SpriteTexture->t_ScreenPosition->y },
						scale, m_Rotation,
						m_SpriteTexture->t_ScreenRect,
						m_SpriteTexture);
				}
				else {
					Renderer2D::DrawQuad({ m_SpriteTexture->t_ScreenPosition->x, m_SpriteTexture->t_ScreenPosition->y },
						scale,
						m_SpriteTexture->t_ScreenRect,
						m_SpriteTexture);
				}
			}
		}

		std::shared_ptr<Texture2D> m_SpriteTexture;
		std::shared_ptr<Texture2D> m_CurrentUsingTexture;
		TextureData m_SpriteRect;
		TextureData m_SpritePos;
		TransformComponent* m_Transform;

		int m_Rotation = 0;
		bool m_RotateQuad = false;

		bool IsActive = true;

		int m_SortingLayer;

	};

	struct Rigidbody2D : public Component{

		Rigidbody2D() {}

		~Rigidbody2D()
		{
			PhysicsWorld::GetPhysicsWorld().DestroyBody(m_Body);
			m_Body = nullptr;
		}

		enum class BodyType { Static = 0, Dynamic, Kinematic };
		Rigidbody2D(BodyType type) : Type(type) {}
		Rigidbody2D(const Rigidbody2D& other) = default;

		BodyType Type = BodyType::Dynamic;
		bool FixedRotation = false;

		b2BodyDef m_bodyDef;
		b2Body* m_Body = nullptr;


		void Start() 
		{
			auto& transform = m_Actor->GetComponent<TransformComponent>();

			switch (Type) {
			case BodyType::Dynamic:
				m_bodyDef.type = b2_dynamicBody;
				break;
			case BodyType::Static:
				m_bodyDef.type = b2_staticBody;
				break;
			case BodyType::Kinematic:
				m_bodyDef.type = b2_kinematicBody;
				break;

			}

			m_bodyDef.position.Set(transform.Position.x, transform.Position.y);

			m_bodyDef.fixedRotation = true;
			m_bodyDef.gravityScale = 0.0f;
			m_bodyDef.bullet = true;

			if(!GameEngine::GetEngine()->IsRunning())
				m_Body = PhysicsWorld::GetPhysicsWorld().CreateBody(m_bodyDef);
			else
				PhysicsWorld::GetPhysicsWorld().QueueCreateBody(m_Body, m_bodyDef);
		}

		void UpdateComponent(float deltaTime)
		{
			auto& transform = m_Actor->GetComponent<TransformComponent>();

			if (m_Body != nullptr)
				m_Body->SetTransform(b2Vec2(transform.Position.x, transform.Position.y), 0.0f);
		}


		void DrawComponent()
		{
			
		}

	};

	struct BoxCollider2DComponent : public Component
	{
		glm::vec2 Offset{ 0.0f,0.0f };
		glm::vec2 Size{ 0.5f,0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;


		Rigidbody2D* m_Rb;

		void* RuntimeFixture = nullptr;

		b2FixtureDef fixtureDef;

		BoxCollider2DComponent(glm::vec2 size, Rigidbody2D* rb) : Size(size), m_Rb(rb) {
		}
		BoxCollider2DComponent(const BoxCollider2DComponent& other) = default;
		
		~BoxCollider2DComponent() {
		}

		void Start() override
		{
			b2PolygonShape shape;
			shape.SetAsBox(Size.x / 2, Size.y / 2);

			fixtureDef.shape = &shape;
			fixtureDef.density = Density;
			fixtureDef.friction = Friction;
			fixtureDef.restitution = Restitution;
			fixtureDef.restitutionThreshold = RestitutionThreshold;

			fixtureDef.userData.pointer = (uintptr_t)m_Actor;
			fixtureDef.isSensor = true;


			if (!GameEngine::GetEngine()->IsRunning()) 
				m_Rb->m_Body->CreateFixture(&fixtureDef);
			else 
				PhysicsWorld::GetPhysicsWorld().QueueFixtureCreation(m_Rb->m_Body, fixtureDef, shape);
			

		}

		void UpdateComponent(float deltaTime) override
		{
			
		}

		void DrawComponent() override
		{

		}
	};

	struct HealthComponent : public Component {

	public:
		using CallbackFunction = std::function<void()>;

		HealthComponent(int MaxhealthSegments) 
			: m_MaxHealthSegments(MaxhealthSegments) {}

		HealthComponent(float MaxHealth)
			: m_MaxHealth(MaxHealth) {}

		template <typename T>
		void setOnDieCallback(T* instance, void (T::* memberFunction)()) {
			m_OnDieCallback = std::bind(memberFunction, instance);
		}

		void Start() override
		{
			m_HealthSegment = m_MaxHealthSegments;
			m_Health = m_MaxHealth;
		}

		void AddHealthSegment(int segmetsToAdd) 
		{
			m_HealthSegment += segmetsToAdd;
			if (m_HealthSegment > m_MaxHealthSegments)
				m_HealthSegment = m_MaxHealthSegments;
		}

		void AddHealth(float healthToAdd)
		{
			m_Health += healthToAdd;
			if (m_Health > m_MaxHealth)
				m_Health = m_MaxHealth;
		}

		void TakeDamageSegment(int segmentsToTakeDmg) {
			m_HealthSegment -= segmentsToTakeDmg;
			if (m_HealthSegment <= 0) {
				m_HealthSegment = 0;
				m_OnDieCallback();
			}
		}

		void TakeDamage(float healthToTakeDmg) {
			m_Health -= healthToTakeDmg;
			if (m_Health <= 0) {
				m_Health = 0;
				std::cout << "TAKE DAMAGE MY LIFE NOW: " << m_Health << std::endl;
				m_OnDieCallback();
			}
		}

		int GetHealthSegments() { return m_HealthSegment; }
		float GetHealth() { return m_Health; }


		void UpdateComponent(float deltaTime) override
		{
			
		}
		void DrawComponent() override
		{
			
		}

	private:
		int m_MaxHealthSegments = 0;
		int m_HealthSegment = 0;

		CallbackFunction m_OnDieCallback;

		float m_Health = 0;
		float m_MaxHealth = 0;


	};

	struct TextRendererComponent : public Component
	{
		TextRendererComponent() {};
		TextRendererComponent(
			std::string TextureFontAtlas, 
			std::vector <char> atlasData, 
			std::string text,
			glm::vec2 drawPosition) 
			:
			currentTextureFontData(atlasData), 
			currentPosition(drawPosition)
		{
			if (TextureFontAtlas.contains("64"))
				atlasPixelSize = 64;
			else if (TextureFontAtlas.contains("32"))
				atlasPixelSize = 32;
			else if (TextureFontAtlas.contains("16"))
				atlasPixelSize = 16;
			else if (TextureFontAtlas.contains("8"))
				atlasPixelSize = 8;
			else if (TextureFontAtlas.contains("4"))
				atlasPixelSize = 4;

			currentTextureFontAtlas = Texture2D::Create(TextureFontAtlas);

			sourceRect = TextureData(0.0f, 0.0f, atlasPixelSize, atlasPixelSize);
			posRect = TextureData(0.0f, 0.0f, atlasPixelSize, atlasPixelSize);

			currentTextureFontAtlas->CreateAndLoadRects(sourceRect, posRect);

			TextToRender = text;
		}

		void SetText(std::string textToRender)
		{
			TextToRender = textToRender;
		}

		void WriteText(std::string textToDraw, glm::vec2 drawPosition) 
		{	
			char* char_array = new char[textToDraw.length() + 1];

			char_array[textToDraw.length()] = '\0';

			for (int i = 0; i < textToDraw.length(); i++) {
				char_array[i] = textToDraw[i];
			}


			int column = 0;
			int line = 0;
			int lineCounter = 0;
			bool haveFound = false;

			for (int i = 0; i < textToDraw.length(); i++)
			{
				column = 0;
				line = 0;
				lineCounter = 0;
				haveFound = false;

				for (int j = 0; j < currentTextureFontData.size(); j++)
				{

					if (lineCounter * atlasPixelSize == currentTextureFontAtlas->GetWidth()) {
						line++;
						lineCounter = 0;
						column = 0;
					}
					if (char_array[i] == currentTextureFontData[j] && !haveFound) 
					{
						haveFound = true;

						sourceRect.d_X = column * atlasPixelSize;
						sourceRect.d_Y = line * atlasPixelSize;

						currentTextureFontAtlas->LoadRects(&sourceRect, &posRect);

						float pixelsToAdd = (i * atlasPixelSize);
						Renderer2D::DrawQuad(
							{ drawPosition.x + pixelsToAdd , drawPosition.y },
							{ 1.0f, 1.0f },
							currentTextureFontAtlas->t_ScreenRect,
							currentTextureFontAtlas);
					}
					
					column++;
					lineCounter++;

				}
			}
		}

		void Start() override
		{

		}

		void AddPosition(glm::vec2 position) {
			currentPosition += position;
		}

		void UpdateComponent(float deltaTime) override
		{

		}

		void DrawComponent() override
		{
			WriteText(TextToRender, currentPosition);
		}

	private:
		std::shared_ptr<Texture2D> currentTextureFontAtlas;
		std::vector <char> currentTextureFontData;
		glm::vec2 currentPosition;
		TextureData sourceRect;
		TextureData posRect;
		std::string TextToRender;

		int atlasPixelSize = -1;


	};

	struct ImageRendererComponent : public Component {

	public:
		ImageRendererComponent(const char* filepath, glm::vec2 DrawPosition)
			: m_DrawPosition(DrawPosition)
		{
			m_ImageTexture = Texture2D::Create(filepath);
			m_ImageRect = TextureData(0.0f, 0.0f, m_ImageTexture->GetWidth(), m_ImageTexture->GetHeight());
			m_ImagePos = TextureData(0.0f, 0.0f, m_ImageTexture->GetWidth(), m_ImageTexture->GetHeight());

			m_ImageTexture->CreateAndLoadRects(m_ImageRect, m_ImagePos);


			m_ImageTexture->t_Size = { m_ImageTexture->GetWidth(), m_ImageTexture->GetHeight() };
		}

		void SetSize(glm::vec2 newSize) {
			m_ImageRect.d_W = newSize.x;
			m_ImageRect.d_H = newSize.y;

			m_ImagePos.d_W = newSize.x;
			m_ImagePos.d_H = newSize.y;

			m_ImageTexture->t_Size = { newSize.x, newSize.y };


			m_ImageTexture->LoadRects(&m_ImageRect, &m_ImagePos);
		}

		void SetScale(float scale) {

			m_ImagePos.d_W = m_ImageRect.d_W * scale;
			m_ImagePos.d_H = m_ImageRect.d_H * scale;

			m_ImageTexture->LoadPositionRect(&m_ImagePos);
		}

		void SetPosition(glm::vec2 newSize) {
			m_ImageRect.d_X = newSize.x;
			m_ImageRect.d_Y = newSize.y;

			m_ImageTexture->LoadRects(&m_ImageRect, &m_ImagePos);
		}

		void Start() override
		{
			
		}

		void UpdateComponent(float deltaTime) override
		{
			
		}

		void SetFlexibility(bool isFlexible, int timesToRender) 
		{
			m_isFlexible = isFlexible;
			m_numberOfTimesToGetRendered = timesToRender;
		}

		enum class TilingDirection {
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		void SetTiling(int pixelsPerTile, TilingDirection direction) {
			m_TilingDirection = direction;
			m_PixelsPerTile = pixelsPerTile;
		}

		void DrawComponent() override
		{
			glm::vec2 scale = { m_ImageTexture->t_ScreenPosition->w / m_ImageTexture->t_Size.x,
				m_ImageTexture->t_ScreenPosition->h / m_ImageTexture->t_Size.y };

			if (!m_isFlexible) 
			{ 
				Renderer2D::DrawQuad({ m_DrawPosition.x, m_DrawPosition.y },
					scale,
					m_ImageTexture->t_ScreenRect,
					m_ImageTexture);
			}
			else 
			{
				if (m_PixelsPerTile == -1)
					m_PixelsPerTile = m_ImageTexture->t_ScreenPosition->x;

				for (int i = 0; i < m_numberOfTimesToGetRendered; i++)
				{
					switch (m_TilingDirection)
					{
					case ImageRendererComponent::TilingDirection::UP:
						Renderer2D::DrawQuad({ m_DrawPosition.x , m_DrawPosition.y - (m_PixelsPerTile * i) },
							scale,
							m_ImageTexture->t_ScreenRect,
							m_ImageTexture);
						break;	
					case ImageRendererComponent::TilingDirection::DOWN:
						Renderer2D::DrawQuad({ m_DrawPosition.x, m_DrawPosition.y + (m_PixelsPerTile * i) },
							scale,
							m_ImageTexture->t_ScreenRect,
							m_ImageTexture);
					case ImageRendererComponent::TilingDirection::LEFT:
						Renderer2D::DrawQuad({ m_DrawPosition.x - (m_PixelsPerTile * i), m_DrawPosition.y },
							scale,
							m_ImageTexture->t_ScreenRect,
							m_ImageTexture);
					case ImageRendererComponent::TilingDirection::RIGHT:
						Renderer2D::DrawQuad({ m_DrawPosition.x + (m_PixelsPerTile * i), m_DrawPosition.y },
							scale,
							m_ImageTexture->t_ScreenRect,
							m_ImageTexture);
						break;
					}
				}
			}
		}

		std::shared_ptr<Texture2D> m_ImageTexture;

	private:
		bool m_isFlexible = false;
		int m_numberOfTimesToGetRendered;
		int m_PixelsPerTile = -1;
		TilingDirection m_TilingDirection = TilingDirection::LEFT;

		TextureData m_ImageRect;
		TextureData m_ImagePos;
		glm::vec2 m_DrawPosition;
	};
}

