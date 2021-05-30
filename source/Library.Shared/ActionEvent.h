#pragma once
#include "Action.h"
#include "Vector.h"
#include "TypeManager.h"
#include "WorldState.h"
#include "EventMessageAttributed.h"
#include "Event.h"
#include "EventQueue.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Class used to create and enqueue an Event<EventMessageAttributed>.
	/// </summary>
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)

	public:
		/// <summary>
		/// Creates a new instance of ActionEvent. 
		/// </summary>
		ActionEvent();
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new ActionEvent and deep copies each member variable from the other ActionEvent into this ActionEvent. 
		/// </summary>
		/// <param name="other">The instance of ActionEvent that is being copied from.</param>
		ActionEvent(const ActionEvent&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new ActionEvent and "steals" the member variables from other ActionEvent into this ActionEvent. 
		/// Shallow copies the member variables and sets the other ActionEvent's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionEvent that is being moved from.</param>
		ActionEvent(ActionEvent&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this ActionEvent and deep copies each element from other ActionEvent into this ActionEvent.
		/// </summary>
		/// <param name="other">The instance of ActionEvent that is being copied from.</param>
		/// <returns>A ActionEvent reference to this object.</returns>
		ActionEvent& operator=(const ActionEvent&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other ActionEvent into this ActionEvent. 
		/// Clears the current ActionEvent and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other ActionEvent's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionEvent that is being moved from.</param>
		/// <returns>A ActionEvent reference to the this object.</returns>
		ActionEvent& operator=(ActionEvent&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this ActionEvent.
		/// </summary>
		~ActionEvent() = default;

		/// <summary>
		/// Returns the the contained "Subtype" string.
		/// </summary>
		/// <returns>The string reference that stores the subtype.</returns>
		const string& GetSubtype() const;

		/// <summary>
		/// Sets the contained "Subtype" string.
		/// </summary>
		/// <param name="subtype">The subtype to set the contained string.</param>
		void SetSubtype(const string& subtype);

		/// <summary>
		/// Returns the the contained "Delay" number.
		/// </summary>
		/// <returns>The string reference that stores the subtype.</returns>
		int GetDelay() const;

		/// <summary>
		/// Sets the contained "Delay" integer.
		/// </summary>
		/// <param name="delay">The delay to set the contained integer.</param>
		void SetDelay(int delay);

		/// <summary>
		/// Creates an Event<EventMessagedAttributed>, copies the auxiliary attributes into the event, and enqueues it into the event queue.
		/// </summary>
		/// <param name="worldState">The WorldState that is managed during each update call.</param>
		virtual void Update(WorldState& worldState);

		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an ActionEvent.
		/// </summary>
		/// <returns>An ActionEvent pointer to the heap allocated memory.</returns>
		gsl::owner<ActionEvent*> Clone() const override;
		/// <summary>
		/// Provides the Signatures Vector for an ActionEvent with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an ActionEvent.</returns>
		static const Vector<Signature> Signatures();

	protected:
		/// <summary>
		/// Thesubtype name, used to match to the reaction attributed.
		/// </summary>
		std::string _subtype = "";
		/// <summary>
		/// The delay of which to enqueue this event.
		/// </summary>
		int _delay = 0;
		/// <summary>
		/// String that will be used to get the subtype.
		/// </summary>
		inline static std::string SubtypeStringLiteral = "Subtype";
		/// <summary>
		/// String that will be used to get the delay.
		/// </summary>
		inline static std::string DelayStringLiteral = "Delay";
	};

	ConcreteFactory(ActionEvent, Scope)
}