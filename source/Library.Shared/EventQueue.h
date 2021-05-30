#pragma once
#include "RTTI.h"
#include "Vector.h"
#include "GameTime.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

namespace FIEAGameEngine
{
	class EventQueue final
	{
		
	public:
		/// <summary>
		/// Creates a new instance of an EventQueue.
		/// <summary>
		EventQueue() = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new EventQueue and "steals" the member variables from other EventQueue into this EventQueue. 
		/// Shallow copies the member variables and sets the other EventQueue's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of EventQueue that is being moved from.</param>
		EventQueue(EventQueue&&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other EventQueue into this EventQueue. 
		/// Clears the current EventQueue and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other EventQueue's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of EventQueue that is being moved from.</param>
		/// <returns>A EventQueue reference to the this object.</returns>
		EventQueue& operator=(EventQueue&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this EventQueue.
		/// </summary>
		virtual ~EventQueue() = default;

		/// <summary>
		/// Given the address of an EventPublisher and an optional delay time, enqueue the event.
		/// </summary>
		/// <param name="event">The address of the EventPublisher we are creating an entry for.</param>
		/// <param name="delay">The delay of which this event should be delivered.</param>
		void Enqueue(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, std::chrono::milliseconds delay = 0ms);

		/// <summary>
		/// Given the a GameTime, publish any queued events that have expired.
		/// </summary>
		/// <param name="gameTime"></param>
		void Update(const GameTime& gameTime);

		/// <summary>
		/// Clears the event queue.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns whether the queue is empty or not.
		/// </summary>
		/// <returns>Whether the queue is empty or not.</returns>
		bool IsEmpty();

		/// <summary>
		/// Returns the number of events in the queue.
		/// </summary>
		/// <returns>The number of events in the queue.</returns>
		size_t Size();

	private:
		/// <summary>
		/// Struct used to encapsulate all of the information needed to determine if an event is expired.
		/// </summary>
		struct QueueEntry
		{
			/// <summary>
			/// Creates a default constructed queue entry.
			/// </summary>
			QueueEntry() = default;
			/// <summary>
			/// Creates a default constructed queue entry.
			/// Takes in a shared pointer to an event, the time it was enqueued, and the delay from which is should become expired.
			/// </summary>
			/// <param name="event">The event that is being enqueued.</param>
			/// <param name="timeEnqueued">The time the event is being enqueued.</param>
			/// <param name="delay">The delay from the time enqueued to become expired.</param>
			QueueEntry(std::shared_ptr<EventPublisher> event, std::chrono::high_resolution_clock::time_point timeEnqueued, std::chrono::milliseconds delay = 0ms);

			/// <summary>
			/// The event that is being enqueued.
			/// </summary>
			std::shared_ptr<EventPublisher> _event;
			/// <summary>
			/// The time the event is being enqueued.
			/// </summary>
			std::chrono::high_resolution_clock::time_point _timeEnqueued;
			/// <summary>
			/// The delay from the time enqueued to become expired.
			/// </summary>
			std::chrono::milliseconds _delay = 0ms;

			/// <summary>
			/// Takes in the current time and determines if the queue entry event is expired.
			/// </summary>
			/// <returns>Whether the queue entry event is expired.</returns>
			bool IsExpired(std::chrono::high_resolution_clock::time_point currentTime) const;
		};

		/// <summary>
		/// The queue of QueueEntries to be delivered.
		/// </summary>
		Vector<QueueEntry> _events;
	};
}