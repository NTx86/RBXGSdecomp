#pragma once

#include "v8world/IRenderable.h"
#include "v8world/INamed.h"

namespace RBX {
	class XmlState;
	class Workspace;

	class MouseCommand : public RBX::INamed, public RBX::IRenderable {
	private:
		bool capturedMouse;
	protected:
		Workspace* workspace;
		std::auto_ptr<XmlState> undoState;

		virtual void capture();
	public:
		virtual MouseCommand* isSticky() const {
			return 0;
		}
	private:
		virtual const std::string getCursorName() const;

	};
}