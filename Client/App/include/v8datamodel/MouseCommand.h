#pragma once

#include "util/IRenderable.h"
#include "util/Name.h"

namespace RBX {
	class XmlState;
	class Workspace;
	class TextureId;
	class UIEvent;

	class MouseCommand : public RBX::INamed, public RBX::IRenderable {
	private:
		bool capturedMouse;
	protected:
		Workspace* workspace;
		std::auto_ptr<XmlState> undoState;
	public:
		virtual RBX::MouseCommand* onMouseDown(const UIEvent&);
	protected:
		virtual void onMouseIdle(const UIEvent&);
		virtual void onMouseHover(const UIEvent&);
		virtual RBX::MouseCommand* onKeyDown(const UIEvent&);
		virtual RBX::MouseCommand* onPeekKeyDown(const UIEvent&);
		virtual void onMouseMove(const UIEvent&);
		virtual void onMouseDelta(const UIEvent&);
		virtual RBX::MouseCommand* onMouseUp(const UIEvent&);
		virtual void capture();
		virtual void releaseCapture();
		virtual void cancel();
	public:
		virtual ~MouseCommand();
		virtual TextureId getTextureId() const;
		virtual MouseCommand* isSticky() const;
	private:
		virtual const std::string getCursorName() const;

	};
}