#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Scope.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace glm;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	
	template<>
	std::wstring ToString<Scope>(Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<Scope>(const Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<Scope>(const Scope& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	std::wstring ToString<Datum::DatumTypes>(const Datum::DatumTypes& t)
	{
		RETURN_WIDE_STRING(to_wstring(static_cast<int>(t)));
	}

	template<>
	std::wstring ToString<Datum>(const Datum& t)
	{
		return ToString(t.Type());
	}

	template<>
	std::wstring ToString<Datum>(Datum* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		try
		{
			return ToString(glm::to_string(t));
		}
		catch (...)
		{
			return L"end()";
		}
	}
	
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTests)
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
			Scope scope;

			RTTI* rtti = &scope;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Scope::TypeIdClass(), rtti->TypeIdInstance());

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			Scope* s = rtti->As<Scope>();
			Assert::IsNotNull(s);
			Assert::AreEqual(&scope, s);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(Scope::TypeIdClass());
			Assert::IsNotNull(r);

			Scope otherScope;
			Assert::IsTrue(rtti->Equals(&otherScope));

			Foo foo(10);
			Assert::IsFalse(rtti->Equals(&foo));

			Datum& aDatum = scope.Append("A"s);
			aDatum = "Hello, World"s;

			Assert::AreEqual("1"s, scope.ToString());
		}

		TEST_METHOD(Constructor)
		{
			Scope scope;
			Assert::AreEqual(0_z, scope.Size());
			Assert::IsNull(scope.GetParent());
		}

		TEST_METHOD(Comparison)
		{


			Scope* child = new Scope;
			Assert::IsNull(child->GetParent());

			Scope scope;
			scope.Adopt(*child, "A"s);
			Assert::AreEqual(&scope, child->GetParent());

			Scope* grandchildScope = new Scope;
			child->Adopt(*grandchildScope, "B"s);
			Assert::AreEqual(child, grandchildScope->GetParent());

			Scope copyScope(scope);
			Assert::IsTrue(scope == copyScope);
			Assert::IsTrue(scope != *child);
		}

		TEST_METHOD(Append)
		{
			Scope scope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;

			Datum& aDatum = scope.Append(a);
			Assert::AreEqual(1_z, scope.Size());
			Assert::AreEqual(&aDatum, scope.Find(a));
			Assert::AreEqual(aDatum, scope[0]);
			Assert::AreEqual(aDatum, scope[a]);

			Datum& bDatum = scope.Append(b);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(&bDatum, scope.Find(b));
			Assert::AreEqual(bDatum, scope[1]);
			Assert::AreEqual(bDatum, scope[b]);

			Datum& cDatum = scope.Append(c);
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(&cDatum, scope.Find(c));
			Assert::AreEqual(cDatum, scope[2]);
			Assert::AreEqual(cDatum, scope[c]);

			Assert::ExpectException<invalid_argument>([&scope] {scope.Append(""); });
		}

		TEST_METHOD(AppendScope)
		{
			Scope scope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;

			Scope& aScope = scope.AppendScope(a);
			Assert::AreEqual(1_z, scope.Size());
			Assert::AreEqual(&scope, aScope.GetParent());

			Datum* aScopeDatum = scope.Find(a);
			Assert::IsNotNull(aScopeDatum);
			Assert::AreEqual(Datum::DatumTypes::Table, aScopeDatum->Type());
			Assert::AreSame(aScope, aScopeDatum->GetScope(0));
			Assert::AreEqual(aScopeDatum, &(scope[0]));
			
			Scope& bScope = scope.AppendScope(b);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(&scope, bScope.GetParent());

			Datum* bScopeDatum = scope.Find(b);
			Assert::IsNotNull(bScopeDatum);
			Assert::AreEqual(Datum::DatumTypes::Table, bScopeDatum->Type());
			Assert::AreSame(bScope, bScopeDatum->GetScope(0));
			Assert::AreEqual(bScopeDatum, &(scope[1]));

			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(&scope, cScope.GetParent());

			Datum* cScopeDatum = scope.Find(c);
			Assert::IsNotNull(cScopeDatum);
			Assert::AreEqual(Datum::DatumTypes::Table, cScopeDatum->Type());
			Assert::AreSame(cScope, cScopeDatum->GetScope(0));
			Assert::AreEqual(cScopeDatum, &(scope[2]));

			Scope& anotherCScope = scope.AppendScope(c);
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(&scope, anotherCScope.GetParent());
			cScopeDatum = scope.Find(c);
			Assert::IsNotNull(cScopeDatum);
			Assert::AreEqual(Datum::DatumTypes::Table, cScopeDatum->Type());
			Assert::AreSame(anotherCScope, cScopeDatum->GetScope(1));

			Assert::ExpectException<invalid_argument>([&scope] {scope.AppendScope(""); });

			string d = "D"s;
			scope.Append(d) = 10;
			Assert::ExpectException<exception>([&scope, d] {scope.AppendScope(d); });
		}
		

		TEST_METHOD(Orphan)
		{
			Scope scope;
			Scope& aScope = scope.AppendScope("A"s);
			Assert::AreEqual(&scope, aScope.GetParent());
			aScope.Orphan();
			Assert::IsTrue(aScope.GetParent() == nullptr);

			delete &aScope;
		}

		TEST_METHOD(Adopt)
		{
			Scope* child = new Scope;
			Assert::IsNull(child->GetParent());

			Scope scope;
			scope.Adopt(*child, "A"s);
			Assert::AreEqual(&scope, child->GetParent());

			// Empty name test
			Assert::ExpectException<exception>([&scope, child] { scope.Adopt(*child, ""s); });

			// Existing, non-table key 
			scope.Append("NonTableDatum") = 10;
			Assert::ExpectException<exception>([&scope] {Scope s; scope.Adopt(s, "NonTableDatum"s); });

			// Self-adoption
			Assert::ExpectException<exception>([&scope, &child] { scope.Adopt(scope, "A"s); });

			// Ancestor-adoption
			Assert::ExpectException<exception>([&scope, &child] { child->Adopt(scope, "A"s); });

			Scope* grandchildScope = new Scope;
			child->Adopt(*grandchildScope, "B"s);
			Assert::AreEqual(child, grandchildScope->GetParent());

			// Grandparent Ancestor-adoption
			Assert::ExpectException<exception>([&grandchildScope, &scope] { grandchildScope->Adopt(scope, "B"s); });

			// Adoption that causes an orphan call
			Scope stealScope;
			stealScope.Adopt(*child, "A"s);
			Assert::AreEqual(child->GetParent(), &stealScope);
		}

		TEST_METHOD(AncestorDescendant)
		{
			Scope* child = new Scope;
			Assert::IsNull(child->GetParent());

			Scope scope;
			scope.Adopt(*child, "A"s);
			Assert::AreEqual(&scope, child->GetParent());

			Scope* grandchildScope = new Scope;
			child->Adopt(*grandchildScope, "B"s);
			Assert::AreEqual(child, grandchildScope->GetParent());

			Assert::IsTrue(grandchildScope->IsDescendantOf(scope));
			Assert::IsTrue(grandchildScope->IsDescendantOf(*child));
			Assert::IsFalse(grandchildScope->IsDescendantOf(*grandchildScope));
			Assert::IsFalse(grandchildScope->IsAncestorOf(*grandchildScope));

			Assert::IsTrue(child->IsDescendantOf(scope));
			Assert::IsTrue(child->IsAncestorOf(*grandchildScope));
			Assert::IsFalse(child->IsDescendantOf(*child));
			Assert::IsFalse(child->IsAncestorOf(*child));

			Assert::IsTrue(scope.IsAncestorOf(*child));
			Assert::IsTrue(scope.IsAncestorOf(*grandchildScope));
			Assert::IsFalse(scope.IsDescendantOf(scope));
			Assert::IsFalse(scope.IsAncestorOf(scope));
		}
		
		TEST_METHOD(Search)
		{
			Scope scope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;

			Datum& aDatum = scope.Append(a);

			// Search for a key in the current scope
			Scope* foundScope = nullptr;
			Datum* foundDatum = scope.Search(a, foundScope);
			Assert::IsNotNull(foundDatum);
			Assert::AreEqual(&aDatum, foundDatum);
			Assert::AreEqual(&scope, foundScope);

			// Search for a key from a nested scope
			Scope& bScope = scope.AppendScope(b);
			foundScope = nullptr;
			foundDatum = bScope.Search(a, foundScope);
			Assert::AreEqual(&aDatum, foundDatum);
			Assert::AreEqual(&scope, foundScope);

			// Search for a key that isn't anywhere in the hierarchy
			foundDatum = bScope.Search(c, foundScope);
			Assert::IsNull(foundDatum);
			Assert::IsNull(foundScope);

			// Searches without foundScope overload
			foundDatum = scope.Search(a);
			Assert::AreEqual(&aDatum, foundDatum);

			foundDatum = bScope.Search(a, foundScope);
			Assert::AreEqual(&aDatum, foundDatum);

			foundDatum = bScope.Search(c, foundScope);
			Assert::IsNull(foundDatum);

			Scope newScope;
			newScope.Append("D") = "New Scope";
			auto result = scope.FindContainedScope(newScope);
			Assert::IsNull(result.first);
			Assert::IsTrue(result.second == 0);
		}

		TEST_METHOD(CopyConstructor)
		{
			Scope scope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;

			Datum& aDatum = scope.Append(a);
			aDatum = "Hello, World"s;
			Datum& bDatum = scope.Append(b);
			bDatum = mat4(5);
			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(3_z, scope.Size());

			auto [cDatum, datumIndex] = scope.FindContainedScope(cScope);
			Assert::IsNotNull(cDatum);

			Scope copiedScope = scope;
			Assert::AreEqual(copiedScope.Size(), copiedScope.Size());
			Assert::AreEqual(3_z, copiedScope.Size());
			Datum* foundDatum = copiedScope.Find(a);
			Assert::AreEqual(aDatum.GetString(), foundDatum->GetString());
			Assert::AreEqual(&copiedScope[0], foundDatum);

			foundDatum = copiedScope.Find(b);
			Assert::AreEqual(bDatum.GetMatrix(), foundDatum->GetMatrix());
			Assert::AreEqual(&copiedScope[1], foundDatum);

			foundDatum = copiedScope.Find(c);
			Assert::AreEqual(cDatum->GetScope(), foundDatum->GetScope());
			Assert::IsNotNull(foundDatum->GetScope().GetParent());
			Assert::AreEqual(&copiedScope[2], foundDatum);


		}

		TEST_METHOD(CopyAssignment)
		{
			Scope scope;
			Scope copiedScope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;


			Datum& aDatum = scope.Append(a);
			aDatum = "Hello, World"s;
			Datum& bDatum = scope.Append(b);
			bDatum = mat4(5);
			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(3_z, scope.Size());

			copiedScope.AppendScope(c);
			copiedScope.Append(b);
			copiedScope.Append(a);
			Assert::AreEqual(3_z, copiedScope.Size());

			auto [cDatum, datumIndex] = scope.FindContainedScope(cScope);
			Assert::IsNotNull(cDatum);

			copiedScope = scope;
			Assert::AreEqual(copiedScope.Size(), copiedScope.Size());
			Assert::AreEqual(3_z, copiedScope.Size());
			Datum* foundDatum = copiedScope.Find(a);
			Assert::AreEqual(aDatum.GetString(), foundDatum->GetString());
			Assert::AreEqual(&copiedScope[0], foundDatum);

			foundDatum = copiedScope.Find(b);
			Assert::AreEqual(bDatum.GetMatrix(), foundDatum->GetMatrix());
			Assert::AreEqual(&copiedScope[1], foundDatum);

			foundDatum = copiedScope.Find(c);
			Assert::AreEqual(cDatum->GetScope(), foundDatum->GetScope());
			Assert::IsNotNull(foundDatum->GetScope().GetParent());
			Assert::AreEqual(&copiedScope[2], foundDatum);


		}


		TEST_METHOD(MoveConstructor)
		{
			Scope scope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;

			Datum& aDatum = scope.Append(a);
			aDatum = "Hello, World"s;
			Datum& bDatum = scope.Append(b);
			bDatum = mat4(5);
			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(3_z, scope.Size());

			auto [cDatum, datumIndex] = scope.FindContainedScope(cScope);
			Assert::IsNotNull(cDatum);

			Scope movedScope = std::move(scope);
			Assert::IsTrue(scope.IsEmpty());
			Assert::AreEqual(3_z, movedScope.Size());
			Datum* foundDatum = movedScope.Find(a);
			Assert::AreEqual(&aDatum, foundDatum);

			foundDatum = movedScope.Find(b);
			Assert::AreEqual(&bDatum, foundDatum);

			foundDatum = movedScope.Find(c);
			Assert::AreEqual(cDatum, foundDatum);
			Assert::AreEqual(foundDatum->GetScope().GetParent(), &movedScope);

			Scope* reparentedScope = new Scope(std::move(cScope));
			Assert::IsTrue(cScope.IsEmpty());
			Assert::IsNull(cScope.GetParent());
			Assert::AreEqual(&movedScope, reparentedScope->GetParent()); // Cannot move the parent
			foundDatum = movedScope.Find(c);
			Assert::AreEqual(cDatum, foundDatum);
			Assert::AreEqual(cDatum, movedScope.FindContainedScope(*reparentedScope).first);

			// The old object is now empty, but it's still allocated and must be deleted. This would
			// previously have been done as a newsted Scope, but as an orphan it would otherwise leak memory
			delete& cScope;
		}


		TEST_METHOD(MoveAssignment)
		{
			Scope scope;
			Scope movedScope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;

			Datum& aDatum = scope.Append(a);
			aDatum = "Hello, World"s;
			Datum& bDatum = scope.Append(b);
			bDatum = mat4(5);
			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(3_z, scope.Size());

			movedScope.AppendScope(c);
			movedScope.Append(b);
			movedScope.Append(a);
			Assert::AreEqual(3_z, movedScope.Size());

			auto [cDatum, datumIndex] = scope.FindContainedScope(cScope);
			Assert::IsNotNull(cDatum);

			movedScope = std::move(scope);
			Assert::IsTrue(scope.IsEmpty());
			Assert::AreEqual(3_z, movedScope.Size());
			Datum* foundDatum = movedScope.Find(a);
			Assert::AreEqual(&aDatum, foundDatum);

			foundDatum = movedScope.Find(b);
			Assert::AreEqual(&bDatum, foundDatum);

			foundDatum = movedScope.Find(c);
			Assert::AreEqual(cDatum, foundDatum);
			Assert::AreEqual(foundDatum->GetScope().GetParent(), &movedScope);

			Scope* reparentedScope = new Scope;
			*reparentedScope = std::move(cScope);
			Assert::IsTrue(cScope.IsEmpty());
			Assert::IsNull(cScope.GetParent());
			Assert::AreEqual(&movedScope, reparentedScope->GetParent()); // Cannot move the parent
			foundDatum = movedScope.Find(c);
			Assert::AreEqual(cDatum, foundDatum);
			Assert::AreEqual(cDatum, movedScope.FindContainedScope(*reparentedScope).first);

			// The old object is now empty, but it's still allocated and must be deleted. This would
			// previously have been done as a newsted Scope, but as an orphan it would otherwise leak memory
			delete& cScope;
		}

		TEST_METHOD(ConstTest)
		{
			Scope scope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;
			string d = "D"s;

			Datum& aDatum = scope.Append(a);
			aDatum = "Hello, World"s;
			Datum& bDatum = scope.Append(b);
			bDatum = mat4(5);
			const Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(3_z, scope.Size());

			const Scope constScope(scope);
			Datum stringDatum = constScope[0];
			Assert::AreEqual(stringDatum, aDatum);

			const Datum* matrixDatum = constScope.Find(b);
			Assert::AreEqual(*matrixDatum, bDatum);

			const Datum* scopeDatum = constScope.Search(c);
			Assert::AreEqual(scopeDatum->GetScope(), cScope);

			const Scope* foundScope = nullptr;
			scopeDatum = constScope.Search(c, foundScope);
			Assert::AreEqual(scopeDatum->GetScope(), cScope);
			Assert::AreEqual(&constScope, foundScope);

			foundScope = nullptr;
			scopeDatum = constScope.Search(d, foundScope);
			Assert::IsNull(scopeDatum);
			Assert::IsNull(foundScope);

			scopeDatum = cScope.Search(a, foundScope);
			Assert::AreEqual(*scopeDatum, constScope[0]);
			Assert::AreEqual(constScope, *foundScope);
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ScopeTests::sStartMemState;
}