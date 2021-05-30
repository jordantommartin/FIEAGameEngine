#pragma once

namespace FIEAGameEngine
{
	class EventPublisher;
	/// <summary>
	/// An abstract class from which an EventSubscriber will derive from.
	/// EventSubscribers receive a notification from EventPublishers and will then process the event.
	/// </summary>
	class EventSubscriber
	{
	public:
		/// <summary>
		/// Default constructor provided for derived classes.
		/// </summary>
		EventSubscriber() = default;
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new EventSubscriber and deep copies each member variable from the other EventSubscriber into this EventSubscriber. 
		/// </summary>
		/// <param name="other">The instance of EventSubscriber that is being copied from.</param>
		EventSubscriber(const EventSubscriber&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new EventSubscriber and "steals" the member variables from other EventSubscriber into this EventSubscriber. 
		/// Shallow copies the member variables and sets the other EventSubscriber's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of EventSubscriber that is being moved from.</param>
		EventSubscriber(EventSubscriber&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this EventSubscriber and deep copies each element from other EventSubscriber into this EventSubscriber.
		/// </summary>
		/// <param name="other">The instance of EventSubscriber that is being copied from.</param>
		/// <returns>A EventSubscriber reference to this object.</returns>
		EventSubscriber& operator=(const EventSubscriber&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other EventSubscriber into this EventSubscriber. 
		/// Clears the current EventSubscriber and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other EventSubscriber's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of EventSubscriber that is being moved from.</param>
		/// <returns>A EventSubscriber reference to the this object.</returns>
		EventSubscriber& operator=(EventSubscriber&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this EventSubscriber.
		/// </summary>
		virtual ~EventSubscriber() = default;

		/// <summary>
		/// Takes in an EventPublisher address and processes the event.
		/// Pure virtual method that will be implemented in derived class.
		/// </summary>
		virtual void Notify(const EventPublisher& event) = 0;
	};
}