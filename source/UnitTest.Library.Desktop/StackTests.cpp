#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "Stack.h"
#include "Foo.h"
#include "Bar.h"
#include "SizeLiteral.h"

using namespace FIEAGameEngine;
using namespace UnitTests;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (...)
		{
			return L"end()";
		}
	}
}
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (...)
		{
			return L"end()";
		}
	}
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<SList<Bar>::Iterator>(const SList<Bar>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (...)
		{
			return L"end()";
		}
	}
}
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<SList<Bar>::ConstIterator>(const SList<Bar>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (...)
		{
			return L"end()";
		}
	}
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<SList<const char*>::Iterator>(const SList<const char*>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (...)
		{
			return L"end()";
		}
	}
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<SList<const char*>::ConstIterator>(const SList<const char*>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (...)
		{
			return L"end()";
		}
	}
}

namespace FIEAGameEngine
{
	template<>
	struct DefaultEquality<Bar>
	{
		bool operator()(const Bar& lhs, const Bar& rhs) const
		{
			return lhs.Data() == rhs.Data();
		}
	};
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(StackTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		/// /// <summary>
		/// Create a stack using default constructor. Test that you can add an element to it and that the size goes up.
		/// </summary>
		TEST_METHOD(TestConstructor)
		{
			{
				Stack<Foo> stack;
				const Foo f;
				stack.Push(f);
				Assert::AreEqual(1_z, stack.Size());
			}
		}

		/// <summary>
		/// Tests all the functions of the Stack object.
		/// </summary>
		TEST_METHOD(TestStackFunctions)
		{
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				Stack<Foo> stack;
				stack.Push(a);
				stack.Push(b);
				stack.Push(c);
				Assert::AreEqual(3_z, stack.Size());

				Assert::AreEqual(3_z, stack.Size());
				Assert::AreEqual(c, stack.Top());

				stack.Pop();
				Assert::AreEqual(2_z, stack.Size());
				Assert::AreEqual(b, stack.Top());

				stack.Pop();
				Assert::AreEqual(1_z, stack.Size());
				Assert::AreEqual(a, stack.Top());

				stack.Pop();
				Assert::IsTrue(stack.IsEmpty());

				stack.Push(Foo(10));
				Assert::AreEqual(1_z, stack.Size());

				stack.Clear();
				Assert::AreEqual(0_z, stack.Size());
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState StackTests::sStartMemState;
}