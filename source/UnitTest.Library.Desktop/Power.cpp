#include "pch.h"
#include "Power.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Power)

	Power::Power() :
	Attributed(Power::TypeIdClass())
	{

	}

	const FIEAGameEngine::Vector<FIEAGameEngine::Signature> Power::Signatures()
	{
		return FIEAGameEngine::Vector<FIEAGameEngine::Signature>
		{
			{"Name"s, FIEAGameEngine::Datum::DatumTypes::String, 1, offsetof(Power, Name) },
			{ "Dps"s, FIEAGameEngine::Datum::DatumTypes::Float, 1, offsetof(Power, Dps) }
		};
	}

	gsl::owner<Power*> Power::Clone() const
	{
		return new Power(*this);
	}

	bool Power::Equals(const RTTI* rhs) const
	{
		const Power* other = rhs->As<Power>();
		return other != nullptr ? *this == *other : false;
	}

	std::string Power::ToString() const
	{
		return "Power"s;
	}

}