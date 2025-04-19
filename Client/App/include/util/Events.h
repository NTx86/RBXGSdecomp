#pragma once
#include <vector>

namespace RBX
{
	struct RaiseRange
	{
	public:
		int index;
		int upper;
		RaiseRange* previous;
	public:
		void removeIndex(int);
	};

	template<typename T, typename U>
	class Listener
	{
	protected:
		virtual void onEvent(const T*, U);
		Listener& operator=(const Listener&);
		virtual ~Listener();

	public:
		//Listener(const Listener&);
		Listener();
	};

	template<typename T, typename U>
	class Notifier
	{
	private:
		std::vector<Listener<T,U>*> listeners;
		RaiseRange* raiseRange;

	protected:
		Notifier(const Notifier&);
		Notifier();
		Notifier& operator=(const Notifier&);

	public:
		void addListener(Listener<T,U>*) const;
		void removeListener(Listener<T,U>*) const;

	protected:
		bool hasListeners() const;
		void raise(U, Listener<T,U>*) const;
		void raise(U) const;
		void raise() const;
		virtual void onAddListener(Listener<T,U>*) const;
		virtual void onRemoveListener(Listener<T,U>*) const;
	};
}
