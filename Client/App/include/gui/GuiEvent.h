#pragma once
#include "util/UIEvent.h"

namespace RBX
{
	class IDataState;
	class GuiTarget;

	class GuiEvent : public UIEvent
	{
	public:
		IDataState* iDataState;
	  
	public:
		GuiEvent(const UIEvent&, IDataState*);
	};

	class GuiResponse
	{
	private:
		enum ResponseType
		{
			NOT_USED,
			USED,
			USED_AND_FINISHED
		};

	private:
		ResponseType response;
		GuiTarget* target;
	  
	private:
		GuiResponse(ResponseType response, GuiTarget* target)
			: response(response),
			  target(target)
		{
		}
		GuiResponse(ResponseType response)
			: response(response)
		{
		}
	public:
		bool wasUsed();
		bool wasNotUsed();
		bool wasFinished();
		GuiTarget* getTarget();
	  
	public:
		static GuiResponse notUsed()
		{
			return GuiResponse(NOT_USED);
		}
		static GuiResponse used(GuiTarget*);
		static GuiResponse used();
		static GuiResponse finished();
	};

	class GuiTarget
	{
	public:
		// TODO: check match
		virtual GuiResponse process(const GuiEvent& guiEvent)
		{
			return GuiResponse::notUsed();
		}
	public:
		//GuiTarget(const GuiTarget&);
		GuiTarget()
		{
		}
	public:
		//GuiTarget& operator=(const GuiTarget&);
	};
}
