#include "pch.h"
#include "TestReaction.h"

using namespace std;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace UnitTests
{
	RTTI_DEFINITIONS(TestReaction)

	TestReaction::TestReaction() :
		Reaction(TestReaction::TypeIdClass())
	{
		Event<FooSubscriber>::Subscribe(*this);
	}

	const Vector<Signature> TestReaction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumTypes::String, 1, offsetof(TestReaction, _name) },
			{ "Count"s, Datum::DatumTypes::Integer, 1, offsetof(TestReaction, Count) }
		};
	}

	void TestReaction::Notify(const EventPublisher& /* e */)
	{
		++Count;
	}
	
	gsl::owner<TestReaction*> TestReaction::Clone() const
	{
		return new TestReaction(*this);
	}
}