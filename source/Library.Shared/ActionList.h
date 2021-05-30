#pragma once

#include "Action.h"
#include "Vector.h"
#include "TypeManager.h"
#include "WorldState.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Class used to hold a list of other actions. Can be thought of like a block of code.
	/// </summary>
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		/// <summary>
		/// Creates a new instance of ActionList. 
		/// </summary>
		ActionList();
		/// <summary>
		/// Constructor that allows you to set the name on instantiation.
		/// </summary>
		/// <param name="name">The name of this ActionList.</param>
		ActionList(std::string& name);
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new ActionList and deep copies each member variable from the other ActionList into this ActionList. 
		/// </summary>
		/// <param name="other">The instance of ActionList that is being copied from.</param>
		ActionList(const ActionList&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new ActionList and "steals" the member variables from other ActionList into this ActionList. 
		/// Shallow copies the member variables and sets the other ActionList's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionList that is being moved from.</param>
		ActionList(ActionList&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this ActionList and deep copies each element from other ActionList into this ActionList.
		/// </summary>
		/// <param name="other">The instance of ActionList that is being copied from.</param>
		/// <returns>A ActionList reference to this object.</returns>
		ActionList& operator=(const ActionList&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other ActionList into this ActionList. 
		/// Clears the current ActionList and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other ActionList's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionList that is being moved from.</param>
		/// <returns>A ActionList reference to the this object.</returns>
		ActionList& operator=(ActionList&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this ActionList.
		/// </summary>
		~ActionList() = default;

		/// <summary>
		/// Uses the action factory to make a new object of the given type, adopts the entity into the actions datum, and returns the address of the new action.
		/// </summary>
		/// <param name="className">The name of the derived class to instantiate. (Action if not derived) </param>
		/// <param name="instanceName">The name to give this instantiation of the object.</param>
		/// <returns>A reference to the action that was created.</returns>
		Action& CreateAction(const std::string& className, const std::string& instanceName);

		///// <summary>
		///// Returns the the contained "actions" datum.
		///// </summary>
		///// <returns>The Datum reference that stores the actions.</returns>
		Datum& Actions();

		///// <summary>
		///// Returns the the contained "actions" datum.
		///// </summary>
		///// <returns>The Datum reference that stores the actions.</returns>
		const Datum& Actions() const;

		/// <summary>
		/// Updates each of the of the actions.
		/// </summary>
		/// <param name="worldState">The WorldState that is managed during each update call.</param>
		virtual void Update(WorldState& worldState);
		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an ActionList.
		/// </summary>
		/// <returns>An Entity pointer to the heap allocated memory.</returns>
		gsl::owner<ActionList*> Clone() const override;
		/// <summary>
		/// Provides the Signatures Vector for an ActionList with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an ActionList.</returns>
		static const Vector<Signature> Signatures();

	protected:
		/// <summary>
		/// Pass through constructor to make sure RTTI type is passed through to parent class.
		/// </summary>
		/// <param name="type"></param>
		explicit ActionList(RTTI::IdType type);
		/// <summary>
		/// Index that will be used to get the actions of this actionList.
		/// </summary>
		inline static const size_t actionsIndex = 2;
		/// <summary>
		/// String that will be used to get the actions of this actionList.
		/// </summary>
		inline static std::string ActionStringLiteral = "Actions"s;
	};

	ConcreteFactory(ActionList, Scope)
}