#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "Vector.h"
#include "Foo.h"
#include "Bar.h"
#include "SizeLiteral.h"

using namespace FIEAGameEngine;
using namespace UnitTests;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
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
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
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
	inline std::wstring ToString<Vector<Bar>::Iterator>(const Vector<Bar>::Iterator& t)
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
	inline std::wstring ToString<Vector<Bar>::ConstIterator>(const Vector<Bar>::ConstIterator& t)
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
	inline std::wstring ToString<Vector<const char*>::Iterator>(const Vector<const char*>::Iterator& t)
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
	inline std::wstring ToString<Vector<const char*>::ConstIterator>(const Vector<const char*>::ConstIterator& t)
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
	struct CustomIncrementStrategy final
	{
		size_t operator()(size_t /*size*/, size_t capacity) const
		{
			return capacity + 5_z;
		}
	};


	TEST_CLASS(VectorTests)
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
		/// <summary>
		/// Tests the special members of the Vector Iterator class.
		/// </summary>
		TEST_METHOD(TestIteratorSpecialMembers)
		{
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				Vector<Foo> vector;
				Vector<Foo> vector2;

				vector.PushBack(a);
				vector.PushBack(b);
				vector.PushBack(c);
				vector.PushBack(Foo(10));

				vector2.PushBack(a);
				vector2.PushBack(b);
				vector2.PushBack(c);


				// Test for Iterator Special members
				{
					Vector<Foo>::Iterator it;// Default constructed
					it = vector.begin(); // Invokes copy assignment
					for (; it != vector.end(); ++it)
					{
						Foo& foo = *it;
						foo.Data() = 10;
					}

					Vector<Foo>::Iterator it2(vector.begin()); // Invokes copy constructor
					for (; it2 != vector.end(); ++it2)
					{
						Foo& foo = *it2;
						foo.Data() = 10;
					}

					Vector<Foo>::Iterator it3; // Default constructed
					Vector<Foo>::Iterator tempBegin = vector.begin(); // Temporary iterator
					it3 = move(tempBegin); // Invokes move assignment
					for (; it3 != vector.end(); ++it3)
					{
						Foo& foo = *it3;
						foo.Data() = 10;
					}

					tempBegin = vector.begin();
					Vector<Foo>::Iterator it4(move(tempBegin)); // Invokes move constructor
					for (; it4 != vector.end(); ++it4)
					{
						Foo& foo = *it4;
						foo.Data() = 10;
					}
				}

				// Test for ConstIterator Special Members
				{
					Vector<Foo>::ConstIterator constIt; // Default constructed
					constIt = vector.cbegin();  // Invokes copy assignment
					for (; constIt != vector.end(); ++constIt)
					{
						const Foo& foo = *constIt;
						foo;
					}

					Vector<Foo>::ConstIterator constIt2(constIt); // Invokes copy constructor
					for (; constIt2 != vector.end(); ++constIt2)
					{
						const Foo& foo = *constIt2;
						foo;
					}

					Vector<Foo>::ConstIterator constIt3;
					Vector<Foo>::ConstIterator tempBegin = vector.begin(); // Temporary iterator
					constIt3 = move(tempBegin); // Invokes move assignment
					for (; constIt3 != vector.end(); ++constIt3)
					{
						const Foo& foo = *constIt3;
						foo;
					}

					tempBegin = vector.cbegin(); // Temporary iterator
					Vector<Foo>::ConstIterator constIt4(move(tempBegin)); // Invokes move constructor
					for (; constIt4 != vector.end(); ++constIt4)
					{
						const Foo& foo = *constIt4;
						foo;
					}
				}

				// Testing promoting a non-const Iterator to a ConstIterator
				{
					Vector<Foo>::Iterator it = vector.begin();
					Vector<Foo>::ConstIterator constIt(it);
					for (; constIt != vector.end(); ++constIt)
					{
						const Foo& foo = *constIt;
						foo;
					}
				}
			}
		}

		/// <summary>
		/// Tests all the overloaded operators for both the Iterator and ConstIterator classes.
		/// </summary>
		TEST_METHOD(TestIteratorOperators)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> vector1;
			Vector<Foo> vector2;
			Vector<Foo> emptyVector;

			vector1.PushBack(a);
			vector1.PushBack(b);
			vector1.PushBack(c);

			vector2.PushBack(a);
			vector2.PushBack(b);
			vector2.PushBack(c);

			// Testing equality and not equality operators on Vector Iterator
			{
				Vector<Foo>::Iterator it1;
				Vector<Foo>::Iterator it2;
				Vector<Foo>::Iterator it3;
				it1 = vector1.begin();
				it2 = vector1.begin();
				it3 = vector1.end();

				Assert::AreEqual(it1, it2);
				Assert::AreNotEqual(it1, it3);

				it2 = move(it1);

				Vector<Foo>::Iterator it4;
				it4 = move(it2);
				Assert::AreEqual(vector1.begin(), it4);
			}

			// Testing equality and not equality operators on Vector ConstIterator
			{
				Vector<Foo>::ConstIterator it1;
				Vector<Foo>::ConstIterator it2;
				Vector<Foo>::ConstIterator it3;
				it1 = vector1.begin();
				it2 = vector1.begin();
				it3 = vector1.end();

				Assert::AreEqual(it1, it2);
				Assert::AreNotEqual(it1, it3);

				it2 = move(it1);

				Vector<Foo>::ConstIterator it4;
				it4 = move(it2);
				Assert::AreEqual(vector1.cbegin(), it4);
				Assert::IsTrue(it4 != vector1.cend());
			}

			// Testing Prefix Increment, Postfix Increment, and Dereference Operator on Vector Iterator
			{
				Vector<Foo>::Iterator it;
				Assert::ExpectException<exception>([&it] { it++; }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&it] { it--; }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&it] { *it; }, L"Expected an exception, but none was thrown");

				it = vector1.begin();
				Assert::AreEqual(*it, Foo(10));
				++it;
				Assert::AreEqual(*it, Foo(20));
				it++;
				Assert::AreEqual(*it, Foo(30));
				--it;
				Assert::AreEqual(*it, Foo(20));
				it--;
				Assert::AreEqual(*it, Foo(10));
			}

			// Testing Prefix Increment, Postfix Increment, and Dereference Operator on Vector ConstIterator
			{
				Vector<Foo>::ConstIterator it;
				Assert::ExpectException<exception>([&it] { it++; }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&it] { it--; }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&it] { *it; }, L"Expected an exception, but none was thrown");

				it = emptyVector.begin();
				Assert::ExpectException<exception>([&it] { *it; }, L"Expected an exception, but none was thrown");

				it = vector1.cbegin();
				Assert::AreEqual(*it, Foo(10));
				++it;
				Assert::AreEqual(*it, Foo(20));
				it++;
				Assert::AreEqual(*it, Foo(30));
				--it;
				Assert::AreEqual(*it, Foo(20));
				it--;
				Assert::AreEqual(*it, Foo(10));
			}

			// Testing addition and subtraction operators on Vector Iterator
			{
				Vector<Foo>::Iterator addIt;
				Assert::ExpectException<exception>([&addIt] { addIt + 5; }, L"Expected an exception, but none was thrown");
				Vector<Foo>::ConstIterator caddIt;
				Assert::ExpectException<exception>([&caddIt] {caddIt + 5; }, L"Expected an exception, but none was thrown");

				addIt = vector1.begin();
				addIt + 2;
				Assert::AreEqual(*addIt, Foo(10));
				addIt = addIt + 2;
				Assert::AreEqual(*addIt, Foo(30));


				Vector<Foo>::Iterator subIt;
				Assert::ExpectException<exception>([&subIt] { subIt - 5; }, L"Expected an exception, but none was thrown");
				Vector<Foo>::ConstIterator csubIt;
				Assert::ExpectException<exception>([&csubIt] {csubIt - 5; }, L"Expected an exception, but none was thrown");

				subIt = vector1.end();
				subIt - 3;
				Assert::AreEqual(subIt, vector1.end());
				subIt = subIt - 3;
				Assert::AreEqual(*subIt, Foo(10));
			}

			// Testing addition and subtraction operators on Vector ConstIterator
			{
				Vector<Foo>::ConstIterator addIt;
				addIt = vector1.begin();
				addIt + 2;
				Assert::AreEqual(*addIt, Foo(10));
				addIt = addIt + 2;
				Assert::AreEqual(*addIt, Foo(30));


				Vector<Foo>::ConstIterator subIt;
				subIt = vector1.end();
				subIt - 3;
				Assert::AreEqual(subIt, vector1.cend());
				subIt = subIt - 3;
				Assert::AreEqual(*subIt, Foo(10));
			}
		}

		/// <summary>
		/// Tests the Find method of the Vector Iterator.
		/// </summary>
		TEST_METHOD(TestIteratorFind)
		{
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				Vector<Foo> vector;
				Assert::AreEqual(vector.end(), vector.Find(a));
				vector.PushBack(a);
				vector.PushBack(b);
				vector.PushBack(c);
				Assert::AreNotEqual(vector.end(), vector.Find(a));
				vector.Clear();
				Assert::AreEqual(vector.end(), vector.Find(a));
				vector.PushBack(a);
				vector.PushBack(b);
				vector.PushBack(c);
				Assert::AreNotEqual(vector.end(), vector.Find(b));
				Assert::AreNotEqual(vector.end(), vector.Find(c));
			}
			
			{
				const Bar a(10);
				const Bar b(20);
				const Bar c(30);

				Vector<Bar> vector;
				Assert::AreEqual(vector.end(), vector.Find<DefaultEquality<Bar>>(a));
				vector.PushBack(a);
				Assert::AreNotEqual(vector.end(), vector.Find<DefaultEquality<Bar>>(a));
				vector.Clear();
				Assert::AreEqual(vector.end(), vector.Find<DefaultEquality<Bar>>(a));
				vector.PushBack(a);
				vector.PushBack(b);
				vector.PushBack(c);
				Assert::AreNotEqual(vector.end(), vector.Find<DefaultEquality<Bar>>(b));
				Assert::AreNotEqual(vector.end(), vector.Find<DefaultEquality<Bar>>(c));
			}

			{
				const char* a("A");
				const char* b("B");
				const char* c("C");

				char testA[2];

				strcpy_s(testA, a);

				Vector<const char*> vector;
				Assert::AreEqual(vector.end(), vector.Find<DefaultEquality<char*>>(testA));
				vector.PushBack(a);
				Assert::AreNotEqual(vector.end(), vector.Find<DefaultEquality<char*>>(testA));
				vector.Clear();
				Assert::AreEqual(vector.end(), vector.Find<DefaultEquality<char*>>(testA));
				vector.PushBack(a);
				vector.PushBack(b);
				vector.PushBack(c);
				Assert::AreNotEqual(vector.end(), vector.Find<DefaultEquality<char*>>(b));
				Assert::AreNotEqual(vector.end(), vector.Find<DefaultEquality<char*>>(c));
			}
		}


		/// <summary>
		/// Tests the remove method of the Vector Iterator.
		/// </summary>
		TEST_METHOD(TestIteratorRemove)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo x(50);
			const Foo y(75);
			const Foo z(100);

			{
				Vector<Foo> vector{ a, b, c };
				Vector<Foo> vector2;
				Vector<Foo>::Iterator it = vector.begin();
				Assert::ExpectException<exception>([&vector2, &it] { vector2.Remove(it); }, L"Expected an exception, but none was thrown");


				Assert::IsTrue(vector.Remove(a));
				Assert::AreEqual(2_z, vector.Size());
				Assert::AreEqual(b, *it);
				Assert::AreEqual(c, *++it);

				Assert::IsFalse(vector.Remove(z));
				Assert::AreEqual(2_z, vector.Size());
				it = vector.begin();
				Assert::AreEqual(b, *it);
				Assert::AreEqual(c, *++it);

				Assert::IsFalse(vector.Remove(vector.end()));
				Assert::AreEqual(2_z, vector.Size());
				it = vector.begin();
				Assert::AreEqual(b, *it);
				Assert::AreEqual(c, *++it);

				Assert::IsTrue(vector.Remove(c));
				Assert::AreEqual(1_z, vector.Size());
				it = vector.begin();
				Assert::AreEqual(b, *it);

				Assert::IsTrue(vector.Remove(b));
				Assert::AreEqual(0_z, vector.Size());
				it = vector.begin();
				Assert::AreEqual(vector.end(), it);
			}

			{
				Vector<Foo> vector{ a, b, c, x, y, z };
				Vector<Foo> vector2;
				Vector<Foo>::Iterator it = vector.begin();
				Assert::ExpectException<exception>([&vector2, &it] { vector2.Remove(it, it); }, L"Expected an exception, but none was thrown");

				Vector<Foo>::Iterator itB = vector.Find(b);
				Vector<Foo>::Iterator itX = vector.Find(x);
				
				Assert::ExpectException<exception>([&vector, &itX, &itB] { vector.Remove(itX, itB); }, L"Expected an exception, but none was thrown");
				Assert::AreEqual(6_z, vector.Size());

				vector.Remove(itB, itX);
				Assert::AreEqual(4_z, vector.Size());
				Assert::AreEqual(a, *it);
				Assert::AreEqual(x, *++it);
				Assert::AreEqual(y, *++it);
				Assert::AreEqual(z, *++it);
				Assert::AreEqual(vector.end(), ++it);

				it = vector.begin();
				Vector<Foo>::Iterator itA = vector.Find(a);
				vector.Remove(itA, itA);
				Assert::AreEqual(3_z, vector.Size());
				Assert::AreEqual(x, *it);
				Assert::AreEqual(y, *++it);
				Assert::AreEqual(z, *++it);
				Assert::AreEqual(vector.end(), ++it);

				it = vector.begin();
				Vector<Foo>::Iterator itZ = vector.Find(z);
				vector.Remove(itZ, itZ);
				Assert::AreEqual(2_z, vector.Size());
				Assert::AreEqual(x, *it);
				Assert::AreEqual(y, *++it);
				Assert::AreEqual(vector.end(), ++it);

				it = vector.begin();
				Vector<Foo>::Iterator itY = vector.Find(y);
				vector.Remove(itY, itY);
				Assert::AreEqual(1_z, vector.Size());
				Assert::AreEqual(x, *it);

				it = vector.begin();
				itX = vector.Find(x);
				vector.Remove(itX, itX);
				Assert::AreEqual(0_z, vector.Size());
				Assert::AreEqual(vector.end(), it);
			}
		}
