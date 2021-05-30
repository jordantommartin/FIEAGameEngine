#include "Event.h"

namespace FIEAGameEngine
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template <typename T>
	Vector<EventSubscriber*> Event<T>::_subscribers;

	template<typename T>
	inline Event<T>::Event(const T& message) :
		EventPublisher(_subscribers), _message(message)
	{

	}

	template<typename T>
	inline Event<T>::Event(T&& message) :
		EventPublisher(&_subscribers), _message(std::forward(message))
	{

	}

	template<typename T>
	inline void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		if (_subscribers.Find(&subscriber) == _subscribers.end())
		{
			_subscribers.PushBack(&subscriber);
		}
	}

	template<typename T>
	inline void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		_subscribers.Remove(&subscriber);
	}

	template<typename T>
	inline const Vector<EventSubscriber*>& Event<T>::Subscribers() 
	{
		return _subscribers;
	}

	template<typename T>
	inline void Event<T>::UnsubscribeAll()
	{
		_subscribers.Clear();
		_subscribers.ShrinkToFit();
	}

	template<typename T>
	inline const T& Event<T>::Message() const
	{
		return _message;
	}

	template<typename T>
	std::string Event<T>::ToString() const
	{
		return "Event"s;
	}

}