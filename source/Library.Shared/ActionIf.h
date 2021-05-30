#pragma once
#include "Action.h"
#include "ActionList.h"
#include "TypeManager.h"
#include "WorldState.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// An action that can be used to check a condition and perform another action based on that condition.
	/// </summary>
	class ActionIf final : public Action 
	{
	public:
		RTTI_DECLARATIONS(ActionIf, Action)

	public:
		/// <summary>
		/// Creates a new instance of ActionIf. 
		/// </summary>
		ActionIf();
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new ActionIf and deep copies each member variable from the other ActionIf into this ActionIf. 
		/// </summary>
		/// <param name="other">The instance of ActionIf that is being copied from.</param>
		ActionIf(const ActionIf&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new ActionIf and "steals" the member variables from other ActionIf into this ActionIf. 
		/// Shallow copies the member variables and sets the other ActionIf's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionIf that is being moved from.</param>
		ActionIf(ActionIf&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this ActionIf and deep copies each element from other ActionIf into this ActionIf.
		/// </summary>
		/// <param name="other">The instance of ActionIf that is being copied from.</param>
		/// <returns>A ActionIf reference to this object.</returns>
		ActionIf& operator=(const ActionIf&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other ActionIf into this ActionIf. 
		/// Clears the current ActionIf and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other ActionIf's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionIf that is being moved from.</param>
		/// <returns>A ActionIf reference to the this object.</returns>
		ActionIf& operator=(ActionIf&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this ActionList.
		/// </summary>
		~ActionIf() = default;

		/// <summary>
		/// Returns the bool condition this action is set to.
		/// </summary>
		const bool Condition() const;
		/// <summary>
		/// Sets the bool condition of this action.
		/// </summary>
		/// <param name="condition">The condition of this action.</param>
		void SetCondition(bool condition);
		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an ActionIf.
		/// </summary>
		/// <returns>An ActionIf pointer to the heap allocated memory.</returns>
		gsl::owner<ActionIf*> Clone() const override;
		/// <summary>
		/// Checks the condition. Based on that, calls update on either the "Then" or "Else" ActionLists.
		/// Will search for the condition if a target is set.
		/// </summary>
		/// <param name="worldState">The WorldState that is managed during each update call.</param>
		void Update(WorldState& worldState) override;
		/// <summary>
		/// Provides the Signatures Vector for an ActionIf with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an ActionIf.</returns>
		static const Vector<Signature> Signatures();

	private:
		/// <summary>
		/// The condition that this action is set to. This determines behavior in the update call.
		/// </summary>
		bool _condition;
		/// <summary>
		/// Index that will be used to get the actions of this entity.
		/// </summary>
		inline static const size_t thenIndex = 3_z;
		/// <summary>
		/// Index that will be used to get the actions of this entity.
		/// </summary>
		inline static const size_t elseIndex = 4_z;
		/// <summary>
		/// String that will be used to get the actions of this entity.
		/// </summary>
		inline static std::string ConditionStringLiteral = "Condition"s;
		/// <summary>
		/// String that will be used to get the name of this entity.
		/// </summary>
		inline static std::string ThenStringLiteral = "Then"s;
		/// <summary>
		/// String that will be used to get the name of this entity.
		/// </summary>
		inline static std::string ElseStringLiteral = "Else"s;

	};

	ConcreteFactory(ActionIf, Scope)
}