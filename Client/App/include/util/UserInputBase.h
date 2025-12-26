#pragma once
#include "RbxGraphics/TextureProxyBase.h"
#include "util/TextureId.h"
#include <G3D/Vector2.h>
#include <SDL.h>

namespace RBX
{
	class Adorn;
	class UserInputBase
	{
	public:
		enum WrapMode
		{
			WRAP_AUTO,
			WRAP_CENTER
		};

	private:
		ContentId currentCursorId;
		G3D::ReferenceCountedPointer<TextureProxyBase> currentCursor;
	  
	protected:
		virtual G3D::Vector2 getCursorPosition();
		virtual G3D::ReferenceCountedPointer<TextureProxyBase> getGameCursor(Adorn*);
	public:
		//UserInputBase(const UserInputBase&);
		UserInputBase();
	public:
		void init(Adorn*);
	public:
		~UserInputBase();
	public:
		virtual void setWrapMode(WrapMode);
		virtual WrapMode getWrapMode() const;
		virtual void centerCursor();
		virtual bool keyDown(SDLKey) const;
		bool altKeyDown() const;
		bool shiftKeyDown() const;
		bool ctrlKeyDown() const;
		virtual void setKeyState(SDLKey, bool);
		virtual void setCursorId(Adorn*, const TextureId&);
		void renderGameCursor(Adorn*);
	public:
		//UserInputBase& operator=(const UserInputBase&);
	};
}
