#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "SList.h"
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
	TEST_CLASS(SListTests)
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
		/// Tests the special members of the SList Iterator class.
		/// </summary>
		TEST_METHOD(TestIteratorSpecialMembers)
		{
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				SList<Foo> list2;

				list.PushBack(a);
				list.PushBack(b);
				list.PushBack(c);
				list.PushBack(Foo(10));

				list2.PushBack(a);
				list2.PushBack(b);
				list2.PushBack(c);


				// Test for Iterator Special members
				{
					SList<Foo>::Iterator it;// Default constructed
					it = list.begin(); // Invokes copy assignment
					for (; it != list.end(); ++it)
					{
						Foo& foo = *it;
						foo.Data() = 10;
					}

					SList<Foo>::Iterator it2(list.begin()); // Invokes copy constructor
					for (; it2 != list.end(); ++it2)
					{
						Foo& foo = *it2;
						foo.Data() = 10;
					}

					SList<Foo>::Iterator it3; // Default constructed
					SList<Foo>::Iterator tempBegin = list.begin(); // Temporary iterator
					it3 = move(tempBegin); // Invokes move assignment
					for (; it3 != list.end(); ++it3)
					{
						Foo& foo = *it3;
						foo.Data() = 10;
					}
					
					tempBegin = list.begin();
					SList<Foo>::Iterator it4(move(tempBegin)); // Invokes move constructor
					for (; it4 != list.end(); ++it4)
					{
						Foo& foo = *it4;
						foo.Data() = 10;
					}
				}

				// Test for ConstIterator Special Members
				{
					SList<Foo>::ConstIterator constIt; // Default constructed
					constIt = list.cbegin();  // Invokes copy assignment
					for (; constIt != list.end(); ++constIt)
					{
						const Foo& foo = *constIt;
						foo;
					}

					SList<Foo>::ConstIterator constIt2(constIt); // Invokes copy constructor
					for (; constIt2 != list.end(); ++constIt2)
					{
						const Foo& foo = *constIt2;
						foo;
					}

					SList<Foo>::ConstIterator constIt3;
					SList<Foo>::ConstIterator tempBegin = list.begin(); // Temporary iterator
					constIt3 = move(tempBegin); // Invokes move assignment
					for (; constIt3 != list.end(); ++constIt3)
					{
						const Foo& foo = *constIt3;
						foo;
					}

					tempBegin = list.cbegin(); // Temporary iterator
					SList<Foo>::ConstIterator constIt4(move(tempBegin)); // Invokes move constructor
					for (; constIt4 != list.end(); ++constIt4)
					{
						const Foo& foo = *constIt4;
						foo;
					}
				}

				// Testing promoting a non-const Iterator to a ConstIterator
				{
					SList<Foo>::Iterator it = list.begin();
					SList<Foo>::ConstIterator constIt(it);
					for (; constIt != list.end(); ++constIt)
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

			SList<Foo> list;
			SList<Foo> list2;

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			list2.PushBack(a);
			list2.PushBack(b);
			list2.PushBack(c);

			// Testing equality and not equality operators on SList Iterator
			{
				SList<Foo>::Iterator it1;
				SList<Foo>::Iterator it2;
				SList<Foo>::Iterator it3;
				it1 = list.begin();
				it2 = list.begin();
				it3 = list.end();

				Assert::AreEqual(it1, it2);
				Assert::AreNotEqual(it1, it3);

				it2 = move(it1);

				SList<Foo>::Iterator it4;
				it4 = move(it2);
				Assert::AreEqual(list.begin(), it4);
			}

			// Testing equality and not equality operators on SList ConstIterator
			{
				SList<Foo>::ConstIterator it1;
				SList<Foo>::ConstIterator it2;
				SList<Foo>::ConstIterator it3;
				it1 = list.begin();
				it2 = list.begin();
				it3 = list.end();

				Assert::AreEqual(it1, it2);
				Assert::AreNotEqual(it1, it3);

				it2 = move(it1);

				SList<Foo>::ConstIterator it4;
				it4 = move(it2);
				Assert::AreEqual(list.cbegin(), it4);
				Assert::IsTrue(it4 != list.cend());
			}

			// Testing Prefix Increment, Postfix Increment, and Dereference Operator on SList Iterator
			{
				SList<Foo>::Iterator it;
				Assert::ExpectException<exception>([&it] { it++; }, L"Expected an exception, but none was thrown");
				it = list.begin();
				Assert::AreEqual(*it, Foo(10));
				++it;
				Assert::AreEqual(*it, Foo(20));
				it++;
				Assert::AreEqual(*it, Foo(30));
			}

			// Testing Prefix Increment, Postfix Increment, and Dereference Operator on SList ConstIterator
			{
				SList<Foo>::ConstIterator it;
				Assert::ExpectException<exception>([&it] { it++; }, L"Expected an exception, but none was thrown");
				it = list.cbegin();
				Assert::AreEqual(*it, Foo(10));
				++it;
				Assert::AreEqual(*it, Foo(20));
				it++;
				Assert::AreEqual(*it, Foo(30));

			}

			{
				SList<Foo>::Iterator it;
				Assert::ExpectException<exception>([&it] { it->Data(); }, L"Expected an exception, but none was thrown");

				SList<Foo>::ConstIterator cit;
				Assert::ExpectException<exception>([&cit] { cit->Data(); }, L"Expected an exception, but none was thrown");
				cit = list.cbegin();
				Assert::AreEqual(a.Data(), cit->Data());
			}
		}

		/// <summary>
		/// Tests all the begin and end methods of the SList Iterator.
		/// </summary>
		TEST_METHOD(IteratorBeginEnd)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			SList<Foo> list;
			SList<Foo> list2;

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			list2.PushBack(a);
			list2.PushBack(b);
			list2.PushBack(c);

			const SList<Foo> constList(list2);
			
			SList<Foo>::Iterator itBegin = list.begin();
			SList<Foo>::ConstIterator itCBegin = list.cbegin();
			SList<Foo>::ConstIterator constItBegin = constList.begin();
			Assert::AreEqual(a, *itBegin);
			Assert::AreEqual(a, *itCBegin);
			Assert::AreEqual(a, *constItBegin);
			
			
			SList<Foo>::Iterator itEnd = list.end();
			SList<Foo>::ConstIterator itCEnd = list.cend();
			SList<Foo>::ConstIterator constItEnd = constList.end();
			Assert::AreEqual(list.end(), itEnd);
			Assert::AreEqual(list.cend(), itCEnd);
			Assert::AreEqual(constList.end(), constItEnd);
		}

		/// <summary>
		/// Tests the Find method of the SList Iterator.
		/// </summary>
		TEST_METHOD(TestIteratorFind)
		{
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				Assert::AreEqual(list.end(), list.Find(a));
				list.PushFront(a);
				Assert::AreNotEqual(list.end(), list.Find(a));
				list.Clear();
				Assert::AreEqual(list.end(), list.Find(a));
				list.PushFront(a);
				list.PushFront(b);
				list.PushFront(c);
				Assert::AreNotEqual(list.end(), list.Find(b));
				Assert::AreNotEqual(list.end(), list.Find(c));

				const SList<Foo> constList(list);
				Assert::AreNotEqual(constList.cend(), constList.Find(a));
			}

			{
				const Bar a(10);
				const Bar b(20);
				const Bar c(30);

				SList<Bar> list;
				Assert::AreEqual(list.end(), list.Find<DefaultEquality<Bar>>(a));
				list.PushFront(a);
				Assert::AreNotEqual(list.end(), list.Find<DefaultEquality<Bar>>(a));
				list.Clear();
				Assert::AreEqual(list.end(), list.Find<DefaultEquality<Bar>>(a));
				list.PushFront(a);
				list.PushFront(b);
				list.PushFront(c);
				Assert::AreNotEqual(list.end(), list.Find<DefaultEquality<Bar>>(b));
				Assert::AreNotEqual(list.end(), list.Find<DefaultEquality<Bar>>(c));
			}

			{
				const char* a("A");
				const char* b("B");
				const char* c("C");

				char testA[2];

				strcpy_s(testA, a);

				SList<const char*> list;
				Assert::AreEqual(list.end(), list.Find<DefaultEquality<char*>>(testA));
				list.PushFront(a);
				Assert::AreNotEqual(list.end(), list.Find<DefaultEquality<char*>>(testA));
				list.Clear();
				Assert::AreEqual(list.end(), list.Find<DefaultEquality<char*>>(testA));
				list.PushBack(a);
				list.PushBack(b);
				list.PushBack(c);
				Assert::AreNotEqual(list.end(), list.Find<DefaultEquality<char*>>(b));
				Assert::AreNotEqual(list.end(), list.Find<DefaultEquality<char*>>(c));
			}
		}

		/// <summary>
		/// Tests the InsertAfter method of the SList Iterator.
		/// </summary>
		TEST_METHOD(TestIteratorInsertAfter)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);

			{
				SList<Foo> list;
				SList<Foo>::Iterator it = list.begin();
				it = list.InsertAfter(a, it);
				it = list.InsertAfter(b, it);
				it = list.InsertAfter(c, it);
				it = list.InsertAfter(a, list.begin());

				Assert::AreEqual(4_z, list.Size());
				it = list.begin();
				Assert::AreEqual(a, *it);
				Assert::AreEqual(a, *++it);
				Assert::AreEqual(b, *++it);
				Assert::AreEqual(c, *++it);

				SList<Foo> list2;
				Assert::ExpectException<exception>([&list2, &a, &it] { list2.InsertAfter(a, it); }, L"Expected an exception, but none was thrown");
			}

			{
				SList<Foo> list;
				SList<Foo>::Iterator it = list.begin();
				for (int i = 0; i < 5; ++i)
				{
					it = list.InsertAfter(a, it);
				}

				Assert::AreEqual(5_z, list.Size());

				it = list.begin();
				for (; it != list.end(); ++it)
				{
					Assert::AreEqual(a, *it);
				}
			}
		}

		/// <summary>
		/// Tests the remove method of the SList Iterator.
		/// </summary>
		TEST_METHOD(TestIteratorRemove)
		{
			const Foo a(10);
			const Foo b(20);
			const Foo c(30);
			const Foo z(100);

			{
				SList<Foo> list;
				SList<Foo> list2;



				SList<Foo>::Iterator it = list.begin();
				Assert::ExpectException<exception>([&list2, &it] { list2.Remove(it); }, L"Expected an exception, but none was thrown");

				it = list.InsertAfter(a, it);
				it = list.InsertAfter(b, it);
				it = list.InsertAfter(c, it);
				Assert::AreEqual(3_z, list.Size());
				
				Assert::IsTrue(list.Remove(b));
				Assert::AreEqual(2_z, list.Size());
				it = list.begin();
				Assert::AreEqual(a, *it);
				Assert::AreEqual(c, *++it);

				Assert::IsTrue(list.Remove(c));
				Assert::AreEqual(1_z, list.Size());
				it = list.begin();
				Assert::AreEqual(a, *it);

				Assert::IsFalse(list.Remove(z));
				Assert::AreEqual(1_z, list.Size());
				it = list.begin();
				Assert::AreEqual(a, *it);

				Assert::IsFalse(list.Remove(list.end()));
				Assert::AreEqual(1_z, list.Size());
				it = list.begin();
				Assert::AreEqual(a, *it);
			}
		}

