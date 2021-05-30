#pragma once

#include "Event.h"
#include "Foo.h"

namespace UnitTests
{
	struct FooSubscriber final : public FIEAGameEngine::EventSubscriber
	{
		/// <summary>
		/// Creates a FooSubcriber.
		/// </summary>
		FooSubscriber() = default;
		/// <summary>
		/// Creates a FooSubcriber. Sets the internal data member
		/// </summary>
		/// <param name="data">The internal data member.</param>
		FooSubscriber(int data);
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new FooSubscriber and deep copies each member variable from the other FooSubscriber into this FooSubscriber. 
		/// </summary>
		/// <param name="other">The instance of FooSubscriber that is being copied from.</param>
		FooSubscriber(const FooSubscriber&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new FooSubscriber and "steals" the member variables from other FooSubscriber into this FooSubscriber. 
		/// Shallow copies the member variables and sets the other FooSubscriber's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of FooSubscriber that is being moved from.</param>
		FooSubscriber(FooSubscriber&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this FooSubscriber and deep copies each element from other FooSubscriber into this FooSubscriber.
		/// </summary>
		/// <param name="other">The instance of FooSubscriber that is being copied from.</param>
		/// <returns>A FooSubscriber reference to this object.</returns>
		FooSubscriber& operator=(const FooSubscriber&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other FooSubscriber into this FooSubscriber. 
		/// Clears the current FooSubscriber and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other FooSubscriber's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of FooSubscriber that is being moved from.</param>
		/// <returns>A FooSubscriber reference to the this object.</returns>
		FooSubscriber& operator=(FooSubscriber&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this FooSubscriber.
		/// </summary>
		~FooSubscriber();

		/// <summary>
		/// Processes the delivered event.
		/// </summary>
		/// <param name="event"></param>
		void Notify(const FIEAGameEngine::EventPublisher& event) override;
		/// <summary>
		/// Boolean to determine if notify was called.
		/// </summary>
		bool WasNotified = false;
		/// <summary>
		/// Internal data member to check if changed from notify,
		/// </summary>
		int _data = 0;
	};
}