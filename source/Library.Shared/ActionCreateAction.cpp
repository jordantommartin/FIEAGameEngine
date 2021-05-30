#include "pch.h"
#include "ActionCreateAction.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)

	ActionCreateAction::ActionCreateAction() :
		Action(ActionCreateAction::TypeIdClass())
	{

	}

	gsl::owner<ActionCreateAction*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
		assert(_parent != nullptr);
		worldState.AddCreateAction(WorldState::CreateAction(_actionToCreateName, _className, _target, *_parent));
	}

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector< FIEAGameEngine::Signature>
		{
			{ NameStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionCreateAction, _name) },
			{ TargetStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionCreateAction, _target) },
			{ ActionToCreateStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionCreateAction, _actionToCreateName) },
			{ ClassNameStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionCreateAction, _className) }
		};
	}
}