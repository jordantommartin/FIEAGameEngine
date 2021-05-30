#include "pch.h"
#include "EventSubscribers.h"

namespace UnitTests 
{
	FooSubscriber::FooSubscriber(int data) :
		_data(data)
	{
		FIEAGameEngine::Event<Foo>::Subscribe(*this);
	}

	FooSubscriber::~FooSubscriber()
	{
		FIEAGameEngine::Event<Foo>::Unsubscribe(*this);
	}

	void FooSubscriber::Notify(const FIEAGameEngine::EventPublisher& event)
	{
		assert(event.Is(FIEAGameEngine::Event<Foo>::TypeIdClass()));
		const FIEAGameEngine::Event<Foo>& fooEvent = static_cast<const FIEAGameEngine::Event<Foo>&>(event);

		const Foo& message = fooEvent.Message();
		_data = message.Data();

		WasNotified = true;
	}
}
