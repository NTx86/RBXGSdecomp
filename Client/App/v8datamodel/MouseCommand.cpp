#include "v8datamodel/MouseCommand.h"
#include "util/Debug.h"
namespace RBX {

	const std::string MouseCommand::getCursorName() const {
		return "ArrowCursor";
	}

	void MouseCommand::releaseCapture() {
		this->capturedMouse = false;
	}
	
	void MouseCommand::cancel() {
		if (this->capturedMouse != false) {
			releaseCapture();
		}
	}

	MouseCommand* MouseCommand::isSticky() const {
			return NULL;
	}
}