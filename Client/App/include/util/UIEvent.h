#pragma once

#include <G3DAll.h>
#include <SDL.h>

namespace RBX {
	class UserInputBase;
	class UIEvent {
	public:
		enum EventType {
			NO_EVENT,
			MOUSE_RIGHT_BUTTON_DOWN,
			MOUSE_RIGHT_BUTTON_UP,
			MOUSE_LEFT_BUTTON_DOWN,
			MOUSE_LEFT_BUTTON_UP,
			MOUSE_MOVE,
			MOUSE_DELTA,
			MOUSE_IDLE,
			MOUSE_WHEEL_FORWARD,
			MOUSE_WHEEL_BACKWARD,
			KEY_DOWN,
			KEY_UP,
		};
	public:
		EventType eventType;
		UserInputBase* userInput;
		union {
			struct {
				Vector2int16 mousePosition;
				Vector2int16 mouseDelta;
				Vector2int16 windowSize;
			};
			struct {
				SDLKey key;
				SDLMod mod;
			};
		};
	public:

	};
}