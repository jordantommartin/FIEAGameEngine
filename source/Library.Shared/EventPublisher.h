#pragma once
#include "RTTI.h"
#include "Vector.h"
#include "EventSubscriber.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// An abstract class from which Event classes are to derive from.
	/// EventPublishers deliver a notification to any EventSubscribers, which will process the event.
	/// </summary>
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)
	public:
		/// <summary>
		/// Constructor is deleted because this is an abstract class.
		/// </summary>
		EventPublisher() = delete;
		/// <summary>
		/// Creates a new instance of an EventPublisher.
		/// Takes in a subscribers vector for the derived class of an Event.
		/// <summary>
		explicit EventPublisher(const Vector<EventSubscriber* >& subscribers);
		/// <summary>
		/// Invokes the move constructor. Creates a new EventPublisher and "steals" the member variables from other EventPublisher into this EventPublisher. 
		/// Shallow copies the member variables and sets the other EventPublisher's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of EventPublisher that is being moved from.</param>
		EventPublisher(EventPublisher&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this EventPublisher and deep copies each element from other EventPublisher into this EventPublisher.
		/// </summary>
		/// <param name="other">The instance of EventPublisher that is being copied from.</param>
		/// <returns>A EventPublisher reference to this object.</returns>
		EventPublisher& operator=(const EventPublisher&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other EventPublisher into this EventPublisher. 
		/// Clears the current EventPublisher and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other EventPublisher's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of EventPublisher that is being moved from.</param>
		/// <returns>A EventPublisher reference to the this object.</returns>
		EventPublisher& operator=(EventPublisher&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this EventPublisher.
		/// </summary>
		virtual ~EventPublisher() = default;

		/// <summary>
		/// Notifies all the subscribers of in the list of subscribers.
		/// </summary>
		void Deliver() const;


	protected:
		/// <summary>
		/// Non-static reference to list of pointers to EventSubscribers from the derived class.
		/// </summary>
		const Vector<EventSubscriber*>* _subscribers;
	};
}