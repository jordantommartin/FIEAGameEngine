#include "pch.h"
#include "EventQueue.h"

namespace FIEAGameEngine
{
	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, std::chrono::milliseconds delay)
	{
		QueueEntry entry(std::move(event), gameTime.CurrentTime(), delay);
		_events.PushBack(entry);
	}

	void EventQueue::Clear()
	{
		_events.Clear();
	}

	bool EventQueue::IsEmpty()
	{
		return _events.IsEmpty();
	}

	size_t EventQueue::Size()
	{
		return _events.Size();
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		if (!_events.IsEmpty())
		{
			auto it = std::partition(_events.begin(), _events.end(), [&gameTime](QueueEntry entry) { return !entry.IsExpired(gameTime.CurrentTime()); });

			size_t numDeliveredEvents = 0_z;
			for (; it != _events.end(); ++it, ++numDeliveredEvents)
			{

				(*it)._event->Deliver();
			}
			_events.Resize(_events.Size() - numDeliveredEvents);
		}
	}

	bool EventQueue::QueueEntry::IsExpired(std::chrono::high_resolution_clock::time_point currentTime) const
	{
		return currentTime > _timeEnqueued + _delay;
	}

	EventQueue::QueueEntry::QueueEntry(std::shared_ptr<EventPublisher> event, std::chrono::high_resolution_clock::time_point timeEnqueued, std::chrono::milliseconds delay) :
		_event(event), _timeEnqueued(timeEnqueued), _delay(delay)
	{

	}
}

