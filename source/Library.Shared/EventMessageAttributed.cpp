#include "pch.h"
#include "EventMessageAttributed.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	EventMessageAttributed::EventMessageAttributed(const string& subtype, WorldState& worldState) :
		Attributed(EventMessageAttributed::TypeIdClass()), _subtype(subtype), _worldState(&worldState)
	{

	}

	const string& EventMessageAttributed::GetSubtype() const
	{
		return _subtype;
	}

	void EventMessageAttributed::SetSubtype(const string& subtype)
	{
		_subtype = subtype;
	}

	const WorldState& EventMessageAttributed::GetWorldState() const
	{
		return *_worldState;
	}

	void EventMessageAttributed::SetWorldState(WorldState& worldState)
	{
		_worldState = &worldState;
	}

	const Vector<Signature> EventMessageAttributed::Signatures()
	{
		return Vector< FIEAGameEngine::Signature>
		{
			{SubtypeStringLiteral, Datum::DatumTypes::String, 1, offsetof(EventMessageAttributed, _subtype) }
		};
	}
}