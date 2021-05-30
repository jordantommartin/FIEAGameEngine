#include "pch.h"
#include "ActionDestroyAction.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction)

	ActionDestroyAction::ActionDestroyAction() :
		Action(ActionDestroyAction::TypeIdClass())
	{

	}

	gsl::owner<ActionDestroyAction*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction(*this);
	}

	void ActionDestroyAction::Update(WorldState& worldState)
	{
		assert(_parent != nullptr);
		worldState.AddDestroyAction(WorldState::DestroyAction(_actionToDeleteName, *_parent));
	}

	const Vector<Signature> ActionDestroyAction::Signatures()
	{
		return Vector< FIEAGameEngine::Signature>
		{
			{ NameStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionDestroyAction, _name) },
			{ ActionToDeleteStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionDestroyAction, _actionToDeleteName) }
		};
	}
}