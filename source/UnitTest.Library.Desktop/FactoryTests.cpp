#include "pch.h"
#include <crtdbg.h>
#include "CppUnitTest.h"
#include "Foo.h"
#include "Bar.h"
#include "Factory.h"
#include "FooFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;
using namespace UnitTests;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
   template<>
   std::wstring ToString<FooFactory>(const FooFactory* t)
   {
       RETURN_WIDE_STRING(t);
   }

   template<>
   std::wstring ToString<UnitTests::BarFactory>(const UnitTests::BarFactory* t)
   {
       RETURN_WIDE_STRING(t);
   }
}

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(FactoryTests)
    {
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


        TEST_METHOD(Constructor)
        {
            Assert::AreEqual(0_z, Factory<RTTI>::Size());
            Assert::IsTrue(Factory<RTTI>::IsEmpty());
        }

        TEST_METHOD(AddRemoveFactory)
        {
            const Factory<RTTI>* foundFactory = Factory<RTTI>::Find("Foo"s);

            Assert::IsNull(foundFactory);
            foundFactory = Factory<RTTI>::Find("Bar"s);
            Assert::IsNull(foundFactory);

            {
                const FooFactory fooFactory;
                Assert::AreEqual(1_z, Factory<RTTI>::Size());
                foundFactory = Factory<RTTI>::Find("Foo"s);
                Assert::AreEqual(&fooFactory, static_cast<const FooFactory*>(foundFactory));

                const BarFactory barFactory;
                Assert::AreEqual(2_z, Factory<RTTI>::Size());
                foundFactory = Factory<RTTI>::Find("Bar"s);
                Assert::AreEqual(&barFactory, static_cast<const BarFactory*>(foundFactory));
            }

            Assert::AreEqual(0_z, Factory<RTTI>::Size());
            foundFactory = Factory<RTTI>::Find("Foo"s);
            Assert::IsNull(foundFactory);
            foundFactory = Factory<RTTI>::Find("Bar"s);
            Assert::IsNull(foundFactory);
        }

        TEST_METHOD(ProductCreationFoo)
        {
            RTTI* rtti = Factory<RTTI>::Create("Foo"s);
            Assert::IsNull(rtti);
            {
                FooFactory foofactory;

                rtti = Factory<RTTI>::Create("Foo"s);
                Assert::IsNotNull(rtti);

                Foo* foo = rtti->As<Foo>();
                Assert::IsNotNull(foo);

                delete foo;
            }

            rtti = Factory<RTTI>::Create("Foo"s);
            Assert::IsNull(rtti);
        }


        TEST_METHOD(ProductCreationBar)
        {
            RTTI* rtti = Factory<RTTI>::Create("Bar"s);
            Assert::IsNull(rtti);
            {
                BarFactory Barfactory;

                rtti = Factory<RTTI>::Create("Bar"s);
                Assert::IsNotNull(rtti);

                Bar* bar = rtti->As<Bar>();
                Assert::IsNotNull(bar);

                delete bar;

                Assert::ExpectException<runtime_error>([] { BarFactory anotherBarFactory; });
            }

            rtti = Factory<RTTI>::Create("Bar"s);
            Assert::IsNull(rtti);
        }

    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState FactoryTests::sStartMemState;
}