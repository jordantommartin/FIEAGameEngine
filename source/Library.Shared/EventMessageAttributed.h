#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// A type of payload that carries an attributed scope. For use with templated Event class.
	/// </summary>
	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);
	public:
		/// <summary>
		/// Default contructor.
		/// </summary>
		EventMessageAttributed() = default;
		/// <summary>
		/// Constructor used to set the member variables.
		/// </summary>
		/// <param name="subtype">The subtype to deliver to the ReactionAttributed.</param>
		/// <param name="worldState">The worldState to be delivered to the ReactionAttributed.</param>
		EventMessageAttributed(const string& subtype, WorldState& worldState);
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new EventMessageAttributed and deep copies each member variable from the other EventMessageAttributed into this EventMessageAttributed. 
		/// </summary>
		/// <param name="other">The instance of EventMessageAttributed that is being copied from.</param>
		EventMessageAttributed(const EventMessageAttributed&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new EventMessageAttributed and "steals" the member variables from other EventMessageAttributed into this EventMessageAttributed. 
		/// Shallow copies the member variables and sets the other EventMessageAttributed's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of EventMessageAttributed that is being moved from.</param>
		EventMessageAttributed(EventMessageAttributed&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this EventMessageAttributed and deep copies each element from other EventMessageAttributed into this EventMessageAttributed.
		/// </summary>
		/// <param name="other">The instance of EventMessageAttributed that is being copied from.</param>
		/// <returns>A EventMessageAttributed reference to this object.</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other EventMessageAttributed into this EventMessageAttributed. 
		/// Clears the current EventMessageAttributed and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other EventMessageAttributed's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of EventMessageAttributed that is being moved from.</param>
		/// <returns>A EventMessageAttributed reference to the this object.</returns>
		EventMessageAttributed& operator=(EventMessageAttributed&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this EventMessageAttributed.
		/// </summary>
		~EventMessageAttributed() = default;

		/// <summary>
		/// Returns the the contained "Subtype" string.
		/// </summary>
		/// <returns>The string reference that stores the subtype.</returns>
		const string& GetSubtype() const;
		/// <summary>
		/// Sets the contained "Subtype" string.
		/// </summary>
		/// <param name="subtype">The subtype to be passed to the ReactionAttributed.</param>
		void SetSubtype(const string& subtype);
		/// <summary>
		/// Returns the the contained world state.
		/// </summary>
		/// <returns>The world state for this game.</returns>
		const WorldState& GetWorldState() const;
		/// <summary>
		/// Sets the contained world state.
		/// </summary>
		/// <param name="worldState">The world state to set this payload to.</param>
		void SetWorldState(WorldState& worldState);

		/// <summary>
		/// Provides the Signatures Vector for an EventMessageAttributed with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an EventMessageAttributed.</returns>
		static const Vector<Signature> Signatures();

	protected:
		/// <summary>
		/// Reference to the world state.
		/// </summary>
		WorldState* _worldState;
		/// <summary>
		/// The name of this subtype.
		/// </summary>
		std::string _subtype;
		/// <summary>
		/// String that will be used to get the subtype.
		/// </summary>
		inline static std::string SubtypeStringLiteral = "Subtype";
	};
}