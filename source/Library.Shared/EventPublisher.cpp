#include "pch.h"
#include "EventPublisher.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(const Vector<EventSubscriber*>& subscribers) :
		_subscribers(&subscribers)
	{

	}

	void EventPublisher::Deliver() const
	{
		for (EventSubscriber* subscriber : *_subscribers)
		{
			assert(subscriber != nullptr);
			subscriber->Notify(*this);
		}
	}
}

