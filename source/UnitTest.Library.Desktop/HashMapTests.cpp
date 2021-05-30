#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "HashMap.h"
#include "Vector.h"
#include "Foo.h"
#include "Bar.h"
#include "SizeLiteral.h"

using namespace FIEAGameEngine;
using namespace UnitTests;
using namespace std;

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

	template<>
	struct DefaultHash<Foo> final
	{
		size_t operator()(const Foo& key) const
		{
			return key.Data();
		}
	};
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<HashMap<Foo, int>::PairType>(const HashMap<Foo, int>::PairType& t)
	{
		try
		{
			std::wstringstream s;
			s << L"("s << ToString(t.first) << L", "s << t.second << L")"s;
			return s.str();
		}
		catch (...)
		{
			return L"end()";
		}
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, int>::PairType>(const HashMap<Bar, int>::PairType& t)
	{
		try
		{
			std::wstringstream s;
			s << L"("s << ToString(t.first) << L", "s << t.second << L")"s;
			return s.str();
		}
		catch (...)
		{
			return L"end()";
		}
	}

	template<>
	inline std::wstring ToString<HashMap<const char*, const char*>::PairType>(const HashMap<const char*, const char*>::PairType& t)
	{
		try
		{
			std::wstringstream s;
			s << L"("s << ToString(t.first) << L", "s << t.second << L")"s;
			return s.str();
		}
		catch (...)
		{
			return L"end()";
		}
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, int>::Iterator>(const HashMap<Foo, int>::Iterator& t)
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

	template<>
	inline std::wstring ToString<HashMap<Foo, int>::ConstIterator>(const HashMap<Foo, int>::ConstIterator& t)
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

	template<>
	inline std::wstring ToString<HashMap<Bar, int>::Iterator>(const HashMap<Bar, int>::Iterator& t)
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

	template<>
	inline std::wstring ToString<HashMap<Bar, int>::ConstIterator>(const HashMap<Bar, int>::ConstIterator& t)
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

	template<>
	inline std::wstring ToString<HashMap<const char*, const char*>::Iterator>(const HashMap<const char*, const char*>::Iterator& t)
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

	template<>
	inline std::wstring ToString<HashMap<const char*, const char*>::ConstIterator>(const HashMap<const char*, const char*>::ConstIterator& t)
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

namespace UnitTestLibraryDesktop
{
	struct CustomHashFunction final
	{
		template <typename TKey>
		size_t operator()(const TKey& key) const
		{
			return hash<TKey>(key);
		}
	};

	TEST_CLASS(HashMapTests)
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


#pragma region IteratorTests

		TEST_METHOD(IteratorConstructorAndOperators)
		{
			// Iterator
			{
				// Construtor
				HashMap<Foo, int> hashMap(5_z);
				HashMap<Foo, int>::Iterator itConstructor = hashMap.begin();

				// Equality
				Assert::AreEqual(itConstructor, hashMap.end());
				auto entry = make_pair(Foo(10), 10);
				hashMap.Insert(entry);
				Assert::AreNotEqual(itConstructor, hashMap.begin());

				// ++ operator
				HashMap<Foo, int>::Iterator itPlusPlus;
				Assert::ExpectException<exception>([&itPlusPlus] { itPlusPlus++; }, L"Expected an exception, but none was thrown");

				hashMap.Insert(make_pair(Foo(20), 20));
				hashMap.Insert(make_pair(Foo(21), 21));
				hashMap.Insert(make_pair(Foo(30), 30));
				hashMap.Insert(make_pair(Foo(50), 50));

				for (itPlusPlus = hashMap.begin(); itPlusPlus != hashMap.end(); ++itPlusPlus)
				{
					;
				}

				for (itPlusPlus = hashMap.begin(); itPlusPlus != hashMap.end(); itPlusPlus++)
				{
					;
				}
				itPlusPlus++;

				// * operator
				HashMap<Foo, int>::Iterator itStar;
				Assert::ExpectException<exception>([&itStar] { *itStar; }, L"Expected an exception, but none was thrown");
				for (itStar = hashMap.begin(); itStar != hashMap.end(); ++itStar)
				{
					entry = *itStar;
				}
				Assert::ExpectException<exception>([&itStar] { *itStar; }, L"Expected an exception, but none was thrown");

				// -> operator
				HashMap<Foo, int>::Iterator itArrow;
				Assert::ExpectException<exception>([&itArrow] { itArrow->first; }, L"Expected an exception, but none was thrown");
				for (itArrow = hashMap.begin(); itArrow != hashMap.end(); ++itArrow)
				{
					int data = itArrow->second;
					data;
				}
				Assert::ExpectException<exception>([&itArrow] { itArrow->first; }, L"Expected an exception, but none was thrown");

			}


			// ConstIterator
			{
				// Construtor
				HashMap<Foo, int> hashMap(5_z);

				hashMap.Insert(make_pair(Foo(10), 20));
				hashMap.Insert(make_pair(Foo(20), 20));
				hashMap.Insert(make_pair(Foo(21), 21));
				hashMap.Insert(make_pair(Foo(30), 30));
				hashMap.Insert(make_pair(Foo(50), 50));
				const HashMap<Foo, int> constHashMap(hashMap);
				HashMap<Foo, int>::ConstIterator itPromote = hashMap.begin();
				HashMap<Foo, int>::ConstIterator constItConstructor = constHashMap.begin();

				// Equality
				Assert::AreEqual(constItConstructor, constHashMap.begin());
				Assert::AreNotEqual(constItConstructor, constHashMap.end());

				// ++ operator
				HashMap<Foo, int>::ConstIterator constItPlusPlus;
				Assert::ExpectException<exception>([&constItPlusPlus] { constItPlusPlus++; }, L"Expected an exception, but none was thrown");



				for (constItPlusPlus = constHashMap.begin(); constItPlusPlus != constHashMap.end(); ++constItPlusPlus)
				{
					;
				}

				for (constItPlusPlus = constHashMap.begin(); constItPlusPlus != constHashMap.end(); constItPlusPlus++)
				{
					;
				}

				constItPlusPlus++;

				// * operator
				HashMap<Foo, int>::ConstIterator constItStar;
				Assert::ExpectException<exception>([&constItStar] { *constItStar; }, L"Expected an exception, but none was thrown");
				for (constItStar = constHashMap.begin(); constItStar != constHashMap.end(); ++constItStar)
				{
					auto entry = *constItStar;
				}
				Assert::ExpectException<exception>([&constItStar] { *constItStar; }, L"Expected an exception, but none was thrown");

				// -> operator
				HashMap<Foo, int>::ConstIterator constItArrow;
				Assert::ExpectException<exception>([&constItArrow] { constItArrow->first; }, L"Expected an exception, but none was thrown");
				for (constItArrow = constHashMap.begin(); constItArrow != constHashMap.end(); ++constItArrow)
				{
					int data = constItArrow->second;
					data;
				}
				Assert::ExpectException<exception>([&constItArrow] { constItArrow->first; }, L"Expected an exception, but none was thrown");
			}
		}


#pragma endregion

#pragma region HashTests
		TEST_METHOD(IntegerHash)
		{
			int a = 10;
			int b = 20;
			int c = 10;
			DefaultHash<int> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(StringHash)
		{
			string a = "Hello"s;
			string b = "Goodbye"s;
			string c(a);
			DefaultHash<string> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(WideStringHash)
		{
			wstring a = L"Hello"s;
			wstring b = L"Goodbye"s;
			wstring c(a);
			DefaultHash<wstring> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(CStringHash)
		{
			const char* a = "Hello";
			const char* b = "Goodbye";
			char c[6];
			strcpy_s(c, a);
			DefaultHash<char*> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(FooHash)
		{
			Foo a(10);
			Foo b(20);
			Foo c(10);

			DefaultHash<Foo> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}
#pragma endregion

#pragma region HashMapTests
		TEST_METHOD(TestConstructor)
		{
			// Test Default Constructor
			{
				const size_t defaultBucketSize = 11;
				HashMap<Foo, int> hashMap;
				Assert::AreEqual(0_z, hashMap.Size());
				Assert::AreEqual(defaultBucketSize, hashMap.BucketSize());
				Assert::AreEqual(hashMap.begin(), hashMap.end());
			}
			
			// Test Constructor with custom size
			{
				const size_t bucketSize = 5;
				HashMap<Foo, int> hashMap(bucketSize);
				Assert::AreEqual(0_z, hashMap.Size());
				Assert::AreEqual(bucketSize, hashMap.BucketSize());
				Assert::AreEqual(hashMap.begin(), hashMap.end());
			}

			// Test Constructor with custom functors
			{
				function<bool(const Foo& lhs, const Foo& rhs)> eq = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data();  };
				function<size_t(const Foo& key)> hf = [](const Foo& key) { return key.Data(); };


				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				const size_t bucketSize = 5;
				HashMap<Foo, int> hashMap(bucketSize, hf, eq);
				Assert::AreEqual(0_z, hashMap.Size());
				Assert::AreEqual(bucketSize, hashMap.BucketSize());
				Assert::AreEqual(hashMap.begin(), hashMap.end());

				auto entry = make_pair(a, a.Data());
				auto [it, valueInserted] = hashMap.Insert(entry);
				Assert::IsTrue(valueInserted);
				Assert::AreEqual(1_z, hashMap.Size());
				Assert::AreEqual(a, it->first);

				tie(it, valueInserted) = hashMap.Insert(make_pair(b, b.Data()));
				Assert::IsTrue(valueInserted);
				Assert::AreEqual(2_z, hashMap.Size());
				Assert::AreEqual(b, it->first);

				tie(it, valueInserted) = hashMap.Insert(make_pair(c, c.Data()));
				Assert::IsTrue(valueInserted);
				Assert::AreEqual(3_z, hashMap.Size());
				Assert::AreEqual(c, it->first);
			}

			// Test Constructor Exceptions
			{
				Assert::ExpectException<exception>([] { HashMap<Foo, int> hashMap(0_z); }, L"Expected an exception, but none was thrown");

				function<bool(const Foo& lhs, const Foo& rhs)> eq = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data();  };
				function<size_t(const Foo& key)> hf = [](const Foo& key) { return key.Data(); };
				const size_t bucketSize = 0_z;
				Assert::ExpectException<exception>([&bucketSize, &hf, &eq] { HashMap<Foo, int> hashMap(bucketSize, hf, eq); }, L"Expected an exception, but none was thrown");
				
			}

		}

		TEST_METHOD(TestSpecialMembers)
		{
			// Test Copy Constructor
			{
				const size_t bucketSize = 5;
				HashMap<Foo, int> hashMap1(bucketSize);
				HashMap<Foo, int> hashMap2(hashMap1);
				Assert::AreEqual(0_z, hashMap2.Size());
				Assert::AreEqual(bucketSize, hashMap2.BucketSize());
			}
			// Test Move Constructor
			{
				const size_t bucketSize = 5;
				HashMap<Foo, int> hashMap1(bucketSize);
				HashMap<Foo, int> hashMap2(move(hashMap1));
				Assert::AreEqual(0_z, hashMap2.Size());
				Assert::AreEqual(bucketSize, hashMap2.BucketSize());
			}
			// Test Copy Assignment
			{
				const size_t bucketSize = 5;
				HashMap<Foo, int> hashMap1(bucketSize);
				HashMap<Foo, int> hashMap2 = move(hashMap1);
				Assert::AreEqual(0_z, hashMap2.Size());
				Assert::AreEqual(bucketSize, hashMap2.BucketSize());
			}
			// Test Move Assignment
			{
				const size_t bucketSize = 5;
				HashMap<Foo, int> hashMap1(bucketSize);
				HashMap<Foo, int> hashMap2 = move(hashMap1);
				Assert::AreEqual(0_z, hashMap2.Size());
				Assert::AreEqual(bucketSize, hashMap2.BucketSize());
			}
		}

		TEST_METHOD(InitializerListConstructor)
		{
			auto a = make_pair(Foo(10), 10);
			auto b = make_pair(Foo(20), 20);
			auto c = make_pair(Foo(30), 30);

			HashMap<Foo, int> hashMap{ a, b, c };
			Assert::AreEqual(3_z, hashMap.Size());
			Assert::AreNotEqual(hashMap.end(), hashMap.Find(a.first));
			Assert::AreNotEqual(hashMap.end(), hashMap.Find(b.first));
			Assert::AreNotEqual(hashMap.end(), hashMap.Find(c.first));
		}

		TEST_METHOD(TestBracketOperators)
		{
			// ++ operator
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				HashMap<Foo, int> hashMap(5_z);

				hashMap.Insert(make_pair(a, 10));
				hashMap.Insert(make_pair(b, 20));
				hashMap.Insert(make_pair(c, 30));

				int aData = hashMap[a];
				Assert::AreEqual(aData, a.Data());

				int noData = hashMap[Foo(100)];
				Assert::IsTrue(hashMap.ContainsKey(Foo(100)));
				Assert::AreEqual(noData, int());
			}
		}

		TEST_METHOD(TestFind)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			HashMap<Foo, int> hashMap(5_z);

			hashMap.Insert(make_pair(a, 10));
			hashMap.Insert(make_pair(b, 20));
			hashMap.Insert(make_pair(c, 30));

			// Find
			HashMap<Foo, int>::Iterator it = hashMap.Find(a);
			Assert::AreEqual(a, it->first);
			Assert::AreEqual(a.Data(), it->second);

			const Foo y(100);
			it = hashMap.Find(y);
			Assert::AreEqual(it, hashMap.end());

			// Const Find
			const HashMap<Foo, int> constHashMap(hashMap);
			HashMap<Foo, int>::ConstIterator constIt = constHashMap.Find(a);
			Assert::AreEqual(a, constIt->first);
			Assert::AreEqual(a.Data(), constIt->second);

			const Foo z(100);
			constIt = constHashMap.Find(z);
			Assert::AreEqual(constIt, constHashMap.end());
		}

		TEST_METHOD(TestInsert)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo d(10);

			HashMap<Foo, int> hashMap;
			auto entry = make_pair(a, a.Data());
			auto [it, valueInserted] = hashMap.Insert(entry);
			Assert::IsTrue(valueInserted);
			Assert::AreEqual(1_z, hashMap.Size());
			Assert::AreEqual(a, it->first);

			tie(it, valueInserted) = hashMap.Insert(make_pair(b, b.Data()));
			Assert::IsTrue(valueInserted);
			Assert::AreEqual(2_z, hashMap.Size());
			Assert::AreEqual(b, it->first);

			tie(it, valueInserted) = hashMap.Insert(make_pair(c, c.Data()));
			Assert::IsTrue(valueInserted);
			Assert::AreEqual(3_z, hashMap.Size());
			Assert::AreEqual(c, it->first);

			tie(it, valueInserted) = hashMap.Insert(make_pair(a, a.Data()));
			Assert::IsFalse(valueInserted);
			Assert::AreEqual(3_z, hashMap.Size());
			Assert::AreEqual(a, it->first);
		}

		TEST_METHOD(TestRemove)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo z(100);

			HashMap<Foo, int> hashMap(5_z);

			hashMap.Insert(make_pair(a, 10));
			hashMap.Insert(make_pair(b, 20));
			hashMap.Insert(make_pair(c, 30));
			Assert::AreEqual(3_z, hashMap.Size());

			hashMap.Remove(a);
			Assert::AreEqual(2_z, hashMap.Size());
			Assert::IsFalse(hashMap.ContainsKey(a));

			hashMap.Remove(z);
			Assert::AreEqual(2_z, hashMap.Size());

			hashMap.Remove(b);
			Assert::AreEqual(1_z, hashMap.Size());
			Assert::IsFalse(hashMap.ContainsKey(b));

			hashMap.Remove(c);
			Assert::AreEqual(0_z, hashMap.Size());
			Assert::IsFalse(hashMap.ContainsKey(c));
		}

		TEST_METHOD(TestClear)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			HashMap<Foo, int> hashMap(5_z);

			hashMap.Insert(make_pair(a, 10));
			hashMap.Insert(make_pair(b, 20));
			hashMap.Insert(make_pair(c, 30));
			Assert::AreEqual(3_z, hashMap.Size());

			hashMap.Clear();
			Assert::AreEqual(0_z, hashMap.Size());

			hashMap.Insert(make_pair(a, 10));
			hashMap.Insert(make_pair(b, 20));
			hashMap.Insert(make_pair(c, 30));
			Assert::AreEqual(3_z, hashMap.Size());
		}

		TEST_METHOD(TestIsEmpty)
		{
			HashMap<Foo, int> hashMap;
			Assert::IsTrue(hashMap.IsEmpty());

			Foo a(10);
			hashMap[a] = a.Data();
			Assert::IsFalse(hashMap.IsEmpty());
		}

		TEST_METHOD(TestSize)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			HashMap<Foo, int> hashMap(5_z);

			hashMap.Insert(make_pair(a, 10));
			hashMap.Insert(make_pair(b, 20));
			hashMap.Insert(make_pair(c, 30));
			Assert::AreEqual(3_z, hashMap.Size());
			Assert::AreEqual(5_z, hashMap.BucketSize());
		}

		TEST_METHOD(TestContainsKey)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo z(100);

			HashMap<Foo, int> hashMap(5_z);

			hashMap.Insert(make_pair(a, 10));
			hashMap.Insert(make_pair(b, 20));
			hashMap.Insert(make_pair(c, 30));
			Assert::IsTrue(hashMap.ContainsKey(a));
			Assert::IsFalse(hashMap.ContainsKey(z));
		}

		TEST_METHOD(TestAt)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			HashMap<Foo, int> hashMap(5_z);

			hashMap.Insert(make_pair(a, 10));
			hashMap.Insert(make_pair(b, 20));
			hashMap.Insert(make_pair(c, 30));

			HashMap<Foo, int>::Iterator it = hashMap.begin();
			int cData = hashMap.At(c);
			Assert::AreEqual(30, cData);
			Assert::ExpectException<exception>([&hashMap] { hashMap.At(Foo(100)); }, L"Expected an exception, but none was thrown");

			const HashMap<Foo, int> constHashMap(hashMap);
			HashMap<Foo, int>::ConstIterator constIt = constHashMap.begin();
			int bData = constHashMap.At(b);
			Assert::AreEqual(20, bData);
			Assert::ExpectException<exception>([&constHashMap] { constHashMap.At(Foo(100)); }, L"Expected an exception, but none was thrown");
		}

		TEST_METHOD(TestBeginEnd)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			HashMap<Foo, int> hashMap(5_z);
			Assert::AreEqual(hashMap.begin(), hashMap.end());
			hashMap.Insert(make_pair(a, 10));
			Assert::AreEqual(hashMap.begin()->first,a);
			Assert::AreNotEqual(hashMap.begin(), hashMap.end());
			hashMap.Insert(make_pair(b, 20));
			Assert::AreEqual(hashMap.begin()->first, b);
			hashMap.Insert(make_pair(c, 30));
			Assert::AreEqual(hashMap.begin()->first, c);

			const HashMap<Foo, int> emptyConstHashMap;
			Assert::AreEqual(emptyConstHashMap.begin(), emptyConstHashMap.end());
			Assert::AreEqual(emptyConstHashMap.cbegin(), emptyConstHashMap.cend());


			const HashMap<Foo, int> constHashMap(hashMap);
			Assert::AreNotEqual(constHashMap.begin(), constHashMap.end());
			Assert::AreEqual(constHashMap.begin()->first, c);

			Assert::AreNotEqual(constHashMap.cbegin(), constHashMap.cend());
			Assert::AreEqual(constHashMap.cbegin()->first, c);
		}

		TEST_METHOD(TestResize)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			HashMap<Foo, int> hashMap(3_z);
			hashMap.Insert(make_pair(a, 10));
			hashMap.Insert(make_pair(b, 20));
			hashMap.Insert(make_pair(c, 30));

			hashMap.Resize(15_z);
			Assert::AreEqual(hashMap.Size(), 3_z);
			Assert::AreEqual(hashMap.BucketSize(), 15_z);
		}
#pragma endregion

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState HashMapTests::sStartMemState;
}
