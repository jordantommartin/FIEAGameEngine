#pragma once
#include "Attributed.h"
#include "Reaction.h"
#include "EventPublisher.h"
#include "Event.h"
#include "EventMessageAttributed.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Implements the Reaction abstract base class. Receives a notify from Event<EventMessageAttributed>.
	/// </summary>
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);
	public:
		/// <summary>
		/// Default constructor. Uses a pass through constructor and subscribes to Event<EventMessageAttributed>
		/// </summary>
		ReactionAttributed();
		/// <summary>
		/// Constructor that sets the member variables.
		/// </summary>
		/// <param name="name">The name of this Reaction attributed.</param>
		/// <param name="subType">The subtype of this Reaction attributed to be compared against the eventMessageAttributed.</param>
		ReactionAttributed(const string& name, const string& subtype);
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new ReactionAttributed and deep copies each member variable from the other ReactionAttributed into this ReactionAttributed. 
		/// </summary>
		/// <param name="other">The instance of ReactionAttributed that is being copied from.</param>
		ReactionAttributed(const ReactionAttributed& other) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new ReactionAttributed and "steals" the member variables from other ReactionAttributed into this ReactionAttributed. 
		/// Shallow copies the member variables and sets the other ReactionAttributed's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ReactionAttributed that is being moved from.</param>
		ReactionAttributed(ReactionAttributed&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this ReactionAttributed and deep copies each element from other ReactionAttributed into this ReactionAttributed.
		/// </summary>
		/// <param name="other">The instance of ReactionAttributed that is being copied from.</param>
		/// <returns>A ReactionAttributed reference to this object.</returns>
		ReactionAttributed& operator=(const ReactionAttributed& other) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other ReactionAttributed into this ReactionAttributed. 
		/// Clears the current ReactionAttributed and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other ReactionAttributed's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ReactionAttributed that is being moved from.</param>
		/// <returns>A ReactionAttributed reference to the this object.</returns>
		ReactionAttributed& operator=(ReactionAttributed&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this ReactionAttributed.
		/// </summary>
		~ReactionAttributed();


		/// <summary>
		/// Returns the the contained "Subtype" string.
		/// </summary>
		/// <returns>The string reference that stores the subtype.</returns>
		string& GetSubtype();

		/// <summary>
		/// Sets the the contained "Subtype" string.
		/// </summary>
		void SetSubtype(const string& subtype);

		/// <summary>
		/// Takes in an Event<EventMessageAttributed> and casts to that type.
		/// Gets the payload and compares the subtypes.
		/// If they match, copies all the auxiliary attibutes and calls update on the actionList.
		/// </summary>
		void Notify(const EventPublisher& event);
		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an ReactionAttributed.
		/// </summary>
		/// <returns>An ReactionAttributed pointer to the heap allocated memory.</returns>
		gsl::owner<ReactionAttributed*> Clone() const;

		/// <summary>
		/// Provides the Signatures Vector for an ReactionAttributed with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an ReactionAttributed.</returns>
		static const Vector<Signature> Signatures();

	protected:
		/// <summary>
		/// The name of this subtype.
		/// </summary>
		std::string _subtype;
		/// <summary>
		/// String that will be used to get the subtype.
		/// </summary>
		inline static std::string SubtypeStringLiteral = "Subtype";
	};

	ConcreteFactory(ReactionAttributed, Scope)
}