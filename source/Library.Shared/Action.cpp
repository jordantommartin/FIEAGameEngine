#include "pch.h"
#include "Action.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Action)

#pragma region Action

	Action::Action(RTTI::IdType type) :
		Attributed(type)
	{

	}

	const std::string& Action::Name() const
	{
		return _name;
	}

	void Action::SetName(const std::string& name)
	{
		_name = name;
	}

	const std::string& Action::Target() const
	{
		return _target;
	}

	void Action::SetTarget(const std::string& target)
	{
		_target = target;
	}

	const Vector<Signature> Action::Signatures()
	{
		return Vector< FIEAGameEngine::Signature>
		{
			{NameStringLiteral, Datum::DatumTypes::String, 1, offsetof(Action, _name) },
			{TargetStringLiteral, Datum::DatumTypes::String, 1, offsetof(Action, _target) },
		};
	}
#pragma endregion
}