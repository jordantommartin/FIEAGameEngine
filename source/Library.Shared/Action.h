#pragma once
#include "GameTime.h"
#include "Factory.h"
#include "Attributed.h"

namespace FIEAGameEngine
{
	class WorldState;
	/// <summary>
	/// An abstract class from which Action classes are to derive from.
	/// Action are statements we can execute through parsing Json grammar.
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);
	public:
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new Action and deep copies each member variable from the other Action into this Action. 
		/// </summary>
		/// <param name="other">The instance of Action that is being copied from.</param>
		Action(const Action&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new Action and "steals" the member variables from other Action into this Action. 
		/// Shallow copies the member variables and sets the other Action's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of Action that is being moved from.</param>
		Action(Action&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this Action and deep copies each element from other Action into this Action.
		/// </summary>
		/// <param name="other">The instance of Action that is being copied from.</param>
		/// <returns>A Action reference to this object.</returns>
		Action& operator=(const Action&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Action into this Action. 
		/// Clears the current Action and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other Action's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of Action that is being moved from.</param>
		/// <returns>A Action reference to the this object.</returns>
		Action& operator=(Action&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this Action.
		/// </summary>
		virtual ~Action() = default;

		/// <summary>
		/// Returns a string with the name of the Action.
		/// </summary>
		/// <returns>The name of this Action as a string.</returns>
		const std::string& Name() const;
		/// <summary>
		/// Sets the name of the Action.
		/// </summary>
		/// <param name="name">The passed in string that the name will be set to.</param>
		void SetName(const std::string& name);
		/// <summary>
		/// Returns a string with the name of the target. (The entity to be acted upon)
		/// </summary>
		/// <returns>The name of the target as a string.</returns>
		const std::string& Target() const;
		/// <summary>
		/// Sets the name of the target. (The entity to be acted upon)
		/// </summary>
		/// <param name="name">The passed in string that the target will be set to.</param>
		void SetTarget(const std::string& target);
		/// <summary>
		/// Updates this Action as well as all of the children actions.
		/// </summary>
		/// <param name="worldState">The WorldState that is managed durinng each update call.</param>
		virtual void Update(WorldState& worldState) = 0;
		/// <summary>
		/// Provides the Signatures Vector for an Action with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an Action.</returns>
		static const Vector<Signature> Signatures();

		///// <summary>
		///// An override on the clone function which provides a heap allocated object of an Action.
		///// </summary>
		///// <returns>An Action pointer to the heap allocated memory.</returns>
		virtual gsl::owner<Action*> Clone() const = 0;

		/// <summary>
		/// Constructor is deleted because this is an abstract class.
		/// </summary>
		Action() = delete;

	protected:

		/// <summary>
		/// Pass through contructor so that we can a call the Attributed constructor from a derived class of Action.
		/// </summary>
		/// <param name="type">The type of the derived class.</param>
		explicit Action(RTTI::IdType type);
		/// <summary>
		/// The name of this Action.
		/// </summary>
		std::string _name;
		/// <summary>
		/// The target that this Action will act upon.
		/// </summary>
		std::string _target;
		/// <summary>
		/// String that will be used to get an entity.
		/// </summary>
		inline static std::string NameStringLiteral = "Name";
		/// <summary>
		/// String that will be used to get the name of this entity.
		/// </summary>
		inline static std::string TargetStringLiteral = "Target";
	};
}