#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "Attributed.h"
#include "AttributedFoo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	wstring ToString<AttributedFoo>(const AttributedFoo& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<AttributedFoo>(AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	wstring ToString<Attributed>(Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	wstring ToString<vec4>(const vec4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

	template<>
	wstring ToString<mat4>(const mat4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTests)
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

		TEST_METHOD(RTTITest)
		{
			AttributedFoo a;

			RTTI* rtti = &a;

			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), rtti->TypeIdInstance());

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsNotNull(r);

			AttributedFoo otherFoo;
			Assert::IsTrue(rtti->Equals(&otherFoo));

			Bar otherBar(10);
			Assert::IsFalse(rtti->Equals(&otherBar));

			Assert::AreEqual("AttributedFoo"s, r->ToString());
		}

		TEST_METHOD(Attributes)
		{
			AttributedFoo foo;
			
			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScope"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"));

			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScopeArray"));

			Assert::AreEqual(&foo, foo["this"].GetPointer()->As<AttributedFoo>());
		}

		TEST_METHOD(AuxiliaryAttributes)
		{
			AttributedFoo foo;

			foo.AppendAuxiliaryAttribute("AuxiliaryInteger");
			Assert::IsTrue(foo.IsAttribute("AuxiliaryInteger"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("AuxiliaryInteger"));
			Assert::IsFalse(foo.IsPrescribedAttribute("AuxiliaryInteger"));

			Datum d1 = foo.AppendAuxiliaryAttribute("AuxiliaryIntegerWithDatum");
			Assert::IsTrue(foo.IsAttribute("AuxiliaryIntegerWithDatum"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("AuxiliaryIntegerWithDatum"));
			Assert::IsFalse(foo.IsPrescribedAttribute("AuxiliaryIntegerWithDatum"));

			Assert::ExpectException<exception>([&foo] {foo.AppendAuxiliaryAttribute("ExternalInteger"); });
		}

		TEST_METHOD(ExternalAssignment)
		{
			AttributedFoo foo;

			Datum* datum = foo.Find("ExternalInteger");
			Assert::AreEqual(1_z, datum->Size());
			Assert::AreSame(datum->GetInteger(), foo.ExternalInteger);

			int a = 5;
			datum->Set(a);
			Assert::AreEqual(a, datum->GetInteger());
			Assert::AreEqual(a, foo.ExternalInteger);

			float b = 6.0f;
			foo["ExternalFloat"] = b;
			Assert::AreEqual(b, foo.ExternalFloat);
		}

		TEST_METHOD(CopyConstructor)
		{
			AttributedFoo foo;
			AttributedFoo anotherAttributedFoo = foo;

			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			
			Assert::AreEqual(&anotherAttributedFoo, anotherAttributedFoo["this"].GetPointer()->As<AttributedFoo>());

			// Test that external storage has been updated
			int& fooData = foo["ExternalInteger"].GetInteger();
			int& anotherFooData = anotherAttributedFoo["ExternalInteger"].GetInteger();
			Assert::AreNotSame(anotherFooData, fooData);
		}

		TEST_METHOD(CopyAssignment)
		{
			AttributedFoo foo;
			AttributedFoo anotherAttributedFoo;
			anotherAttributedFoo = foo;

			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrixArray"));

			Assert::AreEqual(&anotherAttributedFoo, anotherAttributedFoo["this"].GetPointer()->As<AttributedFoo>());

			// Test that external storage has been updated
			int& fooData = foo["ExternalInteger"].GetInteger();
			int& anotherFooData = anotherAttributedFoo["ExternalInteger"].GetInteger();
			Assert::AreNotSame(anotherFooData, fooData);
		}

		TEST_METHOD(AttributeAccessors)
		{
			const size_t prescribedAttributeCount = AttributedFoo::Signatures().Size() + 1; // +1 for this
			AttributedFoo foo;

			Assert::AreEqual(prescribedAttributeCount, foo.Attributes().Size());
			Assert::AreEqual(prescribedAttributeCount, foo.PrescribedAttributes().Size());
			Assert::AreEqual(0_z, foo.AuxiliaryAttributes().Size());

			foo.AppendAuxiliaryAttribute("AuxiliaryInteger");
			Assert::AreEqual(prescribedAttributeCount + 1, foo.Attributes().Size());
			Assert::AreEqual(prescribedAttributeCount, foo.PrescribedAttributes().Size());
			Assert::AreEqual(1_z, foo.AuxiliaryAttributes().Size());
		}

		TEST_METHOD(MoveConstructor)
		{
			AttributedFoo foo;

			size_t originalSize = foo.Size();
			Assert::AreNotEqual(0_z, originalSize);
			Assert::AreNotEqual(1_z, originalSize); // 1 for the "this" pointer (we should have more prescribed attributes)

			foo.ExternalInteger = 1;
			foo.ExternalFloat = 2.0f;
			foo.ExternalString = "Hello World";
			foo.ExternalVector = vec4(1, 2, 3, 4);
			foo.ExternalMatrix = mat4(-1);

			char helloWorld[14];
			strcpy_s(helloWorld, foo.ExternalString.c_str());
			string helloWorldString = helloWorld;
			Assert::AreEqual(helloWorldString, foo.ExternalString);

			int integerArray[] = { 1, 2, 3, 4, 5 };
			std::copy(std::begin(integerArray), std::end(integerArray), foo.ExternalIntegerArray);

			AttributedFoo anotherFoo = std::move(foo);
			Assert::AreEqual(originalSize, anotherFoo.Size());
			Assert::AreEqual(0_z, foo.Size());
			Assert::AreEqual(1, anotherFoo.ExternalInteger);
			Assert::AreEqual(2.0f, anotherFoo.ExternalFloat);
			Assert::AreEqual(helloWorldString, anotherFoo.ExternalString);
			Assert::AreNotSame(helloWorldString, anotherFoo.ExternalString);
			Assert::IsTrue(foo.ExternalString.empty());
			Assert::AreEqual(anotherFoo.ExternalVector, vec4(1, 2, 3, 4));
			Assert::AreEqual(anotherFoo.ExternalMatrix, mat4(-1));

			Assert::AreEqual(&anotherFoo, anotherFoo["this"].GetPointer()->As<AttributedFoo>());

		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			AttributedFoo foo;

			size_t originalSize = foo.Size();
			Assert::AreNotEqual(0_z, originalSize);
			Assert::AreNotEqual(1_z, originalSize); // 1 for the "this" pointer (we should have more prescribed attributes)

			foo.ExternalInteger = 1;
			foo.ExternalFloat = 2.0f;
			foo.ExternalString = "Hello World";
			foo.ExternalVector = vec4(1, 2, 3, 4);
			foo.ExternalMatrix = mat4(-1);

			char helloWorld[14];
			strcpy_s(helloWorld, foo.ExternalString.c_str());
			string helloWorldString = helloWorld;
			Assert::AreEqual(helloWorldString, foo.ExternalString);

			int integerArray[] = { 1, 2, 3, 4, 5 };
			std::copy(std::begin(integerArray), std::end(integerArray), foo.ExternalIntegerArray);

			AttributedFoo anotherFoo;
			anotherFoo = std::move(foo);
			Assert::AreEqual(originalSize, anotherFoo.Size());
			Assert::AreEqual(0_z, foo.Size());
			Assert::AreEqual(1, anotherFoo.ExternalInteger);
			Assert::AreEqual(2.0f, anotherFoo.ExternalFloat);
			Assert::AreEqual(helloWorldString, anotherFoo.ExternalString);
			Assert::AreNotSame(helloWorldString, anotherFoo.ExternalString);
			Assert::IsTrue(foo.ExternalString.empty());
			Assert::AreEqual(anotherFoo.ExternalVector, vec4(1, 2, 3, 4));
			Assert::AreEqual(anotherFoo.ExternalMatrix, mat4(-1));
			
			Assert::AreEqual(&anotherFoo, anotherFoo["this"].GetPointer()->As<AttributedFoo>());

		}

		TEST_METHOD(NestedScopeAttributes)
		{
			AttributedFoo foo;
			Datum& nestedScopeDatum = foo.At("NestedScope"s);
			Assert::AreEqual(nestedScopeDatum.Size(), 1_z);

			Datum& nestedScopeArrayDatum = foo.At("NestedScopeArray"s);
			Assert::AreEqual(AttributedFoo::ArraySize, nestedScopeArrayDatum.Size());

			const AttributedFoo constFoo;
			Datum& constNestedScopeDatum = foo.At("NestedScope"s);
			Assert::AreEqual(constNestedScopeDatum.Size(), 1_z);

			Datum& constNestedScopeArrayDatum = foo.At("NestedScopeArray"s);
			Assert::AreEqual(AttributedFoo::ArraySize, constNestedScopeArrayDatum.Size());
		}

		TEST_METHOD(Clone)
		{
			AttributedFoo foo;
			auto clone = foo.Clone();
			auto anotherAttributedFoo = clone->As<AttributedFoo>();
			Assert::IsNotNull(anotherAttributedFoo);

			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherAttributedFoo->IsPrescribedAttribute("ExternalMatrixArray"));

			Assert::AreEqual(anotherAttributedFoo, (*anotherAttributedFoo)["this"].GetPointer()->As<AttributedFoo>());

			// Test that external storage has been updated 
			int& fooData = foo["ExternalInteger"].GetInteger();
			int& anotherFooData = (*anotherAttributedFoo)["ExternalInteger"s].GetInteger();
			Assert::AreNotSame(anotherFooData, fooData);
			Assert::AreEqual(foo, *anotherAttributedFoo);

			delete clone;
		}
		

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState AttributedTests::sStartMemState;
}

