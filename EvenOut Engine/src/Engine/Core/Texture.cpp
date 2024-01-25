#include "epch.h"
#include "OpenGLTexture.h"
#include "Texture.h"

namespace Engine
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		return std::make_shared<OpenGLTexture2D>(path);
	}


	void Texture2D::CreateAndLoadRects(TextureData rect, TextureData posRect)
	{
		t_ScreenPosition = new SDL_FRect();
		t_ScreenRect = new SDL_FRect();

		t_ScreenRect->x = rect.d_X;
		t_ScreenRect->y = rect.d_Y;
		t_ScreenRect->w = rect.d_W;
		t_ScreenRect->h = rect.d_H;

		t_ScreenPosition->x = posRect.d_X;
		t_ScreenPosition->y = posRect.d_Y;
		t_ScreenPosition->w = posRect.d_W;
		t_ScreenPosition->h = posRect.d_H;
	}

	void Texture2D::LoadRects(TextureData* rect, TextureData* posRect)
	{
		t_ScreenRect->x = rect->d_X;
		t_ScreenRect->y = rect->d_Y;
		t_ScreenRect->w = rect->d_W;
		t_ScreenRect->h = rect->d_H;

		t_ScreenPosition->x = posRect->d_X;
		t_ScreenPosition->y = posRect->d_Y;
		t_ScreenPosition->w = posRect->d_W;
		t_ScreenPosition->h = posRect->d_H;
	}

	void Texture2D::LoadPositionRect(TextureData* posRect) {
		t_ScreenPosition->x = posRect->d_X;
		t_ScreenPosition->y = posRect->d_Y;
		t_ScreenPosition->w = posRect->d_W;
		t_ScreenPosition->h = posRect->d_H;
	}
}