#pragma endregion		

#pragma region SListTests
		/// /// <summary>
		/// Create a list using default constructor. Test that you can add an element to it and that the size goes up.
		/// </summary>
		TEST_METHOD(TestConstructor)
		{
			{
				SList<Foo> list;
				const Foo f;
				list.PushBack(f);
				Assert::AreEqual(1_z, list.Size());
			}
		}


		/// <summary>
		/// Tests both the copy constructor and the copy assignment.
		/// </summary>
		TEST_METHOD(TestCopySemantics)
		{
			/// <summary>
			/// Testing the copy constructor.
			/// Create a list and fill with 3 elements. Copy the list using the copy constructor and make sure the size, front, and back are equal.
			/// Pop elements off the front until the copy of the list is empty, checking the size, front, and back at each pop. Check at the end that the list is empty.
			/// </summary>
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				list.PushBack(a);
				list.PushBack(b);
				list.PushBack(c);
				Assert::AreEqual(3_z, list.Size());

				SList<Foo> copyOfList{ list };
				Assert::AreEqual(3_z, copyOfList.Size());
				Assert::AreEqual(list.Front(), copyOfList.Front());
				Assert::AreEqual(list.Back(), copyOfList.Back());

				copyOfList.PopFront();
				Assert::AreEqual(2_z, copyOfList.Size());
				Assert::AreEqual(b, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::AreEqual(1_z, copyOfList.Size());
				Assert::AreEqual(c, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::IsTrue(copyOfList.IsEmpty());
			}

			/// <summary>
			/// Testing the copy assignment.
			/// Create a list and fill with 3 elements. Create a separate list with different order of elements.
			/// Copy the list using copy assignment and make sure the size, front, and back are equal.
			/// Pop elements off the front until the copy of the list is empty, checking the size, front, and back at each pop. Check at the end that the list is empty.
			/// </summary>
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				list.PushBack(a);
				list.PushBack(b);
				list.PushBack(c);
				Assert::AreEqual(3_z, list.Size());

				SList<Foo> copyOfList;
				copyOfList.PushBack(c);
				copyOfList.PushBack(b);
				copyOfList.PushBack(a);
				Assert::AreEqual(3_z, copyOfList.Size());

				copyOfList = list;
				Assert::AreEqual(list.Size(), copyOfList.Size());
				Assert::AreEqual(list.Front(), copyOfList.Front());
				Assert::AreEqual(list.Back(), copyOfList.Back());

				copyOfList.PopFront();
				Assert::AreEqual(2_z, copyOfList.Size());
				Assert::AreEqual(b, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::AreEqual(1_z, copyOfList.Size());
				Assert::AreEqual(c, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::IsTrue(copyOfList.IsEmpty());
			}
		}

		/// <summary>
		/// Tests both the move constructor and the move assignment.
		/// </summary>
		TEST_METHOD(TestMoveSemantics)
		{
			/// <summary>
			/// Testing the move constructor.
			/// Create a list and fill with 3 elements. Move the list using the move constructor.
			/// Make sure the previous list cannot be accessed using Front(). Check that the new list has the same size, front, and back that the previous list did.
			/// Pop elements off the front until the copy of the list is empty, checking the size, front, and back at each pop. Check at the end that the list is empty.
			/// </summary>
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				list.PushBack(a);
				list.PushBack(b);
				list.PushBack(c);
				Assert::AreEqual(3_z, list.Size());

				SList<Foo> copyOfList{ move(list) };
				Assert::ExpectException<exception>([&list] { list.Front(); }, L"Expected an exception, but none was thrown");

				Assert::AreEqual(3_z, copyOfList.Size());
				Assert::AreEqual(a, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::AreEqual(2_z, copyOfList.Size());
				Assert::AreEqual(b, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::AreEqual(1_z, copyOfList.Size());
				Assert::AreEqual(c, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::IsTrue(copyOfList.IsEmpty());
			}

			/// <summary>
			/// Testing the move assignment.
			/// Create a list and fill with 3 elements. Create a separate list with different number and order of elements. Move the list using move assignment.
			/// Make sure the previous list cannot be accessed using Front(). Check that the new list has the same size, front, and back that the previous list did.
			/// Pop elements off the front until the copy of the list is empty, checking the size, front, and back at each pop. Check at the end that the list is empty.
			/// </summary>
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				list.PushBack(a);
				list.PushBack(b);
				list.PushBack(c);
				Assert::AreEqual(3_z, list.Size());

				SList<Foo> copyOfList;
				copyOfList.PushBack(c);
				copyOfList.PushBack(b);
				Assert::AreEqual(2_z, copyOfList.Size());

				copyOfList = move(list);
				Assert::ExpectException<exception>([&list] { list.Front(); }, L"Expected an exception, but none was thrown");

				Assert::AreEqual(3_z, copyOfList.Size());
				Assert::AreEqual(a, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::AreEqual(2_z, copyOfList.Size());
				Assert::AreEqual(b, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::AreEqual(1_z, copyOfList.Size());
				Assert::AreEqual(c, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());

				copyOfList.PopFront();
				Assert::IsTrue(copyOfList.IsEmpty());
			}
		}

		/// <summary>
		/// Test the PopFront() method.
		/// Create a list and test using PopFront() on an empty list. No exception should occur.
		/// Push 3 elements onto the list. Save off the front element and use PopFront().
		/// Check that the size went down and that the new front is not the same as the element we saved off.
		/// </summary>
		TEST_METHOD(TestPopFront)
		{
			{
				// Setup the list
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				list.PopFront();

				list.PushBack(a);
				list.PushBack(b);
				list.PushBack(c);
				Assert::AreEqual(3_z, list.Size());

				// Remove element and test that size went down and the popped element is no longer at the front
				Foo front = list.Front();
				list.PopFront();
				Assert::AreEqual(2_z, list.Size());
				Assert::AreNotEqual(front, list.Front());
			}
		}

		/// <summary>
		/// Test the PopBack() method.
		/// Create a list and test using PopBack()  on an empty list. No exception should occur.
		/// Push 3 elements onto the list. Save off the back element and use PopBack().
		/// Check that the size went down and that the new back is not the same as the element we saved off.
		/// </summary>
		TEST_METHOD(TestPopBack)
		{
			{
				// Setup the list
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				list.PopBack();

				list.PushBack(a);
				list.PushBack(b);
				list.PushBack(c);
				Assert::AreEqual(3_z, list.Size());

				// Remove element and test that size went down and the popped element is no longer at the back
				Foo back = list.Back();
				list.PopBack();
				Assert::AreEqual(2_z, list.Size());
				Assert::AreNotEqual(back, list.Back());
			}
		}

		/// <summary>
		/// Tests Front() and PushFront()
		/// Creates a list and pushes 3 elements onto the front, checking that the size increments and that Front() gives the newly pushed element.
		/// </summary>
		TEST_METHOD(TestFront)
		{
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				Foo d(40);

				SList<Foo> list;

				list.PushFront(a);
				Assert::AreEqual(1_z, list.Size());
				Assert::AreEqual(a, list.Front());

				list.PushFront(b);
				Assert::AreEqual(2_z, list.Size());
				Assert::AreEqual(b, list.Front());
				Assert::AreEqual(a, list.Back());

				list.PushFront(c);
				Assert::AreEqual(3_z, list.Size());
				Assert::AreEqual(c, list.Front());
				Assert::AreEqual(a, list.Back());

				SList<Foo> moveList;

				moveList.PushFront(move(d));
				Assert::ExpectException<exception>([&d] { d.Data(); }, L"Expected an exception, but none was thrown");
				Assert::AreEqual(1_z, moveList.Size());
				Assert::AreEqual(Foo(40), moveList.Front());
				Assert::AreEqual(Foo(40), moveList.Back());

				moveList.PushFront(Foo(50));
				Assert::AreEqual(2_z, moveList.Size());
				Assert::AreEqual(Foo(50), moveList.Front());
				Assert::AreEqual(Foo(40), moveList.Back());
			}
		}

		/// <summary>
		/// Tests Front() on an empty list and a const list, which should cause exceptions.
		/// </summary>
		TEST_METHOD(TestFrontWithEmptyList)
		{
			{
				SList<Foo> list;
				Assert::ExpectException <runtime_error>([&list] { list.Front(); });

				const SList<Foo>& constList = list;
				Assert::ExpectException <runtime_error>([&constList] { constList.Front(); });
			}
		}

		/// <summary>
		/// Tests Back() and PushBack()
		/// Creates a list and pushes 3 elements onto the back, checking that the size increments and that Back() gives the newly pushed element.
		/// </summary>
		TEST_METHOD(TestBack)
		{
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);
				Foo d(40);

				SList<Foo> list;

				list.PushBack(a);
				Assert::AreEqual(1_z, list.Size());
				Assert::AreEqual(a, list.Back());

				list.PushBack(b);
				Assert::AreEqual(2_z, list.Size());
				Assert::AreEqual(b, list.Back());
				Assert::AreEqual(a, list.Front());

				list.PushBack(c);
				Assert::AreEqual(3_z, list.Size());
				Assert::AreEqual(c, list.Back());
				Assert::AreEqual(a, list.Front());

				const SList<Foo>& constList = list;
				Assert::AreEqual(3_z, constList.Size());
				Assert::AreEqual(c, constList.Back());
				Assert::AreEqual(a, constList.Front());

				SList<Foo> moveList;

				moveList.PushBack(move(d));
				Assert::ExpectException<exception>([&d] { d.Data(); }, L"Expected an exception, but none was thrown");
				Assert::AreEqual(1_z, moveList.Size());
				Assert::AreEqual(Foo(40), moveList.Front());
				Assert::AreEqual(Foo(40), moveList.Back());

				moveList.PushBack(Foo(50));
				Assert::AreEqual(2_z, moveList.Size());
				Assert::AreEqual(Foo(50), moveList.Back());
				Assert::AreEqual(Foo(40), moveList.Front());
			}
		}

		/// <summary>
		/// Tests Back() on an empty list and a const list, which should cause exceptions. 
		/// </summary>
		TEST_METHOD(TestBackWithEmptyList)
		{
			{
				SList<Foo> list;
				Assert::ExpectException <runtime_error>([&list] { list.Back(); });

				const SList<Foo>& constList = list;
				Assert::ExpectException <runtime_error>([&constList] { constList.Back(); });
			}
		}

		/// <summary>
		/// Tests Size() method.
		/// Creates a list and pushes 3 elements onto the back of the list, checking that the Size() method gives the correct number.
		/// Pops those 3 elements of the back, checking that the Size() method gives the correct number.
		/// </summary>
		TEST_METHOD(TestSize)
		{
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				Assert::AreEqual(0_z, list.Size());
				list.PushBack(a);
				Assert::AreEqual(1_z, list.Size());
				list.PushBack(b);
				Assert::AreEqual(2_z, list.Size());
				list.PushBack(c);
				Assert::AreEqual(3_z, list.Size());
				list.PopBack();
				Assert::AreEqual(2_z, list.Size());
				list.PopBack();
				Assert::AreEqual(1_z, list.Size());
				list.PopBack();
				Assert::AreEqual(0_z, list.Size());
			}
		}

		/// <summary>
		/// Tests the IsEmpty() method.
		/// Creates an empty list and tests that IsEmpty() returns true.
		/// Adds an element to this list and tests that IsEmpty() is false.
		/// </summary>
		TEST_METHOD(IsEmpty)
		{
			{
				// List does not contain any elements
				SList<Foo> list;
				Assert::IsTrue(list.IsEmpty());

				// List contains an element
				const Foo a(10);
				list.PushBack(a);
				Assert::IsFalse(list.IsEmpty());
			}
		}

		/// <summary>
		/// Tests the Clear() method.
		/// Creates a list and adds 3 elements to the list. 
		/// Clear the list and check that the size is 0. Check that Front() and Back() gives exceptions due to being set to nullptr.
		/// Create an empty list and use Clear() on it. No exception should occur.
		/// </summary>
		TEST_METHOD(Clear)
		{
			{
				const Foo a(10);
				const Foo b(20);
				const Foo c(30);

				SList<Foo> list;
				list.PushFront(a);
				list.PushFront(b);
				list.PushFront(c);
				list.Clear();
				Assert::AreEqual(0_z, list.Size());
				Assert::ExpectException <runtime_error>([&list] {list.Front(); });
				Assert::ExpectException <runtime_error>([&list] {list.Back(); });

				SList<Foo> emptyList;
				emptyList.Clear();
			}
		}
#pragma endregion 


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTests::sStartMemState;
}
