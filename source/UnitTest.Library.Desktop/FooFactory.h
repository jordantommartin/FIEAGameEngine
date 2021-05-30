#pragma once
#include "Factory.h"
#include "Foo.h"
#include "Bar.h"

namespace UnitTests
{
	using namespace std::string_literals;

	class FooFactory final : public FIEAGameEngine::Factory<FIEAGameEngine::RTTI>
	{
	public:
		FooFactory()
		{
			Add(*this);
		}
		~FooFactory()
		{
			Remove(*this);
		}

		inline const std::string& ClassName() const override
		{
			return _className;
		}

		inline gsl::owner<FIEAGameEngine::RTTI*> Create() const override
		{
			return new Foo();
		}
	private:
		inline static const std::string _className{ "Foo"s };
	};
}