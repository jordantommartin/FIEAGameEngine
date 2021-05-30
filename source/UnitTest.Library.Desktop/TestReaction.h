#pragma once

#include "Reaction.h"
#include "TypeManager.h"
#include "EventSubscribers.h"

namespace UnitTests
{
	class TestReaction final : public FIEAGameEngine::Reaction
	{
		RTTI_DECLARATIONS(TestReaction, Reaction)

	public:
		/// <summary>
		/// Default constructor. Uses pass through constructor for RTTI. Subscribes to event of FooSubscriber.
		/// </summary>
		TestReaction();
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new TestReaction and deep copies each member variable from the other TestReaction into this TestReaction. 
		/// </summary>
		/// <param name="other">The instance of TestReaction that is being copied from.</param>
		TestReaction(const TestReaction& other) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new TestReaction and "steals" the member variables from other TestReaction into this TestReaction. 
		/// Shallow copies the member variables and sets the other TestReaction's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of TestReaction that is being moved from.</param>
		TestReaction(TestReaction&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this TestReaction and deep copies each element from other TestReaction into this TestReaction.
		/// </summary>
		/// <param name="other">The instance of TestReaction that is being copied from.</param>
		/// <returns>A TestReaction reference to this object.</returns>
		TestReaction& operator=(const TestReaction & other) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other TestReaction into this TestReaction. 
		/// Clears the current TestReaction and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other TestReaction's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of TestReaction that is being moved from.</param>
		/// <returns>A TestReaction reference to the this object.</returns>
		TestReaction& operator=(TestReaction&&) = default;
		/// <summary>
		/// Default destructor.
		/// </summary>
		~TestReaction() = default;

		/// <summary>
		/// Takes in an EventPublisher address and processes the event.
		/// </summary>
		void Notify(const FIEAGameEngine::EventPublisher& e) override;
		/// <summary>
		/// Clones the object and creates a new heap allocated TestReaction.
		/// </summary>
		/// <returns></returns>
		gsl::owner<TestReaction*> Clone() const override;

		/// <summary>
		/// Test variable to check if notify occured.
		/// </summary>
		std::int32_t Count;
		/// <summary>
		/// Provides the Signatures Vector for an TestReaction with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an TestReaction.</returns>
		static const FIEAGameEngine::Vector<FIEAGameEngine::Signature> Signatures();
	};

	ConcreteFactory(TestReaction, FIEAGameEngine::Scope)
}