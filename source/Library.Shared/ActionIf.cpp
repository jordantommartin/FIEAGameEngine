#include "pch.h"
#include "ActionIf.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionIf)

	ActionIf::ActionIf() :
		Action(ActionIf::TypeIdClass())
	{

	}

	const bool ActionIf::Condition() const
	{
		return _condition;
	}

	void ActionIf::SetCondition(bool condition)
	{
		_condition = condition;
	}

	gsl::owner<ActionIf*> ActionIf::Clone() const 
	{
		return new ActionIf(*this);
	}

	void ActionIf::Update(WorldState& worldState)
	{
		worldState._currentAction = this;

		if (!_target.empty())
		{
			Datum* foundTarget = Search(_target);
			assert(foundTarget != nullptr);
			assert(foundTarget->Type() == Datum::DatumTypes::Integer);
			_condition = foundTarget->GetInteger();
		}

		size_t actionListIndex = (_condition) ? thenIndex : elseIndex;

		Datum& datum = _order[actionListIndex]->second;
		Scope* scope = &datum.GetScope();
		assert(scope != nullptr);
		assert(scope->Is(ActionList::TypeIdClass()));

		ActionList& actionList = static_cast<ActionList&>(*scope);
		actionList.Update(worldState);
	}
	const Vector<Signature> ActionIf::Signatures()
	{
		return Vector< FIEAGameEngine::Signature>
		{
			{NameStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionIf, _name) },
			{ConditionStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionIf, _target) },
			{ThenStringLiteral, Datum::DatumTypes::Table, 0, 0 },
			{ElseStringLiteral, Datum::DatumTypes::Table, 0, 0 }
		};
	}
}