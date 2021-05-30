#pragma once
#include "ActionList.h"
#include "TypeManager.h"
#include "WorldState.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// An action that can be used to create a new action of a certain class and add it to the hierarchy.
	/// </summary>
	class ActionCreateAction final : public Action
	{
	public:
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:
		/// <summary>
		/// Creates a new instance of ActionCreateAction. 
		/// </summary>
		ActionCreateAction();
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new ActionCreateAction and deep copies each member variable from the other ActionCreateAction into this ActionCreateAction. 
		/// </summary>
		/// <param name="other">The instance of ActionCreateAction that is being copied from.</param>
		ActionCreateAction(const ActionCreateAction&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new ActionCreateAction and "steals" the member variables from other ActionCreateAction into this ActionCreateAction. 
		/// Shallow copies the member variables and sets the other ActionCreateAction's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionCreateAction that is being moved from.</param>
		ActionCreateAction(ActionCreateAction&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this ActionCreateAction and deep copies each element from other ActionCreateAction into this ActionCreateAction.
		/// </summary>
		/// <param name="other">The instance of ActionCreateAction that is being copied from.</param>
		/// <returns>A ActionCreateAction reference to this object.</returns>
		ActionCreateAction& operator=(const ActionCreateAction&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other ActionCreateAction into this ActionCreateAction. 
		/// Clears the current ActionCreateAction and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other ActionCreateAction's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionCreateAction that is being moved from.</param>
		/// <returns>A ActionCreateAction reference to the this object.</returns>
		ActionCreateAction& operator=(ActionCreateAction&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this ActionCreateAction.
		/// </summary>
		~ActionCreateAction() = default;
		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an ActionCreateAction.
		/// </summary>
		/// <returns>An ActionCreateAction pointer to the heap allocated memory.</returns>
		gsl::owner<ActionCreateAction*> Clone() const override;
		/// <summary>
		/// Registers the information required to create the action with the queue in the WorldState.
		/// </summary>
		/// <param name="worldState">The WorldState that is managed during each update call.</param>
		void Update(WorldState& worldState) override;
		/// <summary>
		/// Provides the Signatures Vector for an ActionCreateAction with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an ActionCreateAction.</returns>
		static const Vector<Signature> Signatures();

	private:
		/// <summary>
		/// The name of the action to create.
		/// </summary>
		string _actionToCreateName;
		/// <summary>
		/// The name of the class for the type that the new action will be.
		/// </summary>
		string _className;
		/// <summary>
		/// String that will be used to get the ActionToCreate of this action.
		/// </summary>
		inline static std::string ActionToCreateStringLiteral = "ActionToCreate"s;
		/// <summary>
		/// String that will be used to get the ClassName of this action.
		/// </summary>
		inline static std::string ClassNameStringLiteral = "ClassName"s;
		
	};

	ConcreteFactory(ActionCreateAction, Scope)
}