#include "pch.h"
#include "AttributedFoo.h"

using namespace FIEAGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo)

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{

	}

	const FIEAGameEngine::Vector< FIEAGameEngine::Signature> AttributedFoo::Signatures()
	{
		return Vector< FIEAGameEngine::Signature>
		{
			{"ExternalInteger"s, Datum::DatumTypes::Integer, 1, offsetof(AttributedFoo, ExternalInteger) },
			{ "ExternalFloat"s, Datum::DatumTypes::Float, 1, offsetof(AttributedFoo, ExternalFloat) },
			{ "ExternalString"s, Datum::DatumTypes::String, 1, offsetof(AttributedFoo, ExternalString) },
			{ "ExternalVector"s, Datum::DatumTypes::Vector, 1, offsetof(AttributedFoo, ExternalVector) },
			{ "ExternalMatrix"s, Datum::DatumTypes::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray"s, Datum::DatumTypes::Integer, ArraySize, offsetof(AttributedFoo, ExternalIntegerArray) },
			{ "ExternalFloatArray"s, Datum::DatumTypes::Float, ArraySize, offsetof(AttributedFoo, ExternalFloatArray) },
			{ "ExternalStringArray"s, Datum::DatumTypes::String, ArraySize, offsetof(AttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray"s, Datum::DatumTypes::Vector, ArraySize, offsetof(AttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray"s, Datum::DatumTypes::Matrix, ArraySize, offsetof(AttributedFoo, ExternalMatrixArray) },
			{ "NestedScope"s, Datum::DatumTypes::Table, 1, 0 },
			{ "NestedScopeArray"s, Datum::DatumTypes::Table, ArraySize, 0 },
		};
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		const AttributedFoo* other = rhs->As<AttributedFoo>();
		return other != nullptr ? *this == *other : false;
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo"s;
	}
	
}