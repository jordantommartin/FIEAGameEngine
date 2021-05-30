#include "pch.h"
#include "ActionEvent.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionEvent)

	ActionEvent::ActionEvent() :
		Action(ActionEvent::TypeIdClass())
	{

	}

	const string& ActionEvent::GetSubtype() const
	{
		return _subtype;
	}

	void ActionEvent::SetSubtype(const string& subtype)
	{
		_subtype = subtype;
	}

	int ActionEvent::GetDelay() const
	{
		return _delay;
	}

	void ActionEvent::SetDelay(int delay)
	{
		_delay = delay;
	}

	gsl::owner<ActionEvent*> ActionEvent::Clone() const
	{
		return new ActionEvent(*this);
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		EventMessageAttributed payload(_subtype, worldState);
		auto auxilaryAttributes = AuxiliaryAttributes();
		for (auto auxiliaryAttribute : auxilaryAttributes)
		{
			payload.AppendAuxiliaryAttribute(auxiliaryAttribute->first) = auxiliaryAttribute->second;
		}
		shared_ptr<Event<EventMessageAttributed>> actionEvent = make_shared<Event<EventMessageAttributed>>(payload);
		worldState._eventQueue->Enqueue(actionEvent, worldState.GetGameTime(), (std::chrono::milliseconds) _delay);
	}

	const Vector<Signature> ActionEvent::Signatures()
	{
		return Vector<Signature>
		{
			{ SubtypeStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionEvent, _subtype) },
			{ DelayStringLiteral, Datum::DatumTypes::Integer, 1, offsetof(ActionEvent, _delay) }
		};
	}
}