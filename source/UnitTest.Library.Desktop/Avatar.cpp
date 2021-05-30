#include "pch.h"
#include "Avatar.h"

using namespace FIEAGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTests
{
	RTTI_DEFINITIONS(Avatar)

	Avatar::Avatar() :
		Entity(Avatar::TypeIdClass())
	{

	}

	gsl::owner<Avatar*> Avatar::Clone() const
	{
		return new Avatar(*this);
	}

	const Vector<Signature> Avatar::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumTypes::String, 1, offsetof(Avatar, _name) },
			{ "Children"s, Datum::DatumTypes::Table, 0, 0 },
			{ "Actions"s, Datum::DatumTypes::Table, 0, 0 },
			{ "HitPoints"s, Datum::DatumTypes::Integer, 1, offsetof(Avatar, HitPoints) }
		};
	}
}