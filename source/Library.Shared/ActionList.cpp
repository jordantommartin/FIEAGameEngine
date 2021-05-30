#include "pch.h"
#include "ActionList.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList() :
		Action(ActionList::TypeIdClass())
	{

	}

	ActionList::ActionList(RTTI::IdType type) :
		Action(type)
	{

	}

	ActionList::ActionList(std::string& name) :
		Action(ActionList::TypeIdClass())
	{
		SetName(name);
	}

	Action& ActionList::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* scope = Factory<Scope>::Create(className);
		assert(scope != nullptr);
		assert(scope->Is(Action::TypeIdClass()));

		Action& action = static_cast<Action&>(*scope);
		action.SetName(instanceName);
		Adopt(action, ActionStringLiteral);
		return action;
	}

	Datum& ActionList::Actions()
	{
		assert((_order.Size() - 1) >= actionsIndex && _order[actionsIndex]->second.Type() == Datum::DatumTypes::Table);
		return _order[actionsIndex]->second;
	}

	const Datum& ActionList::Actions() const
	{
		assert((_order.Size() - 1) >= actionsIndex && _order[actionsIndex]->second.Type() == Datum::DatumTypes::Table);
		return _order[actionsIndex]->second;
	}

	gsl::owner<ActionList*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	void ActionList::Update(WorldState& worldState)
	{
		worldState._currentAction = this;

		for (size_t i = 0; i < Actions().Size(); ++i)
		{
			Scope& action = Actions().GetScope(i);
			assert(action.Is(Action::TypeIdClass()));
			static_cast<Action&>(action).Update(worldState);
		}
	}

	const Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{ NameStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionList, _name) },
			{ ActionStringLiteral, Datum::DatumTypes::Table, 0, 0 }
		};
	}
}