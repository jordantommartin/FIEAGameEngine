#pragma once
#include "ActionList.h"
#include "EventSubscriber.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Abstract base class for Reactions to derive from, which will act as function calls in response to an event becoming expired.
	/// </summary>
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)

	public:
		/// <summary>
		/// Destructor marked virtual because this class is being derived from.
		/// </summary>
		virtual ~Reaction() = default;

		/// <summary>
		/// Takes in an EventPublisher address and processes the event.
		/// Pure virtual method that will be implemented in derived class.
		/// </summary>
		void Notify(const EventPublisher& event) = 0;

		/// <summary>
		/// Update function written in the case that a user accidentally adds a Reaction into a Json deserialization. This would be called every frame, so leave empty.
		/// </summary>
		inline void Update(WorldState& worldState) override { UNREFERENCED_LOCAL(worldState); /* This method has been left empty intentionally.*/ };

	protected:
		/// <summary>
		/// Pass through contructor so that we can a call the Attributed constructor from a derived class of Reaction.
		/// </summary>
		/// <param name="type">The type of the derived class.</param>
		explicit Reaction(RTTI::IdType type);
	};
}