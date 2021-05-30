#pragma once
#include <CppUnitTest.h>
#include "Foo.h"
#include "Bar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Bar>(const Bar& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}
}