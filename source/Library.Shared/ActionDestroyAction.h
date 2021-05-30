#pragma once
#include "ActionList.h"
#include "TypeManager.h"
#include "WorldState.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// An action that can be used to destroy another action that already exists in the hierarchy.
	/// </summary>
	class ActionDestroyAction final : public Action
	{
	public:
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:
		/// <summary>
		/// Creates a new instance of ActionDestroyAction. 
		/// </summary>
		ActionDestroyAction();
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new ActionDestroyAction and deep copies each member variable from the other ActionDestroyAction into this ActionDestroyAction. 
		/// </summary>
		/// <param name="other">The instance of ActionDestroyAction that is being copied from.</param>
		ActionDestroyAction(const ActionDestroyAction&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new ActionDestroyAction and "steals" the member variables from other ActionDestroyAction into this ActionDestroyAction. 
		/// Shallow copies the member variables and sets the other ActionDestroyAction's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionDestroyAction that is being moved from.</param>
		ActionDestroyAction(ActionDestroyAction&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this ActionDestroyAction and deep copies each element from other ActionDestroyAction into this ActionDestroyAction.
		/// </summary>
		/// <param name="other">The instance of ActionDestroyAction that is being copied from.</param>
		/// <returns>A ActionDestroyAction reference to this object.</returns>
		ActionDestroyAction& operator=(const ActionDestroyAction&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other ActionDestroyAction into this ActionDestroyAction. 
		/// Clears the current ActionDestroyAction and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other ActionDestroyAction's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionDestroyAction that is being moved from.</param>
		/// <returns>A ActionDestroyAction reference to the this object.</returns>
		ActionDestroyAction& operator=(ActionDestroyAction&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this ActionList.
		/// </summary>
		~ActionDestroyAction() = default;

		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an ActionDestroyAction.
		/// </summary>
		/// <returns>An ActionDestroyAction pointer to the heap allocated memory.</returns>
		gsl::owner<ActionDestroyAction*> Clone() const override;

		/// <summary>
		/// Registers the information required to destroy the action with the queue in the WorldState.
		/// </summary>
		/// <param name="worldState">The WorldState that is managed during each update call.</param>
		void Update(WorldState& worldState) override;
		/// <summary>
		/// Provides the Signatures Vector for an ActionDestroyAction with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an ActionDestroyAction.</returns>
		static const Vector<Signature> Signatures();

	private:
		/// <summary>
		/// The name of the action that is to be deleted.
		/// </summary>
		string _actionToDeleteName;
		/// <summary>
		/// String that will be used to get the ActionToDelete of this action.
		/// </summary>
		inline static std::string ActionToDeleteStringLiteral = "ActionToDelete"s;
	};

	ConcreteFactory(ActionDestroyAction, Scope)
}