#pragma once

#include "util/IRenderable.h"
#include "util/Name.h"
#include "util/UIEvent.h"

namespace RBX 
{
	class XmlState;
	class Workspace;
	class TextureId;

	class MouseCommand : public RBX::INamed, public RBX::IRenderable 
	{
	private:
		bool capturedMouse;
	protected:
		Workspace* workspace;
		std::auto_ptr<XmlState> undoState;
	public:
		virtual MouseCommand* onMouseDown(const UIEvent&);

	protected:
		virtual void onMouseIdle(const UIEvent&);
		virtual void onMouseHover(const UIEvent&);
		virtual MouseCommand* onKeyDown(const UIEvent&);
		virtual MouseCommand* onPeekKeyDown(const UIEvent&);
		virtual void onMouseMove(const UIEvent&);
		virtual void onMouseDelta(const UIEvent&);
		virtual MouseCommand* onMouseUp(const UIEvent&);
		virtual void capture();
		virtual void releaseCapture();
		virtual void cancel();
		MouseCommand(Workspace*);

	public:
		virtual ~MouseCommand();
		virtual TextureId getTextureId() const;
		virtual MouseCommand* isSticky() const;

	private:
		virtual const std::string getCursorName() const;
	};
}