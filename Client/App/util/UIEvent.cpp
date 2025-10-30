#include "util/UIEvent.h"

namespace RBX
{
	bool UIEvent::isMouseEvent() const
	{
		return eventType == MOUSE_RIGHT_BUTTON_DOWN 
        || eventType == MOUSE_RIGHT_BUTTON_UP 
        || eventType == MOUSE_LEFT_BUTTON_DOWN 
        || eventType == MOUSE_LEFT_BUTTON_UP 
		|| eventType == MOUSE_MOVE 
        || eventType == MOUSE_DELTA 
        || eventType == MOUSE_IDLE; 
	}
}