#pragma once
#include "RTTI.h"
#include "Vector.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

using namespace std::string_literals;

namespace FIEAGameEngine
{
	/// <summary>
	/// A templated extension of an EventPublisher that represents an event of the templated type.
	/// The deliver method method of a subscriber of this event will receive the payload of this event when delivered.
	/// </summary>	
	/// <typeparam name="T">The kind of payload this event is storing.</typeparam>
	template <typename T>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher)
	public:
		/// <summary>
		/// Creates a new instance of an Event. 
		/// Uses a pass through constructor to populate the subscribers vector.
		/// <summary>
		explicit Event(const T& message);
		/// <summary>
		/// Creates a new instance of an Event. 
		/// Uses a pass through constructor to populate the subscribers vector.
		/// For use with an RValue reference.
		/// <summary>
		explicit Event(T&& message);
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new Event and deep copies each member variable from the other Event into this Event. 
		/// </summary>
		/// <param name="other">The instance of Event that is being copied from.</param>
		Event(const Event&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new Event and "steals" the member variables from other Event into this Event. 
		/// Shallow copies the member variables and sets the other Event's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of Event that is being moved from.</param>
		Event(Event&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this Event and deep copies each element from other Event into this Event.
		/// </summary>
		/// <param name="other">The instance of Event that is being copied from.</param>
		/// <returns>A Event reference to this object.</returns>
		Event& operator=(const Event&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Event into this Event. 
		/// Clears the current Event and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other Event's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of Event that is being moved from.</param>
		/// <returns>A Event reference to the this object.</returns>
		Event& operator=(Event&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this Event.
		/// </summary>
		~Event() = default;

		/// <summary>
		/// Given the address of an EventSubscriber, add it to the list of subscribers for this event type.
		/// </summary>
		/// <param name="subscriber">The event that will be added to the list of subscribers.</param>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Given the address of an EventSubscriber, remove it from the list of subscribers for this event type.
		/// </summary>
		/// <param name="subscriber">The event that will be removed from the list of subscribers.</param>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Unsubscribe all subscribers to this event type.
		/// </summary>
		static void UnsubscribeAll();

		static const Vector<EventSubscriber*>& Subscribers();

		/// <summary>
		/// Returns a const reference to the payload that cannot be mutated.
		/// </summary>
		/// <returns></returns>
		const T& Message() const;

		/// <summary>
		/// Overrides the RTTI To String method
		/// </summary>
		/// <returns>The data as a string representation.</returns>
		std::string ToString() const override;
	private:
		/// <summary>
		/// The list of subscribers to this event.
		/// </summary>
		static Vector<EventSubscriber*> _subscribers;

		/// <summary>
		/// The payload of this event.
		/// </summary>
		T _message;
	};
}

#include "Event.inl"