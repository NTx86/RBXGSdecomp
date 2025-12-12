#pragma once
#include <vector>

namespace RBX
{
	template<typename Class, typename Event>
	class Notifier;

	struct RaiseRange
	{
	public:
		unsigned int index;
		unsigned int upper;
		RaiseRange* previous;
	public:
		void removeIndex(unsigned int);
	};

	template<typename Class, typename Event>
	class Listener
	{
		friend class Notifier<Class, Event>;

	protected:
		virtual void onEvent(const Class*, Event);
		Listener& operator=(const Listener&);
		virtual ~Listener();

	public:
		//Listener(const Listener&);
		Listener();
	};

	template<typename Class, typename Event>
	class Notifier
	{
	private:
		std::vector<Listener<Class, Event>*> listeners;
		mutable RaiseRange* raiseRange;

	protected:
		//Notifier(const Notifier&);
		Notifier()
			: listeners(),
			  raiseRange(NULL)
		{
		}
		Notifier& operator=(const Notifier&);

	public:
		void addListener(Listener<Class, Event>*) const;
		void removeListener(Listener<Class, Event>*) const;

	protected:
		bool hasListeners() const
		{
			return !listeners.empty();
		}
		// TODO: does not match
		void raise(Event event, Listener<Class, Event>* listener) const
		{
			listener->onEvent((Class*)this, event);
		}
		// TODO: does not match (this function is very weird)
		void raise(Event event) const
		{
			RaiseRange range;
			range.index = 0;
			range.upper = (int)listeners.size();

			RaiseRange* oldRange = this->raiseRange;
			this->raiseRange = &range;

			if (range.upper != 0)
			{
				while (range.index < range.upper)
				{
					Listener<Class, Event>* listener = listeners[range.index];
					raise(event, listener);
					++range.index;
				}
			}

			this->raiseRange = oldRange;
		}
		void raise() const;
		virtual void onAddListener(Listener<Class, Event>*) const;
		virtual void onRemoveListener(Listener<Class, Event>*) const;
	};
}
