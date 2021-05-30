#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Datum.h"
#include "Scope.h"
#include <glm/gtx/string_cast.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace glm;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Datum>(const Datum& t)
	{
		try
		{
			std::string str = ""s;
			for (size_t i = 0_z; i < t.Size(); ++i)
			{
				str.append(t.ToString(i));
			}
			return ToString(str);
		}
		catch (...)
		{
			return L"end()";
		}
	}

	template<>
	inline std::wstring ToString<Datum::DatumTypes>(const Datum::DatumTypes& t)
	{
		try
		{
			return ToString(static_cast<int>(t));
		}
		catch (...)
		{
			return L"end()";
		}
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
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

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4 &t)
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

	template<>
	inline std::wstring ToString<RTTI>(RTTI* t)
	{
		try
		{
			return ToString(t->ToString());
		}
		catch (...)
		{
			return L"end()";
		}
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTests)
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


		TEST_METHOD(TestConstructor)
		{
			Datum d;
			Assert::AreEqual(Datum::DatumTypes::Unknown, d.Type());

			Datum i(Datum::DatumTypes::Integer);
			Assert::AreEqual(Datum::DatumTypes::Unknown, d.Type());

			Datum f(Datum::DatumTypes::Float);
			Assert::AreEqual(Datum::DatumTypes::Unknown, d.Type());

			Datum s(Datum::DatumTypes::String);
			Assert::AreEqual(Datum::DatumTypes::Unknown, d.Type());

			Datum v(Datum::DatumTypes::Vector);
			Assert::AreEqual(Datum::DatumTypes::Unknown, d.Type());

			Datum m(Datum::DatumTypes::Matrix);
			Assert::AreEqual(Datum::DatumTypes::Unknown, d.Type());

			Datum p(Datum::DatumTypes::Pointer);
			Assert::AreEqual(Datum::DatumTypes::Unknown, d.Type());

			Datum vp(Datum::DatumTypes::Unknown);
			Assert::AreEqual(Datum::DatumTypes::Unknown, d.Type());
		}

		TEST_METHOD(TestSpecialMembers)
		{
			// Test Initializer List Copy Construction
			{
				Datum i{ 5, 10, 20 };

				Datum f{ 5.f, 10.f, 20.f };

				Datum s{ "Hello"s, "Hi"s, "Henlo"s };

				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };

				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				Datum p{ a_rtti, b_rtti, c_rtti };
			}

			// Test Copy Constructor
			{
				Datum i{ 5, 10, 20 };
				Datum i_copy(i);

				Datum f{ 5.f, 10.f, 20.f };
				Datum f_copy(f);

				Datum s{ "Hello"s, "Hi"s, "Henlo"s };
				Datum s_copy(s);

				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				Datum v_copy(v);

				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				Datum m_copy(m);

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				Datum p{ a_rtti, b_rtti, c_rtti };
				Datum p_copy(p);
			}

			// Test Copy Assignment
			{
				Datum i{ 5, 10, 20 };
				Datum i_copy;
				i_copy = i;

				Datum f{ 5.f, 10.f, 20.f };
				Datum f_copy;
				f_copy = f;

				Datum s{ "Hello"s, "Hi"s, "Henlo"s };
				Datum s_copy;
				s_copy = s;

				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				Datum v_copy;
				v_copy = v;

				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				Datum m_copy;
				m_copy = m;

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				Datum p{ a_rtti, b_rtti, c_rtti };
				Datum p_copy;
				p_copy = p;
			}

			// Test Move Constructor
			{
				
				Datum i{ 5, 10, 20 };
				Datum i_move(move(i));

				Datum f{ 5.f, 10.f, 20.f };
				Datum f_move(move(f));

				Datum s{ "Hello"s, "Hi"s, "Henlo"s };
				Datum s_move(move(s));

				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				Datum v_move(move(v));

				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				Datum m_move(move(m));

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				Datum p{ a_rtti, b_rtti, c_rtti };
				Datum p_move(move(p));
			}

			// Test Move Assignment
			{
				Datum i{ 5, 10, 20 };
				Datum i_move{ 20, 10, 5 };
				i_move = move(i);

				
				Datum f{ 5.f, 10.f, 20.f };
				Datum f_move{20.f, 10.f, 5.f};
				f_move = move(f);

				Datum s{ "Hello"s, "Hi"s, "Henlo"s };
				Datum s_move{ "Henlo"s, "Hi"s, "Hello"s };
				s_move = move(s);

				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				Datum v_move{ glm::vec4(2), glm::vec4(1), glm::vec4(0) };
				v_move = move(v);

				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				Datum m_move{ glm::mat4(2), glm::mat4(1), glm::mat4(0) };;
				m_move = move(m);

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				Datum p{ a_rtti, b_rtti, c_rtti };
				Datum p_move{ c_rtti, b_rtti, a_rtti };
				p_move = move(p);
			}
		}

		TEST_METHOD(TestCopyOperatorOverloads)
		{
			// Copy Constructor Overloads
			{
				Datum i(10);

				Datum f(10.f);

				Datum s("Hello"s);

				Datum v(glm::vec4(0));

				Datum m(glm::mat4(0));

				Foo a;
				RTTI* a_rtti = &a;

				Datum p(a_rtti);
			}

			// Copy Assignment Overloads
			{
				Datum i;
				i = 10;
				Datum i_list{ 5, 10, 20 };
				i_list = 20;

				Datum f;
				f = 10.f;
				Datum f_list{ 5.f, 10.f, 20.f };
				f_list = 20.f;

				Datum s;
				s = "Hello"s;
				Datum s_list{ "Hello"s, "Hi"s, "Henlo"s };
				s_list = "Hello"s;

				Datum v;
				v = glm::vec4(0);
				Datum v_list{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				v_list = glm::vec4(0);

				Datum m;
				m = glm::mat4(0);
				Datum m_list{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				m_list = glm::mat4(0);

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;

				Datum p;
				p = a_rtti;
				Datum p_list{ a_rtti, b_rtti, c_rtti };
				p_list = a_rtti;

				Datum sc;
				Scope scope;
				sc = scope;
				Assert::AreEqual(sc.Type(), Datum::DatumTypes::Table);
			}
		}

		TEST_METHOD(TestComparison)
		{
			// Datum Comparisons
			{
				Datum i1{ 5, 10, 20 };
				Datum i2{ 20, 10, 5 };
				Datum i3{ 5, 10, 20 };
				Assert::IsTrue(i1 != i2);
				Assert::IsTrue(i1 == i3);

				Datum f1{ 5.f, 10.f, 20.f };
				Datum f2{ 20.f, 10.f, 5.f };
				Datum f3{ 5.f, 10.f, 20.f };
				Assert::IsTrue(f1 != f2);
				Assert::IsTrue(f1 == f3);

				
				Datum s1{ "Hello"s, "Hi"s, "Hiiiiii"s };
				Datum s2{ "Hiiiiii"s, "Hi"s, "Hello"s };
				Datum s3{ "Hello"s, "Hi"s, "Hiiiiii"s };
				Assert::IsTrue(s1 != s2);
				Assert::IsTrue(s1 == s3);
				
				Datum v1{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				Datum v2{ glm::vec4(2), glm::vec4(1), glm::vec4(0) };
				Datum v3{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				Assert::IsTrue(v1 != v2);
				Assert::IsTrue(v1 == v3);

				Datum m1{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				Datum m2{ glm::mat4(2), glm::mat4(1), glm::mat4(0) };
				Datum m3{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				Assert::IsTrue(m1 != m2);
				Assert::IsTrue(m1 == m3);
			
				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				Datum p1{ a_rtti, b_rtti, c_rtti };
				Datum p2{ c_rtti, b_rtti, a_rtti };
				Datum p3{ a_rtti, b_rtti, c_rtti };
				Assert::IsTrue(p1 != p2);
				Assert::IsTrue(p1 == p3);
			}

			// Comparison Overloads
			{
				Datum i1{ 5 };
				Assert::IsTrue(i1 == 5);
				Assert::IsTrue(i1 != 10);

				Datum f1{ 5.f };
				Assert::IsTrue(f1 == 5.f);
				Assert::IsTrue(f1 != 10.f);

				Datum s1{ "Hello"s };
				Assert::IsTrue(s1 == "Hello"s);
				Assert::IsTrue(s1 != "Hi"s);

				Datum v1{ glm::vec4(0) };
				Assert::IsTrue(v1 == glm::vec4(0));
				Assert::IsTrue(v1 != glm::vec4(1));

				Datum m1{ glm::mat4(0) };
				Assert::IsTrue(m1 == glm::mat4(0));
				Assert::IsTrue(m1 != glm::mat4(1));

				Foo a(10), b(20);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				Datum p1{ a_rtti };
				Assert::IsTrue(p1 == a_rtti);
				Assert::IsTrue(p1 != b_rtti);
			}
		}

		TEST_METHOD(TestSetType)
		{
			Datum d(Datum::DatumTypes::Float);

			Assert::ExpectException<exception>([&d] { d.SetType(Datum::DatumTypes::Integer); }, L"Expected an exception, but none was thrown");
			d.Reserve(100);
		}

		TEST_METHOD(TestReserveResize)
		{
			// Test Reserve
			{
				Datum d;
				Assert::ExpectException<exception>([&d] { d.Reserve(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.Resize(100_z); }, L"Expected an exception, but none was thrown");
				
			}

			// Test Resize
			{
				Datum i{ 5, 10, 20 };
				i.Resize(100_z);

				Datum f{ 5.f, 10.f, 20.f };
				f.Resize(100_z);

				Datum s{ "Hello"s, "Hi"s, "Henlo"s };
				s.Resize(100_z);

				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				v.Resize(100_z);

				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				m.Resize(100_z);

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				Datum p{ a_rtti, b_rtti, c_rtti };
				p.Resize(100_z);
			}
		}

		TEST_METHOD(TestGetSet)
		{
			{
				Datum datumInt{ 0 };
				Datum datumFloat{ 0.f };
				Foo test;
				RTTI* test_rtti = &test;
				Scope scope;

				Assert::ExpectException<exception>([&datumFloat] { datumFloat.GetInteger(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.GetFloat(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.GetString(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.GetVector(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.GetMatrix(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.GetPointer(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.GetScope(0_z); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&datumFloat] { datumFloat.Set(0); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.Set(0.f); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.Set(""); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.Set(glm::vec4(0)); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.Set(glm::mat4(0)); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt, &test_rtti] { datumInt.Set(test_rtti); }, L"Expected an exception, but none was thrown");

				Datum empty;

				Assert::ExpectException<exception>([&empty] { empty.GetInteger(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.GetFloat(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.GetString(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.GetVector(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.GetMatrix(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.GetPointer(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.GetScope(100_z); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&empty] { empty.Set(0, 100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.Set(0.f, 100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.Set("", 100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.Set(glm::vec4(0), 100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty] { empty.Set(glm::mat4(0), 100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&empty, &test_rtti] { empty.Set(test_rtti); }, L"Expected an exception, but none was thrown");

				Datum i{ 5, 10, 20 };
				i.Set(20, 0_z);
				i.Set(10, 1_z);
				i.Set(5, 2_z);
				Assert::AreEqual(i.GetInteger(0_z), 20);
				Assert::AreEqual(i.GetInteger(1_z), 10);
				Assert::AreEqual(i.GetInteger(2_z), 5);

				Datum f{ 5.f, 10.f, 20.f };
				f.Set(20.f, 0_z);
				f.Set(10.f, 1_z);
				f.Set(5.f, 2_z);
				Assert::AreEqual(f.GetFloat(0_z), 20.f);
				Assert::AreEqual(f.GetFloat(1_z), 10.f);
				Assert::AreEqual(f.GetFloat(2_z), 5.f);

				Datum s{ "Hello"s, "Hi"s, "Henlo"s };
				s.Set("Henlo"s, 0_z);
				s.Set("Hi"s, 1_z);
				s.Set("Hello"s, 2_z);
				Assert::AreEqual(s.GetString(0_z), "Henlo"s);
				Assert::AreEqual(s.GetString(1_z), "Hi"s);
				Assert::AreEqual(s.GetString(2_z), "Hello"s);

				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				v.Set(glm::vec4(2), 0_z);
				v.Set(glm::vec4(1), 1_z);
				v.Set(glm::vec4(0), 2_z);
				Assert::AreEqual(v.GetVector(0_z), glm::vec4(2));
				Assert::AreEqual(v.GetVector(1_z), glm::vec4(1));
				Assert::AreEqual(v.GetVector(2_z), glm::vec4(0));

				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				m.Set(glm::mat4(2), 0_z);
				m.Set(glm::mat4(1), 1_z);
				m.Set(glm::mat4(0), 2_z);
				Assert::AreEqual(m.GetMatrix(0_z), glm::mat4(2));
				Assert::AreEqual(m.GetMatrix(1_z), glm::mat4(1));
				Assert::AreEqual(m.GetMatrix(2_z), glm::mat4(0));

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				Datum p{ a_rtti, b_rtti, c_rtti };
				p.Set(c_rtti, 0_z);
				p.Set(b_rtti, 1_z);
				p.Set(a_rtti, 2_z);
				Assert::AreEqual(p.GetPointer(0_z), c_rtti);
				Assert::AreEqual(p.GetPointer(1_z), b_rtti);
				Assert::AreEqual(p.GetPointer(2_z), a_rtti);
			}

			// Const Get Test
			{
				const Datum constDatumInt{ 0 };
				const Datum constDatumFloat{ 0.f };

				Assert::ExpectException<exception>([&constDatumFloat] { constDatumFloat.GetInteger(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.GetFloat(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.GetString(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.GetVector(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.GetMatrix(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.GetPointer(0_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.GetScope(0_z); }, L"Expected an exception, but none was thrown");

				const Datum constEmpty;

				Assert::ExpectException<exception>([&constEmpty] { constEmpty.GetInteger(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constEmpty] { constEmpty.GetFloat(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constEmpty] { constEmpty.GetString(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constEmpty] { constEmpty.GetVector(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constEmpty] { constEmpty.GetMatrix(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constEmpty] { constEmpty.GetPointer(100_z); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constEmpty] { constEmpty.GetScope(100_z); }, L"Expected an exception, but none was thrown");

				Datum constI{ 5, 10, 20 };
				Datum constF{ 5.f, 10.f, 20.f };
				Datum constS{ "Hello"s, "Hi"s, "Henlo"s };
				Datum constV{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				Datum constM{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				const Datum constP{ a_rtti, b_rtti, c_rtti };

				Assert::AreEqual(constI.GetInteger(0_z), 5);
				Assert::AreEqual(constF.GetFloat(0_z), 5.f);
				Assert::AreEqual(constS.GetString(0_z), "Hello"s);
				Assert::AreEqual(constV.GetVector(0_z), glm::vec4(0));
				Assert::AreEqual(constM.GetMatrix(0_z), glm::mat4(0));
				Assert::AreEqual(constP.GetPointer(0_z), a_rtti);
			}
		}

		TEST_METHOD(TestSetStringToString)
		{
			{
				Datum empty;
				std::string empty_str = "";
				Assert::ExpectException<exception>([&empty, &empty_str] { empty.SetFromString(empty_str, 0_z); }, L"Expected an exception, but none was thrown");

				std::string str_5 = "5";
				std::string str_10 = "10";
				std::string str_20 = "20";
				Datum i{ 5, 10, 20 };
				i.SetFromString(str_20, 0_z);
				i.SetFromString(str_10, 1_z);
				i.SetFromString(str_5, 2_z);
				Assert::AreEqual(i.ToString(0_z), to_string(20));
				Assert::AreEqual(i.ToString(1_z), to_string(10));
				Assert::AreEqual(i.ToString(2_z), to_string(5));

				std::string str_f5 = "5.0";
				std::string str_f10 = "10.0";
				std::string str_f20 = "20.0";
				Datum f{ 5.f, 10.f, 20.f };
				f.SetFromString(str_f20, 0_z);
				f.SetFromString(str_f10, 1_z);
				f.SetFromString(str_f5, 2_z);
				Assert::AreEqual(f.ToString(0_z), to_string(20.f));
				Assert::AreEqual(f.ToString(1_z), to_string(10.f));
				Assert::AreEqual(f.ToString(2_z), to_string(5.f));


				std::string str_hello = "Hello";
				std::string str_hi = "Hi";
				std::string str_henlo = "Henlo";
				Datum s{ "Hello"s, "Hi"s, "Henlo"s };
				s.SetFromString(str_henlo, 0_z);
				s.SetFromString(str_hi, 1_z);
				s.SetFromString(str_hello, 2_z);
				Assert::AreEqual(s.ToString(0_z), "Henlo"s);
				Assert::AreEqual(s.ToString(1_z), "Hi"s);
				Assert::AreEqual(s.ToString(2_z), "Hello"s);

				std::string str_vec0 = glm::to_string(glm::vec4(0));
				std::string str_vec1 = glm::to_string(glm::vec4(1));
				std::string str_vec2 = glm::to_string(glm::vec4(2));
				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				v.SetFromString(str_vec2, 0_z);
				v.SetFromString(str_vec1, 1_z);
				v.SetFromString(str_vec0, 2_z);
				Assert::AreEqual(v.ToString(0_z), glm::to_string(glm::vec4(2)));
				Assert::AreEqual(v.ToString(1_z), glm::to_string(glm::vec4(1)));
				Assert::AreEqual(v.ToString(2_z), glm::to_string(glm::vec4(0)));

				std::string str_mat0 = glm::to_string(glm::mat4(0));
				std::string str_mat1 = glm::to_string(glm::mat4(1));
				std::string str_mat2 = glm::to_string(glm::mat4(2));
				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				m.SetFromString(str_mat2, 0_z);
				m.SetFromString(str_mat1, 1_z);
				m.SetFromString(str_mat0, 2_z);
				Assert::AreEqual(m.ToString(0_z), glm::to_string(glm::mat4(2)));
				Assert::AreEqual(m.ToString(1_z), glm::to_string(glm::mat4(1)));
				Assert::AreEqual(m.ToString(2_z), glm::to_string(glm::mat4(0)));

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;

				std::string str_ptr = a_rtti->ToString();
				Datum p{ a_rtti, b_rtti, c_rtti };
				Assert::ExpectException<exception>([&str_ptr, &p] { p.SetFromString(str_ptr, 0_z); }, L"Expected an exception, but none was thrown");
				
				Assert::AreEqual(p.ToString(0_z), a_rtti->ToString());
				Assert::AreEqual(p.ToString(1_z), b_rtti->ToString());
				Assert::AreEqual(p.ToString(2_z), c_rtti->ToString());
			}
		}

		TEST_METHOD(TestPushBackPopBackFrontBack)
		{
			// Exceptions
			{
				int i = 10;
				float f = 10.f;
				std::string s = "Hello"s;
				glm::vec4 v = glm::vec4(0);
				glm::mat4 m = glm::mat4(0);
				Foo a;
				RTTI* p = &a;

				Datum d;
				const Datum constD;
				Assert::ExpectException<exception>([&d] { d.FrontInteger(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.FrontFloat(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.FrontString(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.FrontVector(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.FrontMatrix(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.FrontPointer(); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&d] { d.BackInteger(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.BackFloat(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.BackString(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.BackVector(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.BackMatrix(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.BackPointer(); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&constD] { constD.FrontInteger(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.FrontFloat(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.FrontString(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.FrontVector(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.FrontMatrix(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.FrontPointer(); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&constD] { constD.BackInteger(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.BackFloat(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.BackString(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.BackVector(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.BackMatrix(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constD] { constD.BackPointer(); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&d] { d.PopBack(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d, &i] { d.PushBack(i); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.PushBack(10); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d, &f] { d.PushBack(f); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.PushBack(10.f); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d, &s] { d.PushBack(s); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.PushBack("Hello"s); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d, &v] { d.PushBack(v); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.PushBack(glm::vec4(0)); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d, &m] { d.PushBack(m); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d] { d.PushBack(glm::mat4(0)); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&d, &p] { d.PushBack(p); }, L"Expected an exception, but none was thrown");


				Datum datumInt{ 0 };
				Datum datumFloat{ 0.f };
				const Datum constDatumInt{ 0 };
				const Datum constDatumFloat{ 0.f };

				Assert::ExpectException<exception>([&datumFloat] { datumFloat.FrontInteger(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.FrontFloat(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.FrontString(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.FrontVector(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.FrontMatrix(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.FrontPointer(); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&datumFloat] { datumFloat.BackInteger(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.BackFloat(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.BackString(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.BackVector(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.BackMatrix(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt] { datumInt.BackPointer(); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&constDatumFloat] { constDatumFloat.FrontInteger(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.FrontFloat(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.FrontString(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.FrontVector(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.FrontMatrix(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.FrontPointer(); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&constDatumFloat] { constDatumFloat.BackInteger(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.BackFloat(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.BackString(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.BackVector(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.BackMatrix(); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constDatumInt] { constDatumInt.BackPointer(); }, L"Expected an exception, but none was thrown");

			}

			// Integers
			{
				// LValue PushBack Test
				{
					int a = 10;
					int b = 20;
					int c = 30;

					Datum d;
					d.SetType(Datum::DatumTypes::Integer);

					d.PushBack(a);
					Assert::AreEqual(d.GetInteger(0_z), a);
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontInteger(), a);
					Assert::AreEqual(d.BackInteger(), a);

					d.PushBack(b);
					Assert::AreEqual(d.GetInteger(1_z), b);
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontInteger(), a);
					Assert::AreEqual(d.BackInteger(), b);

					d.PushBack(c);
					Assert::AreEqual(d.GetInteger(2_z), c);
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontInteger(), a);
					Assert::AreEqual(d.BackInteger(), c);

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontInteger(), a);
					Assert::AreEqual(d.BackInteger(), b);

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontInteger(), a);
					Assert::AreEqual(d.BackInteger(), a);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// RValue PushBack Test
				{
					Datum d;

					d.SetType(Datum::DatumTypes::Integer);

					d.PushBack(10);
					Assert::AreEqual(d.GetInteger(0_z), 10);
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontInteger(), 10);
					Assert::AreEqual(d.BackInteger(), 10);

					d.PushBack(20);
					Assert::AreEqual(d.GetInteger(1_z), 20);
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontInteger(), 10);
					Assert::AreEqual(d.BackInteger(), 20);

					d.PushBack(30);
					Assert::AreEqual(d.GetInteger(2_z), 30);
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontInteger(), 10);
					Assert::AreEqual(d.BackInteger(), 30);

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontInteger(), 10);
					Assert::AreEqual(d.BackInteger(), 20);

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontInteger(), 10);
					Assert::AreEqual(d.BackInteger(), 10);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// Const Test
				{
					int a = 10;
					int b = 20;
					int c = 30;

					const Datum constD{ a, b, c };
					Assert::AreEqual(constD.Size(), 3_z);
					Assert::AreEqual(constD.FrontInteger(), 10);
					Assert::AreEqual(constD.BackInteger(), 30);
				}
			}

			// Floats
			{
				// LValue PushBack Test
				{
					float a = 10.f;
					float b = 20.f;
					float c = 30.f;

					Datum d;

					d.SetType(Datum::DatumTypes::Float);

					d.PushBack(a);
					Assert::AreEqual(d.GetFloat(0_z), a);
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontFloat(), a);
					Assert::AreEqual(d.BackFloat(), a);

					d.PushBack(b);
					Assert::AreEqual(d.GetFloat(1_z), b);
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontFloat(), a);
					Assert::AreEqual(d.BackFloat(), b);

					d.PushBack(c);
					Assert::AreEqual(d.GetFloat(2_z), c);
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontFloat(), a);
					Assert::AreEqual(d.BackFloat(), c);

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontFloat(), a);
					Assert::AreEqual(d.BackFloat(), b);

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontFloat(), a);
					Assert::AreEqual(d.BackFloat(), a);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// RValue PushBack Test
				{
					Datum d;

					d.SetType(Datum::DatumTypes::Float);

					d.PushBack(10.f);
					Assert::AreEqual(d.GetFloat(0_z), 10.f);
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontFloat(), 10.f);
					Assert::AreEqual(d.BackFloat(), 10.f);

					d.PushBack(20.f);
					Assert::AreEqual(d.GetFloat(1_z), 20.f);
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontFloat(), 10.f);
					Assert::AreEqual(d.BackFloat(), 20.f);

					d.PushBack(30.f);
					Assert::AreEqual(d.GetFloat(2_z), 30.f);
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontFloat(), 10.f);
					Assert::AreEqual(d.BackFloat(), 30.f);

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontFloat(), 10.f);
					Assert::AreEqual(d.BackFloat(), 20.f);

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontFloat(), 10.f);
					Assert::AreEqual(d.BackFloat(), 10.f);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// Const Test
				{
					float a = 10.f;
					float b = 20.f;
					float c = 30.f;

					const Datum constD{ a, b, c };
					Assert::AreEqual(constD.Size(), 3_z);
					Assert::AreEqual(constD.FrontFloat(), 10.f);
					Assert::AreEqual(constD.BackFloat(), 30.f);
				}
			}

			// Strings
			{
				// LValue PushBack Test
				{
					std::string a = "Hello"s;
					std::string b = "Hi"s;
					std::string c = "Henlo"s;

					Datum d;

					d.SetType(Datum::DatumTypes::String);

					d.PushBack(a);
					Assert::AreEqual(d.GetString(0_z), a);
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontString(), a);
					Assert::AreEqual(d.BackString(), a);

					d.PushBack(b);
					Assert::AreEqual(d.GetString(1_z), b);
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontString(), a);
					Assert::AreEqual(d.BackString(), b);

					d.PushBack(c);
					Assert::AreEqual(d.GetString(2_z), c);
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontString(), a);
					Assert::AreEqual(d.BackString(), c);

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontString(), a);
					Assert::AreEqual(d.BackString(), b);

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontString(), a);
					Assert::AreEqual(d.BackString(), a);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// RValue PushBack Test
				{
					Datum d;

					d.SetType(Datum::DatumTypes::String);

					d.PushBack("Hello"s);
					Assert::AreEqual(d.GetString(0_z), "Hello"s);
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontString(), "Hello"s);
					Assert::AreEqual(d.BackString(), "Hello"s);

					d.PushBack("Hi"s);
					Assert::AreEqual(d.GetString(1_z), "Hi"s);
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontString(), "Hello"s);
					Assert::AreEqual(d.BackString(), "Hi"s);

					d.PushBack("Henlo"s);
					Assert::AreEqual(d.GetString(2_z), "Henlo"s);
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontString(), "Hello"s);
					Assert::AreEqual(d.BackString(), "Henlo"s);

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontString(), "Hello"s);
					Assert::AreEqual(d.BackString(), "Hi"s);

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontString(), "Hello"s);
					Assert::AreEqual(d.BackString(), "Hello"s);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// Const Test
				{
					std::string a = "Hello"s;
					std::string b = "Hi"s;
					std::string c = "Henlo"s;

					const Datum constD{ a, b, c };
					Assert::AreEqual(constD.Size(), 3_z);
					Assert::AreEqual(constD.FrontString(), "Hello"s);
					Assert::AreEqual(constD.BackString(), "Henlo"s);
				}
			}

			// Vectors
			{
				// LValue PushBack Test
				{
					glm::vec4 a = glm::vec4(0);
					glm::vec4 b = glm::vec4(1);
					glm::vec4 c = glm::vec4(2);

					Datum d;

					d.SetType(Datum::DatumTypes::Vector);

					d.PushBack(a);
					Assert::AreEqual(d.GetVector(0_z), a);
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontVector(), a);
					Assert::AreEqual(d.BackVector(), a);

					d.PushBack(b);
					Assert::AreEqual(d.GetVector(1_z), b);
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontVector(), a);
					Assert::AreEqual(d.BackVector(), b);

					d.PushBack(c);
					Assert::AreEqual(d.GetVector(2_z), c);
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontVector(), a);
					Assert::AreEqual(d.BackVector(), c);

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontVector(), a);
					Assert::AreEqual(d.BackVector(), b);

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontVector(), a);
					Assert::AreEqual(d.BackVector(), a);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// RValue PushBack Test
				{
					Datum d;

					d.SetType(Datum::DatumTypes::Vector);

					d.PushBack(glm::vec4(0));
					Assert::AreEqual(d.GetVector(0_z), glm::vec4(0));
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontVector(), glm::vec4(0));
					Assert::AreEqual(d.BackVector(), glm::vec4(0));

					d.PushBack(glm::vec4(1));
					Assert::AreEqual(d.GetVector(1_z), glm::vec4(1));
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontVector(), glm::vec4(0));
					Assert::AreEqual(d.BackVector(), glm::vec4(1));

					d.PushBack(glm::vec4(2));
					Assert::AreEqual(d.GetVector(2_z), glm::vec4(2));
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontVector(), glm::vec4(0));
					Assert::AreEqual(d.BackVector(), glm::vec4(2));

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontVector(), glm::vec4(0));
					Assert::AreEqual(d.BackVector(), glm::vec4(1));

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontVector(), glm::vec4(0));
					Assert::AreEqual(d.BackVector(), glm::vec4(0));

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// Const Test
				{
					glm::vec4 a = glm::vec4(0);
					glm::vec4 b = glm::vec4(1);
					glm::vec4 c = glm::vec4(2);

					const Datum constD{ a, b, c };
					Assert::AreEqual(constD.Size(), 3_z);
					Assert::AreEqual(constD.FrontVector(), glm::vec4(0));
					Assert::AreEqual(constD.BackVector(), glm::vec4(2));
				}
			}

			// Matrixs
			{
				// LValue PushBack Test
				{
					glm::mat4 a = glm::mat4(0);
					glm::mat4 b = glm::mat4(1);
					glm::mat4 c = glm::mat4(2);

					Datum d;

					d.SetType(Datum::DatumTypes::Matrix);

					d.PushBack(a);
					Assert::AreEqual(d.GetMatrix(0_z), a);
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontMatrix(), a);
					Assert::AreEqual(d.BackMatrix(), a);

					d.PushBack(b);
					Assert::AreEqual(d.GetMatrix(1_z), b);
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontMatrix(), a);
					Assert::AreEqual(d.BackMatrix(), b);

					d.PushBack(c);
					Assert::AreEqual(d.GetMatrix(2_z), c);
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontMatrix(), a);
					Assert::AreEqual(d.BackMatrix(), c);

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontMatrix(), a);
					Assert::AreEqual(d.BackMatrix(), b);

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontMatrix(), a);
					Assert::AreEqual(d.BackMatrix(), a);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// RValue PushBack Test
				{
					Datum d;

					d.SetType(Datum::DatumTypes::Matrix);

					d.PushBack(glm::mat4(0));
					Assert::AreEqual(d.GetMatrix(0_z), glm::mat4(0));
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontMatrix(), glm::mat4(0));
					Assert::AreEqual(d.BackMatrix(), glm::mat4(0));

					d.PushBack(glm::mat4(1));
					Assert::AreEqual(d.GetMatrix(1_z), glm::mat4(1));
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontMatrix(), glm::mat4(0));
					Assert::AreEqual(d.BackMatrix(), glm::mat4(1));

					d.PushBack(glm::mat4(2));
					Assert::AreEqual(d.GetMatrix(2_z), glm::mat4(2));
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontMatrix(), glm::mat4(0));
					Assert::AreEqual(d.BackMatrix(), glm::mat4(2));

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontMatrix(), glm::mat4(0));
					Assert::AreEqual(d.BackMatrix(), glm::mat4(1));

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontMatrix(), glm::mat4(0));
					Assert::AreEqual(d.BackMatrix(), glm::mat4(0));

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// Const Test
				{
					glm::mat4 a = glm::mat4(0);
					glm::mat4 b = glm::mat4(1);
					glm::mat4 c = glm::mat4(2);

					const Datum constD{ a, b, c };
					Assert::AreEqual(constD.Size(), 3_z);
					Assert::AreEqual(constD.FrontMatrix(), glm::mat4(0));
					Assert::AreEqual(constD.BackMatrix(), glm::mat4(2));
				}
			}

			// Pointers
			{
				Foo a(10), b(20), c(30);

				// LValue PushBack Test
				{
					RTTI* a_rtti = &a;
					RTTI* b_rtti = &b;
					RTTI* c_rtti = &c;

					Datum d;

					d.SetType(Datum::DatumTypes::Pointer);

					d.PushBack(a_rtti);
					Assert::AreEqual(d.GetPointer(0_z), a_rtti);
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontPointer(), a_rtti);
					Assert::AreEqual(d.BackPointer(), a_rtti);

					d.PushBack(b_rtti);
					Assert::AreEqual(d.GetPointer(1_z), b_rtti);
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontPointer(), a_rtti);
					Assert::AreEqual(d.BackPointer(), b_rtti);

					d.PushBack(c_rtti);
					Assert::AreEqual(d.GetPointer(2_z), c_rtti);
					Assert::AreEqual(d.Size(), 3_z);
					Assert::AreEqual(d.FrontPointer(), a_rtti);
					Assert::AreEqual(d.BackPointer(), c_rtti);

					d.PopBack();
					Assert::AreEqual(d.Size(), 2_z);
					Assert::AreEqual(d.FrontPointer(), a_rtti);
					Assert::AreEqual(d.BackPointer(), b_rtti);

					d.PopBack();
					Assert::AreEqual(d.Size(), 1_z);
					Assert::AreEqual(d.FrontPointer(), a_rtti);
					Assert::AreEqual(d.BackPointer(), a_rtti);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);

					d.PopBack();
					Assert::AreEqual(d.Size(), 0_z);
				}

				// Const Test
				{
					RTTI* a_rtti = &a;
					RTTI* b_rtti = &b;
					RTTI* c_rtti = &c;

					const Datum constD{ a_rtti, b_rtti, c_rtti };
					Assert::AreEqual(constD.Size(), 3_z);
					Assert::AreEqual(constD.FrontPointer(), a_rtti);
					Assert::AreEqual(constD.BackPointer(), c_rtti);
				}
			}
		}

		TEST_METHOD(TestFindRemove)
		{
			// Exceptions
			{
				int i = 10;
				float f = 10.f;
				std::string s = "Hello"s;
				glm::vec4 v = glm::vec4(0);
				glm::mat4 m = glm::mat4(0);
				Foo a;
				const RTTI* p = &a;

				Datum d;
				Datum datumInt{ 0 };
				Datum datumFloat{ 0.f };

				Assert::ExpectException<exception>([&datumFloat, &i] { datumFloat.Remove(i); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt, &f] { datumInt.Remove(f); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt, &s] { datumInt.Remove(s); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt, &v] { datumInt.Remove(v); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt, &m] { datumInt.Remove(m); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&datumInt, &p] { datumInt.Remove(p); }, L"Expected an exception, but none was thrown");

				Assert::ExpectException<exception>([&d] { d.Remove(0.f); }, L"Expected an exception, but none was thrown");


			}

			// Find
			{
				size_t index = 0_z;
				Datum i{ 5, 10, 20 };
				const Datum constI{ 5, 10, 20 };
				index = i.IndexOf(5);
				Assert::AreEqual(index, 0_z);
				index = constI.IndexOf(5);
				Assert::AreEqual(index, 0_z);

				Datum f{ 5.f, 10.f, 20.f };
				const Datum constF{ 5.f, 10.f, 20.f };
				index = f.IndexOf(5.f);
				Assert::AreEqual(index, 0_z);
				index = constF.IndexOf(5.f);
				Assert::AreEqual(index, 0_z);

				Datum s{ "Hello"s, "Hi"s, "Henlo"s };
				const Datum constS{ "Hello"s, "Hi"s, "Henlo"s };
				index = f.IndexOf("Hello"s);
				Assert::AreEqual(index, 0_z);
				index = constF.IndexOf("Hello"s);
				Assert::AreEqual(index, 0_z);

				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				const Datum constV{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				index = v.IndexOf(glm::vec4(0));
				Assert::AreEqual(index, 0_z);
				index = constV.IndexOf(glm::vec4(0));
				Assert::AreEqual(index, 0_z);

				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				const Datum constM{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				index = m.IndexOf(glm::mat4(0));
				Assert::AreEqual(index, 0_z);
				index = constM.IndexOf(glm::mat4(0));
				Assert::AreEqual(index, 0_z);

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				const RTTI* c_a_rtti = &a;
				Datum p{ a_rtti, b_rtti, c_rtti };
				const Datum constP{ a_rtti, b_rtti, c_rtti };
				index = p.IndexOf(c_a_rtti);
				Assert::AreEqual(index, 0_z);
				index = constP.IndexOf(c_a_rtti);
				Assert::AreEqual(index, 0_z);
			}

			// Remove
			{
				Datum empty;
				Assert::ExpectException<exception>([&empty] { empty.RemoveAt(0_z); }, L"Expected an exception, but none was thrown");

				size_t index = 0_z;
				Datum i{ 5, 10, 20 };
				const Datum constI{ 5, 10, 20 };
				index = i.Remove(5);
				Assert::AreEqual(i.Size(), 2_z);

				Datum f{ 5.f, 10.f, 20.f };
				index = f.Remove(5.f);
				Assert::AreEqual(f.Size(), 2_z);

				Datum s{ "Hello"s, "Hi"s, "Henlo"s };
				index = s.Remove("Hello"s);
				Assert::AreEqual(s.Size(), 2_z);

				Datum v{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				index = v.Remove(glm::vec4(0));
				Assert::AreEqual(v.Size(), 2_z);

				Datum m{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				index = m.Remove(glm::mat4(0));
				Assert::AreEqual(m.Size(), 2_z);

				Foo a(10), b(20), c(30);
				RTTI* a_rtti = &a;
				RTTI* b_rtti = &b;
				RTTI* c_rtti = &c;
				const RTTI* c_a_rtti = &a;
				Datum p{ a_rtti, b_rtti, c_rtti };
				index = p.Remove(c_a_rtti);
				Assert::AreEqual(p.Size(), 2_z);
			}
		}

		TEST_METHOD(TestSetStorage)
		{
			int i[] = { 10, 20, 30 };
			float f[] = { 10.f, 20.f, 30.f };
			std::string s[] = { "Hello"s, "Hi"s, "Henlo"s };
			glm::vec4 v[] = { glm::vec4(0), glm::vec4(1), glm::vec4(2) };
			glm::mat4 m[] = { glm::mat4(0), glm::mat4(1), glm::mat4(2) };
			Foo a;
			RTTI* p[3] = { &a, &a, &a };

			int iScalar = 10;
			float fScalar = 10.f;
			std::string sScalar = "Hello"s;
			glm::vec4 vScalar = glm::vec4(0);
			glm::mat4 mScalar = glm::mat4(0);
			RTTI* pScalar = &a;

			{
				{
					Datum d;
					d.SetStorage(i, 3_z);
					Assert::ExpectException<exception>([&d, &i] { d.SetStorage(i, 0_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d] { d.Resize(100_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d] { d.Reserve(100_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d] { d.Clear(); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d] { d.PopBack(); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d] { d.RemoveAt(0_z); }, L"Expected an exception, but none was thrown");

					Assert::ExpectException<exception>([&d] { d.PushBack(0); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d] { d.PushBack(0.f); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d] { d.PushBack(""); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d] { d.PushBack(glm::vec4(0)); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d] { d.PushBack(glm::mat4(0)); }, L"Expected an exception, but none was thrown");

					Assert::ExpectException<exception>([&d, &iScalar] { d.PushBack(iScalar); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d, &fScalar] { d.PushBack(fScalar); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d, &sScalar] { d.PushBack(sScalar); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d, &vScalar] { d.PushBack(vScalar); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d, &mScalar] { d.PushBack(mScalar); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&d, &pScalar] { d.PushBack(pScalar); }, L"Expected an exception, but none was thrown");

					Datum intDatum;
					intDatum.SetType(Datum::DatumTypes::Integer);
					Datum floatDatum;
					floatDatum.SetType(Datum::DatumTypes::Float);

					Assert::ExpectException<exception>([&floatDatum, &i] { floatDatum.SetStorage(i, 0_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &f] { intDatum.SetStorage(f, 0_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &s] { intDatum.SetStorage(s, 0_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &v] { intDatum.SetStorage(v, 0_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &m] { intDatum.SetStorage(m, 0_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &p] { intDatum.SetStorage(p, 0_z); }, L"Expected an exception, but none was thrown");

					Assert::ExpectException<exception>([&floatDatum, &i] { floatDatum.SetStorage(i, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &f] { intDatum.SetStorage(f, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &s] { intDatum.SetStorage(s, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &v] { intDatum.SetStorage(v, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &m] { intDatum.SetStorage(m, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&intDatum, &p] { intDatum.SetStorage(p, 3_z); }, L"Expected an exception, but none was thrown");
				}

				{
					Datum intDatum{ 10, 20, 30 };
					Datum floatDatum{ 10.f, 20.f, 30.f };
					Datum stringDatum{ "Hello"s, "Hi"s, "Henlo"s };
					Datum vectorDatum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
					Datum matrixDatum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
					Datum pointerDatum{ &a, &a, &a };

					Assert::ExpectException<exception>([&intDatum, &i] { intDatum.SetStorage(i, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&floatDatum, &f] { floatDatum.SetStorage(f, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&stringDatum, &s] { stringDatum.SetStorage(s, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&vectorDatum, &v] { vectorDatum.SetStorage(v, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&matrixDatum, &m] { matrixDatum.SetStorage(m, 3_z); }, L"Expected an exception, but none was thrown");
					Assert::ExpectException<exception>([&pointerDatum, &p] { pointerDatum.SetStorage(p, 3_z); }, L"Expected an exception, but none was thrown");
				}
			}

			{
				Datum d1, d2, d3, d4, d5, d6;
				d1.SetStorage(i, 3_z);
				d2.SetStorage(f, 3_z);
				d3.SetStorage(s, 3_z);
				d4.SetStorage(v, 3_z);
				d5.SetStorage(m, 3_z);
				d6.SetStorage(p, 3_z);

				Datum dCopy(d1);
				Datum dAssign;
				dAssign = d2;
			}
		}



	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DatumTests::sStartMemState;
}