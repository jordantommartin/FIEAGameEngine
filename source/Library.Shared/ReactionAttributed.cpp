#include "pch.h"
#include "ReactionAttributed.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed)

	ReactionAttributed::ReactionAttributed() :
		Reaction(ReactionAttributed::TypeIdClass())
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const string& name, const string& subType) :
		Reaction(ReactionAttributed::TypeIdClass()), _subtype(subType)
	{
		SetName(name);
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	string& ReactionAttributed::GetSubtype()
	{
		return _subtype;
	}

	void ReactionAttributed::SetSubtype(const string& subtype)
	{
		_subtype = subtype;
	}

	void ReactionAttributed::Notify(const EventPublisher& event)
	{
		assert(event.Is(Event<EventMessageAttributed>::TypeIdClass()));
		const Event<EventMessageAttributed>& reactionEvent = static_cast<const Event<EventMessageAttributed>&>(event);
		const EventMessageAttributed& payload = reactionEvent.Message();
		
		if (GetSubtype() == payload.GetSubtype())
		{
			auto payloadAuxilaryAttributes = payload.AuxiliaryAttributes();
			for (auto auxiliaryAttribute : payloadAuxilaryAttributes)
			{
				AppendAuxiliaryAttribute(auxiliaryAttribute->first) = auxiliaryAttribute->second;
			}
			ActionList::Update(const_cast<WorldState&>(payload.GetWorldState()));
		}
	}

	gsl::owner<ReactionAttributed*> ReactionAttributed::Clone() const
	{
		ReactionAttributed* clone = new ReactionAttributed(*this);
		Event<EventMessageAttributed>::Subscribe(*clone);
		return clone;
	}

	const Vector<Signature> ReactionAttributed::Signatures()
	{
		return Vector<Signature>
		{
			{ NameStringLiteral, Datum::DatumTypes::String, 1, offsetof(ReactionAttributed, _name) },
			{ ActionStringLiteral, Datum::DatumTypes::Table, 0, 0 },
			{ SubtypeStringLiteral, Datum::DatumTypes::String, 1, offsetof(ReactionAttributed, _subtype) }
		};
	}
}