#pragma endregion

#pragma region VectorTests

		TEST_METHOD(TestResize)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> vector{ a, b, c };
			vector.Resize(0);
		}

		TEST_METHOD(TestVectorSpecialMembers)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			{
				// Default constructor
				Vector<Foo> vector1;
				// Constructor with optional parameter
				Vector<Foo> vector2(10_z);
				Assert::AreEqual(10_z, vector2.Capacity());
			}

			{
				// Copy constructor with initializer vector
				Vector<Foo> vector1{ a, b, c };
				Assert::AreEqual(a, vector1[0]);
				Assert::AreEqual(b, vector1[1]);
				Assert::AreEqual(c, vector1[2]);
				Assert::AreEqual(3_z, vector1.Size());
				// Copy assignment with initializer vector
				Vector<Foo> vector2 = { a, b, c };
				Assert::AreEqual(a, vector2[0]);
				Assert::AreEqual(b, vector2[1]);
				Assert::AreEqual(c, vector2[2]);
				Assert::AreEqual(3_z, vector2.Size());
			}

			{
				Vector<Foo> vector1{ a, b, c };
				// Copy constructor
				Vector<Foo> vector2(vector1);
				Assert::AreEqual(a, vector2[0]);
				Assert::AreEqual(b, vector2[1]);
				Assert::AreEqual(c, vector2[2]);
				Assert::AreEqual(3_z, vector2.Size());

				// Copy assignment
				Vector<Foo> vector3{ a };
				Vector<Foo> vector4{ a, a, b, b, c, c };
				vector3 = vector2;
				Assert::AreEqual(a, vector3[0]);
				Assert::AreEqual(b, vector3[1]);
				Assert::AreEqual(c, vector3[2]);
				Assert::AreEqual(3_z, vector3.Size());

				vector4 = vector2;				
				Assert::AreEqual(a, vector3[0]);
				Assert::AreEqual(b, vector3[1]);
				Assert::AreEqual(c, vector3[2]);
				Assert::AreEqual(3_z, vector3.Size());
			}


			{
				Vector<Foo> vector1{ a, b, c };
				// Move constructor
				Vector<Foo> vector2(move(vector1));
				Assert::AreEqual(a, vector2[0]);
				Assert::AreEqual(b, vector2[1]);
				Assert::AreEqual(c, vector2[2]);
				Assert::AreEqual(3_z, vector2.Size());
				Assert::ExpectException<exception>([&vector1] { vector1.At(0); }, L"Expected an exception, but none was thrown");

				// Move assignment
				Vector<Foo> vector3{ c, b, a };
				vector3 = std::move(vector2);
				Assert::AreEqual(a, vector3[0]);
				Assert::AreEqual(b, vector3[1]);
				Assert::AreEqual(c, vector3[2]);
				Assert::AreEqual(3_z, vector3.Size());
				Assert::ExpectException<exception>([&vector2] { vector2.At(0); }, L"Expected an exception, but none was thrown");
			}
		}

		TEST_METHOD(TestVectorBracketAndAt)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			// Bracket operator
			{
				Vector<Foo> emptyVector;
				const Vector<Foo> constEmptyVector;
				Assert::ExpectException<exception>([&emptyVector] { emptyVector[0]; }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constEmptyVector] { constEmptyVector[0]; }, L"Expected an exception, but none was thrown");

				Vector<Foo> vector{ a, b, c };
				Foo temp = vector[1];
				Assert::AreEqual(b, temp);
				Assert::ExpectException<exception>([&vector] { vector[5]; }, L"Expected an exception, but none was thrown");


				const Vector<Foo> constVector(vector);
				const Foo constTemp = constVector[1];
				Assert::AreEqual(b, temp);
				Assert::ExpectException<exception>([&constVector] { constVector[5]; }, L"Expected an exception, but none was thrown");
			}
			
			// At function
			{
				Vector<Foo> emptyVector;
				const Vector<Foo> constEmptyVector;
				Assert::ExpectException<exception>([&emptyVector] { emptyVector.At(0); }, L"Expected an exception, but none was thrown");
				Assert::ExpectException<exception>([&constEmptyVector] { constEmptyVector.At(0); }, L"Expected an exception, but none was thrown");

				Vector<Foo> vector{ a, b, c };
				Foo temp = vector.At(1);
				Assert::AreEqual(b, temp);
				Assert::ExpectException<exception>([&vector] { vector.At(5); }, L"Expected an exception, but none was thrown");


				const Vector<Foo> constVector(vector);
				const Foo constTemp = constVector.At(1);
				Assert::AreEqual(b, temp);
				Assert::ExpectException<exception>([&constVector] { constVector.At(5); }, L"Expected an exception, but none was thrown");
			}
		}

		/// <summary>
		/// Tests all the begin and end methods of the vector.
		/// </summary>
		TEST_METHOD(IteratorBeginEnd)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> vector;
			Vector<Foo> vector2;

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			vector2.PushBack(a);
			vector2.PushBack(b);
			vector2.PushBack(c);

			Assert::AreNotEqual(vector.begin(), vector.end());

			const Vector<Foo> constvector(vector2);

			Vector<Foo>::Iterator itBegin = vector.begin();
			Vector<Foo>::ConstIterator itCBegin = vector.cbegin();
			Vector<Foo>::ConstIterator constItBegin = constvector.begin();
			Assert::AreEqual(a, *itBegin);
			Assert::AreEqual(a, *itCBegin);
			Assert::AreEqual(a, *constItBegin);


			Vector<Foo>::Iterator itEnd = vector.end();
			Vector<Foo>::ConstIterator itCEnd = vector.cend();
			Vector<Foo>::ConstIterator constItEnd = constvector.end();
			Assert::AreEqual(vector.end(), itEnd);
			Assert::AreEqual(vector.cend(), itCEnd);
			Assert::AreEqual(constvector.end(), constItEnd);
		}

		TEST_METHOD(TestFrontAndBack)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			// Default constructor
			Vector<Foo> vector;
			Assert::ExpectException<exception>([&vector] { vector.Front(); }, L"Expected an exception, but none was thrown");
			Assert::ExpectException<exception>([&vector] { vector.Back(); }, L"Expected an exception, but none was thrown");
			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.Back());

			const Vector<Foo> emptyConstVector;
			Assert::ExpectException<exception>([&emptyConstVector] { emptyConstVector.Front(); }, L"Expected an exception, but none was thrown");
			Assert::ExpectException<exception>([&emptyConstVector] { emptyConstVector.Back(); }, L"Expected an exception, but none was thrown");
			const Vector<Foo> constVector(vector);
			Assert::AreEqual(a, constVector.Front());
			Assert::AreEqual(c, constVector.Back());
		}

		TEST_METHOD(TestPushBack)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			// Default constructor
			Vector<Foo> vector;
			Assert::AreEqual(0_z, vector.Capacity());
			Assert::AreEqual(0_z, vector.Size());
			vector.PushBack(a); // lvalue
			vector.PushBack(Foo(20)); // rvalue
			Assert::AreEqual(2_z, vector.Capacity());
			Assert::AreEqual(2_z, vector.Size());
			Assert::AreEqual(a, vector[0]);
			Assert::AreEqual(b, vector[1]);
		}

		// Also tests isEmpty() and Size()
		TEST_METHOD(TestPopBack)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			Vector<Foo> vector{ a, b, c };
			Assert::AreEqual(3_z, vector.Size());
			Assert::IsFalse(vector.IsEmpty());

			vector.PopBack();
			Assert::AreEqual(2_z, vector.Size());
			Assert::AreEqual(3_z, vector.Capacity());
			Assert::AreEqual(a, vector[0]);
			Assert::AreEqual(b, vector[1]);
			Assert::ExpectException<exception>([&vector] { vector[2]; }, L"Expected an exception, but none was thrown");

			vector.PopBack();
			Assert::AreEqual(1_z, vector.Size());
			Assert::AreEqual(a, vector[0]);
			Assert::ExpectException<exception>([&vector] { vector[1]; }, L"Expected an exception, but none was thrown");
			
			vector.PopBack();
			Assert::IsTrue(vector.IsEmpty());
			Assert::ExpectException<exception>([&vector] { vector[0]; }, L"Expected an exception, but none was thrown");

		}

		TEST_METHOD(TestFind)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo x(100);
			const Foo y(200);
			const Foo z(300);

			// Test on  an empty vector
			{
				Vector<Foo> vector;
				Vector<Foo>::Iterator it = vector.Find(a);
				Assert::AreEqual(it, vector.end());
			}

			// Test on a filled vector
			{
				Vector<Foo> vector{ a, b, c, x, y };
				// Found the value
				Vector<Foo>::Iterator it = vector.Find(c);
				Assert::AreEqual(*it, c);
				// Did not find the value
				it = vector.Find(z);
				Assert::AreEqual(it, vector.end());
			}

			// Test on a filled const vector
			{
				const Vector<Foo> constVector{ a, b, c, x, y };
				// Found the value
				Vector<Foo>::ConstIterator it = constVector.Find(c);
				Assert::AreEqual(*it, c);
				// Did not find the value
				it = constVector.Find(z);
				Assert::AreEqual(it, constVector.end());
			}
		}

		// Also tests Capacity()
		TEST_METHOD(TestReserve)
		{
			Vector<Foo> vector;
			Assert::AreEqual(0_z, vector.Capacity());
			vector.Reserve(100_z);
			Assert::AreEqual(100_z, vector.Capacity());
			vector.Reserve(10_z);
			Assert::AreEqual(100_z, vector.Capacity());
			vector.PushBack(Foo(10));
			vector.Clear();
		}

		TEST_METHOD(TestShrinkToFit)
		{
			Vector<Foo> vector;
			vector.ShrinkToFit();
			Assert::AreEqual(0_z, vector.Capacity());
			vector.Reserve(100_z);
			Assert::AreEqual(100_z, vector.Capacity());
			vector.PushBack(Foo(10));
			vector.PushBack(Foo(20));
			vector.PushBack(Foo(30));
			vector.ShrinkToFit();
			Assert::AreEqual(3_z, vector.Capacity());
		}
#pragma endregion

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTests::sStartMemState;
}
