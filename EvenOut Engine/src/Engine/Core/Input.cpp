#include "epch.h"
#include "Input.h"

namespace Engine {
	SDL_Gamepad* Input::m_Pad;
	
	void Input::SendGamepad(SDL_Gamepad* controller)
	{ 
		m_Pad = controller;
	}

	bool Input::IsGamepadButtonPressed(SDL_GamepadButton button, bool singleClick)
	{
		if (singleClick) {
			static bool flag = false;
			//std::this_thread::sleep_for(0.1s);
			if (SDL_GetGamepadButton(m_Pad, button) != 0 && !flag) {
				flag = true;
				return true;
			}
			if (SDL_GetGamepadButton(m_Pad, button) == 0) {
				flag = false;
				return false;
			}
		}
		else {
			if (SDL_GetGamepadButton(m_Pad, button) != 0) {
				return true;
			}
		}
		return false;
	}

	float Input::GetGamepadAxis(SDL_GamepadAxis axis)
	{
		Sint16 value = SDL_GetGamepadAxis(m_Pad, axis);

		return value = std::clamp((float)value, -1.0f, 1.0f);
	}
}
