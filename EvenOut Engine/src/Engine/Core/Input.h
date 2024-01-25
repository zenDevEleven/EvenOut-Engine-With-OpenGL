#pragma once

namespace Engine {
	class Input {
	public:
		static void SendGamepad(SDL_Gamepad* controller);
		static bool IsGamepadButtonPressed(SDL_GamepadButton button, bool singleClick);
		static float GetGamepadAxis(SDL_GamepadAxis axis);
	private:
		static SDL_Gamepad* m_Pad;
	};
}