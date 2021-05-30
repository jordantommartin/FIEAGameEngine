#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace FIEAGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FooTest)
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

		TEST_METHOD(TestRTTI)
		{
			Foo f;
			RTTI* rtti = &f;
			RTTI const* constRtti = &f;

			Assert::IsTrue(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is("Foo"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));

			Foo* castFoo = rtti->As<Foo>();
			Assert::IsNotNull(castFoo);
			Assert::IsNull(rtti->As<Bar>());

			Assert::IsTrue(constRtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(constRtti->Is("Foo"s));
			Assert::IsFalse(constRtti->Is(Bar::TypeIdClass()));

			const Foo* constCastFoo = constRtti->As<Foo>();
			Assert::IsNotNull(constCastFoo);
			Assert::IsNull(constRtti->As<Bar>());

			Assert::IsNotNull(rtti->QueryInterface(Foo::TypeIdClass()));
		}

		/// <summary>
		/// Creates a Foo using the constructor without and with providing an initial value. Check that the values of the Foo is correct.
		/// </summary>
		TEST_METHOD(TestConstructor)
		{
			Foo a;
			Assert::AreEqual(0, a.Data());

			const int value = 10;
			Foo b(value);
			Assert::AreEqual(value, b.Data());
		}

		/// <summary>
		/// Test both the copy constructor and the copy assignment. 
		/// Create a initial Foo a.
		/// Use the copy constructor to create b and test that they are equal.
		/// Create Foo c and use copy assignment to overwrite the data in c. Test that c and a are equal.
		/// </summary>
		TEST_METHOD(TestCopySemantics)
		{
			const int value = 10;
			Foo a(value);
			Assert::AreEqual(value, a.Data());

			Foo b(a);
			Assert::AreEqual(a, b);

			Foo c;
			c = a;
			Assert::AreEqual(a, c);
			Assert::IsFalse(a != c);
		}

		/// <summary>
		/// Test both the move constructor and the move assignment.
		/// Create an initial Foo a.
		/// Use the move constructor to create b and check that the value is retained. Try to access a and expect an error.
		/// Create Foo c and use the move assignment to overwrite the data in c. Test that the value is retained. Try to access b and expect an error.
		/// </summary>
		TEST_METHOD(TestMoveSemantics)
		{
			const int value = 10;
			Foo a(value);

			Foo b(move(a));
			Assert::AreEqual(value, b.Data());
			Assert::ExpectException<exception>([&a] { a.Data(); }, L"Expected an excpetion, but none was thrown");

			Foo c;
			Assert::AreEqual(0, c.Data());
			c = move(b);
			Assert::AreEqual(value, c.Data());
			Assert::ExpectException<exception>([&b] { b.Data(); }, L"Expected an excpetion, but none was thrown");
		}

		/// <summary>
		/// Test the equality operators for Foo.
		/// Test that two different Foos are not equal.
		/// Test that a copied Foo is equal to the original.
		/// Test that a moved Foo cannot use the equality operator.
		/// Test that a moved const Foo cannot be accessed.
		/// </summary>
		TEST_METHOD(TestComparison)
		{
			const int value = 10;
			Foo a(value);

			Foo b;
			Assert::IsTrue(a != b);

			b = a;
			Assert::AreEqual(a, b);

			b = move(a);
			Assert::ExpectException<exception>([&a, &b] { a == b; }, L"Expected an excpetion, but none was thrown");

			const int newValue = 5;
			int& data = b.Data();
			data = newValue;

			Foo c;
			c = move(b);
			Assert::AreEqual(newValue, c.Data());
			Assert::ExpectException<exception>([&b] { const_cast<const Foo&>(b).Data(); }, L"Expected an excpetion, but none was thrown");
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState FooTest::sStartMemState;
}
