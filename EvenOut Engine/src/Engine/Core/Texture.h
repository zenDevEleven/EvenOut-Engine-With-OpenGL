#pragma once
namespace Engine
{
	struct TextureData {

		TextureData() : d_X(0), d_Y(0), d_W(0), d_H(0) {}

		TextureData(float x, float y, float w, float h)
			: d_X(x), d_Y(y), d_W(w), d_H(h)
		{
		}

		SDL_FRect* FillRect(SDL_FRect& rect) {

			rect.x = this->d_X;
			rect.y = this->d_Y;
			rect.w = this->d_W;
			rect.h = this->d_H;

			return &rect;

		}

		float d_X;
		float d_Y;
		float d_W;
		float d_H;
	};


	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual float GetWidthF() const = 0;
		virtual float GetHeightF() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;

	};

	class Texture2D : public Texture 
	{
		public:
			static std::shared_ptr<Texture2D> Create(const std::string& path);

			void CreateAndLoadRects(TextureData rect, TextureData posRect);

			void LoadPositionRect(TextureData* posRect);
			void LoadRects(TextureData* rect, TextureData* posRect);
			void SetTextureColor(glm::vec4 color);
			void ClearTextureColor();
		public:
			SDL_FRect* t_ScreenPosition;
			SDL_FRect* t_ScreenRect;
			int t_PixelSize;
			glm::vec2 t_Size;
	};
}

