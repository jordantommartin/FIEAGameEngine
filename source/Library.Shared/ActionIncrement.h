#pragma once
#include "RTTI.h"
#include "Action.h"
#include "TypeManager.h"
#include "WorldState.h"
#include "ReactionAttributed.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// An action that can be used to target an integer in the hierarchy and add the step to it.
	/// </summary>
	class ActionIncrement final : public Action
	{
	public:
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:
		/// <summary>
		/// Creates a new instance of ActionIncrement. 
		/// </summary>
		ActionIncrement();
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new ActionIncrement and deep copies each member variable from the other ActionIncrement into this ActionIncrement. 
		/// </summary>
		/// <param name="other">The instance of ActionIncrement that is being copied from.</param>
		ActionIncrement(const ActionIncrement&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new ActionIncrement and "steals" the member variables from other ActionIncrement into this ActionIncrement. 
		/// Shallow copies the member variables and sets the other ActionIncrement's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionIncrement that is being moved from.</param>
		ActionIncrement(ActionIncrement&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this ActionIncrement and deep copies each element from other ActionIncrement into this ActionIncrement.
		/// </summary>
		/// <param name="other">The instance of ActionIncrement that is being copied from.</param>
		/// <returns>A ActionIncrement reference to this object.</returns>
		ActionIncrement& operator=(const ActionIncrement&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other ActionIncrement into this ActionIncrement. 
		/// Clears the current ActionIncrement and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other ActionIncrement's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionIncrement that is being moved from.</param>
		/// <returns>A ActionIncrement reference to the this object.</returns>
		ActionIncrement& operator=(ActionIncrement&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this ActionList.
		/// </summary>
		~ActionIncrement() = default;

		/// <summary>
		/// Returns the name of the target to find. (The integer that will be searched for in the hierarchy and added to)
		/// </summary>
		/// <returns>The name of the target to find.</returns>
		const std::string& Target() const;
		/// <summary>
		/// Sets the name of the target to find. (The integer that will be searched for in the hierarchy and added to)
		/// </summary>
		/// <param name="target">The new name of the target to find.</param>
		void SetTarget(const std::string& target);

		/// <summary>
		/// Returns the current step.
		/// </summary>
		/// <returns>The current step.</returns>
		int Step() const;
		/// <summary>
		/// Sets the current step.
		/// </summary>
		/// <param name="step">The new step to set.</param>
		void SetStep(int step);

		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an ActionIncrement.
		/// </summary>
		/// <returns>An ActionIncrement pointer to the heap allocated memory.</returns>
		gsl::owner<ActionIncrement*> Clone() const override;

		/// <summary>
		/// Performs the increment on the target.
		/// </summary>
		/// <param name="worldState">The WorldState that is managed during each update call.</param>
		void Update(WorldState& worldState) override;
		/// <summary>
		/// Provides the Signatures Vector for an ActionIncrement with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an ActionIncrement.</returns>
		static const Vector<Signature> Signatures();

	private:
		/// <summary>
		/// The amount to add to the target.
		/// </summary>
		int _step = 1;
		/// <summary>
		/// String that will be used to get the step of this action.
		/// </summary>
		inline static std::string StepStringLiteral = "Step"s;
	};

	ConcreteFactory(ActionIncrement, Scope)
}