#include "pch.h"
#include "ActionIncrement.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement)

	ActionIncrement::ActionIncrement() :
		Action(ActionIncrement::TypeIdClass())
	{

	}

	const std::string& ActionIncrement::Target() const
	{
		return _target;
	}
	void ActionIncrement::SetTarget(const std::string& target)
	{
		_target = target;
	}
	int ActionIncrement::Step() const
	{
		return _step;
	}
	void ActionIncrement::SetStep(int step)
	{
		_step = step;
	}
	gsl::owner<ActionIncrement*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}
	void ActionIncrement::Update(WorldState& worldState)
	{
		worldState._currentAction = this;
		
		Scope* parent = GetParent();
		if (_target == "" && parent != nullptr && parent->Is(ReactionAttributed::TypeIdClass()))
		{
			Datum* datum = parent->Search(TargetStringLiteral);
			assert(datum != nullptr);
			_target = datum->GetString();

			datum = parent->Search(StepStringLiteral);
			assert(datum != nullptr);
			_step = datum->GetInteger();
		}

		Datum* foundTarget = Search(_target);
		
		if (foundTarget == nullptr)
		{
			throw std::runtime_error("Target was not found.");
		}

		foundTarget->Set(foundTarget->GetInteger() + _step);
	}
	const Vector<Signature> ActionIncrement::Signatures()
	{
		return Vector< FIEAGameEngine::Signature>
		{
			{NameStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionIncrement, _name) },
			{TargetStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionIncrement, _target) },
			{StepStringLiteral, Datum::DatumTypes::Integer, 1, offsetof(ActionIncrement, _step) }
		};
	}
}