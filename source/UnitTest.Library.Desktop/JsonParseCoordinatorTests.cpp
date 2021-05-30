#include "pch.h"
#include "JsonParseCoordinator.h"
#include "JsonTestParseHelper.h"
#include "JsonIntegerParseHelper.h"
#include "Bar.h"
#include "json\json.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    std::wstring ToString<JsonParseCoordinator>(JsonParseCoordinator* t)
    {
        RETURN_WIDE_STRING(t);
    }

    template<>
    std::wstring ToString<JsonParseCoordinator>(const JsonParseCoordinator* t)
    {
        RETURN_WIDE_STRING(t);
    }


    template<>
    std::wstring ToString<JsonParseCoordinator::SharedData>(JsonParseCoordinator::SharedData* t)
    {
        RETURN_WIDE_STRING(t);
    }

    template<>
    std::wstring ToString<UnitTests::JsonTestParseHelper>(UnitTests::JsonTestParseHelper* t)
    {
        RETURN_WIDE_STRING(t);
    }

    template<>
    std::wstring ToString<UnitTests::JsonTestParseHelper::SharedData>(UnitTests::JsonTestParseHelper::SharedData* t)
    {
        RETURN_WIDE_STRING(t);
    }

    template<>
    std::wstring ToString<UnitTests::JsonTestParseHelper::SharedData>(const UnitTests::JsonTestParseHelper::SharedData* t)
    {
        RETURN_WIDE_STRING(t);
    }
}

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(JsonParseCoordinatorTest)
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

        TEST_METHOD(JsonTestParseHelperRTTI)
        {
            JsonTestParseHelper helper;

            RTTI* rtti = &helper;
            Assert::IsFalse(rtti->Is("Foo"s));
            Assert::IsTrue(rtti->Is("IJsonParseHelper"s));
            Assert::IsTrue(rtti->Is("JsonTestParseHelper"s));
            Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
            Assert::IsTrue(rtti->Is(JsonTestParseHelper::TypeIdClass()));
            Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
            Assert::AreEqual(JsonTestParseHelper::TypeIdClass(), rtti->TypeIdInstance());

            Foo* f = rtti->As<Foo>();
            Assert::IsNull(f);

            JsonTestParseHelper* h = rtti->As<JsonTestParseHelper>();
            Assert::IsNotNull(h);
            Assert::AreEqual(&helper, h);
            RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
            Assert::IsNull(r);
            r = rtti->QueryInterface(JsonTestParseHelper::TypeIdClass());
            Assert::IsNotNull(r);

            r = rtti->QueryInterface(Foo::TypeIdClass());
            Assert::IsNull(r);

            r = rtti->QueryInterface(JsonTestParseHelper::TypeIdClass());
            Assert::IsNotNull(r);
        }

        TEST_METHOD(JsonTestParseHelperSharedDataRTTI)
        {
            JsonTestParseHelper::SharedData sharedData;

            RTTI* rtti = &sharedData;
            Assert::IsFalse(rtti->Is("Foo"s));
            Assert::IsTrue(rtti->Is("SharedData"s));
            Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
            Assert::IsTrue(rtti->Is(JsonTestParseHelper::SharedData::TypeIdClass()));
            Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
            Assert::AreEqual(JsonTestParseHelper::SharedData::TypeIdClass(), rtti->TypeIdInstance());

            Foo* f = rtti->As<Foo>();
            Assert::IsNull(f);

            JsonTestParseHelper::SharedData* h = rtti->As<JsonTestParseHelper::SharedData>();
            Assert::IsNotNull(h);
            Assert::AreEqual(&sharedData, h);
            RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
            Assert::IsNull(r);
            r = rtti->QueryInterface(JsonTestParseHelper::SharedData::TypeIdClass());
            Assert::IsNotNull(r);

            r = rtti->QueryInterface(Foo::TypeIdClass());
            Assert::IsNull(r);

            r = rtti->QueryInterface(JsonTestParseHelper::SharedData::TypeIdClass());
            Assert::IsNotNull(r);
        }

        TEST_METHOD(Constructor)
        {
            JsonTestParseHelper::SharedData sharedData;
            JsonParseCoordinator parseCoordinator(sharedData);
            Assert::IsFalse(parseCoordinator.IsClone());
            Assert::AreEqual(0_z, parseCoordinator.Helpers().Size());

            Assert::AreEqual(&sharedData, parseCoordinator.GetSharedData()->As<JsonTestParseHelper::SharedData>());

            JsonTestParseHelper testParseHelper;
            parseCoordinator.AddHelper(testParseHelper);
            Assert::AreEqual(1_z, parseCoordinator.Helpers().Size());

            Assert::AreEqual(&sharedData, parseCoordinator.GetSharedData()->As<JsonTestParseHelper::SharedData>());
            Assert::AreEqual(&parseCoordinator, sharedData.GetJsonParseCoordinator());

            const auto& constSharedData = sharedData;
            Assert::AreEqual(const_cast<const JsonParseCoordinator*>(&parseCoordinator), constSharedData.GetJsonParseCoordinator());

            const auto& constParseCoordinator = parseCoordinator;
            Assert::AreEqual(const_cast<const JsonTestParseHelper::SharedData*>(&sharedData), constParseCoordinator.GetSharedData()->As<JsonTestParseHelper::SharedData>());

            JsonTestParseHelper::SharedData anotherSharedData;
            parseCoordinator.SetSharedData(&anotherSharedData);
            Assert::AreEqual(&anotherSharedData, parseCoordinator.GetSharedData()->As<JsonTestParseHelper::SharedData>());
        }


        TEST_METHOD(MoveSemantics)
        {
            JsonTestParseHelper::SharedData sharedData;
            JsonTestParseHelper testParseHelper;

            {
                JsonParseCoordinator parseCoordinator(sharedData);
                parseCoordinator.AddHelper(testParseHelper);

                //Move Constructor
                JsonParseCoordinator otherParseCoordinator(std::move(parseCoordinator));
                Assert::IsFalse(otherParseCoordinator.IsClone());
                Assert::AreEqual(1_z, otherParseCoordinator.Helpers().Size());
                Assert::AreEqual(&sharedData, otherParseCoordinator.GetSharedData()->As<JsonTestParseHelper::SharedData>());
                Assert::AreEqual(&otherParseCoordinator, sharedData.GetJsonParseCoordinator());
            }

            {
                JsonParseCoordinator parseCoordinator(sharedData);
                parseCoordinator.AddHelper(testParseHelper);

                
                //Move Assignment Operator
                JsonTestParseHelper::SharedData otherSharedData;
                JsonParseCoordinator otherParseCoordinator;
                otherParseCoordinator = std::move(parseCoordinator);
                Assert::IsFalse(otherParseCoordinator.IsClone());
                Assert::AreEqual(1_z, otherParseCoordinator.Helpers().Size());
                Assert::AreEqual(&sharedData, otherParseCoordinator.GetSharedData()->As<JsonTestParseHelper::SharedData>());
                Assert::AreEqual(&otherParseCoordinator, sharedData.GetJsonParseCoordinator());
            }
        }

        TEST_METHOD(ParseFromString)
        {
            const string inputString = R"(
            {
                "Address":
                {
                    "type": "table",
                    "value":
                    {
                        "Street":
                        {
                            "type": "string",
                            "value": "123 Anystreet St."
                        }
                    }
                }
            })"s;

            JsonTestParseHelper::SharedData sharedData;
            JsonTestParseHelper testParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(testParseHelper);

            Assert::IsFalse(testParseHelper.InitializeCalled);
            parseCoordinator.Initialize();
            Assert::IsTrue(testParseHelper.InitializeCalled);

            Assert::AreEqual(0U, testParseHelper._startHandlerCount);
            Assert::AreEqual(0U, testParseHelper._endHandlerCount);
            Assert::AreEqual(0U, sharedData.Depth());
            Assert::AreEqual(0U, sharedData.MaxDepth);

            parseCoordinator.Parse(inputString);

            Assert::AreEqual(6U, testParseHelper._startHandlerCount);
            Assert::AreEqual(6U, testParseHelper._endHandlerCount);
            Assert::AreEqual(0U, sharedData.Depth());
            Assert::AreEqual(4U, sharedData.MaxDepth);
        }

        TEST_METHOD(ParseFromStream)
        {
            stringstream inputStream;
            inputStream << R"(
            {
                "Health":
                {
                    "type": "integer",
                    "value": 100
                }
            })"s;

            JsonTestParseHelper::SharedData sharedData;
            JsonTestParseHelper testParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(testParseHelper);

            Assert::IsFalse(testParseHelper.InitializeCalled);
            parseCoordinator.Initialize();
            Assert::IsTrue(testParseHelper.InitializeCalled);

            parseCoordinator.Parse(inputStream);

            Assert::AreEqual(3U, testParseHelper._startHandlerCount);
            Assert::AreEqual(3U, testParseHelper._endHandlerCount);
            Assert::AreEqual(0U, sharedData.Depth());
            Assert::AreEqual(2U, sharedData.MaxDepth);
        }

        TEST_METHOD(FileParsing)
        {
            const std::string filename = "TestFile.json";
            const string inputString = R"({ "Health": { "type": "integer", "value": 100 } })";

            ofstream outputFile(filename);
            Assert::IsTrue(outputFile.good());

            outputFile << inputString;
            outputFile.close();

            JsonTestParseHelper::SharedData sharedData;
            JsonTestParseHelper testParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(testParseHelper);

            Assert::IsFalse(testParseHelper.InitializeCalled);
            parseCoordinator.Initialize();
            Assert::IsTrue(testParseHelper.InitializeCalled);

            parseCoordinator.ParseFromFile(filename);

            Assert::AreEqual(3U, testParseHelper._startHandlerCount);
            Assert::AreEqual(3U, testParseHelper._endHandlerCount);
            Assert::AreEqual(0U, sharedData.Depth());
            Assert::AreEqual(2U, sharedData.MaxDepth);
            Assert::AreEqual(filename, parseCoordinator.GetFileName());

            const JsonParseCoordinator constParseCoordinator(parseCoordinator);
            Assert::AreEqual(constParseCoordinator.GetFileName(), filename);

            Assert::ExpectException<invalid_argument>([&parseCoordinator] { parseCoordinator.ParseFromFile("NonExistantFile.json"); });
        }

        TEST_METHOD(Clone)
        {
            JsonTestParseHelper::SharedData sharedData;
            JsonParseCoordinator parseCoordinator(sharedData);
            Assert::IsFalse(parseCoordinator.IsClone());

            JsonTestParseHelper testParseHelper;
            parseCoordinator.AddHelper(testParseHelper);

            JsonIntegerParseHelper integerParseHelper;
            parseCoordinator.AddHelper(integerParseHelper);

            auto clone = parseCoordinator.Clone();
            Assert::AreNotEqual(clone, &parseCoordinator);
            Assert::IsTrue(clone->IsClone());
            Assert::AreEqual(2_z, clone->Helpers().Size());

            JsonTestParseHelper* clonedHelper = clone->Helpers()[0]->As<JsonTestParseHelper>();
            Assert::IsNotNull(clonedHelper);
            Assert::AreNotEqual(clonedHelper, parseCoordinator.Helpers()[0]->As<JsonTestParseHelper>());
            Assert::AreNotSame(clone->GetSharedData(), parseCoordinator.GetSharedData());

            
            Assert::ExpectException<runtime_error>([&clone] { JsonTestParseHelper helper; clone->AddHelper(helper); });
            

            
            clone->RemoveHelper(*clonedHelper);
            Assert::AreEqual(1_z, clone->Helpers().Size());
            
            Assert::ExpectException<runtime_error>([&clone] { JsonTestParseHelper::SharedData data; clone->SetSharedData(&data); });
            
            delete clone;
        }

        TEST_METHOD(ParsingInvalidJson)
        {
            JsonTestParseHelper::SharedData sharedData;
            sharedData.Initialize();
            JsonTestParseHelper testParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(testParseHelper);
            Assert::ExpectException<exception>([&parseCoordinator] {
                const string inputString = R"({ "integer": { 100 } })"; 
                parseCoordinator.Parse(inputString); 
            });
        }

        /// <summary>
        /// Does not actually parse a string. Used to test when no helper can parse.
        /// </summary>
        TEST_METHOD(NoHandlerAccepts)
        {
            JsonTestParseHelper::SharedData sharedData;
            sharedData.Initialize();
            JsonIntegerParseHelper integerParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(integerParseHelper);
            const string inputString = R"({ "string": "hello" })";
            parseCoordinator.Parse(inputString);
        }

        TEST_METHOD(IntegerParsing)
        {
            string inputString = R"({ "integer": 100 })";

            JsonIntegerParseHelper::SharedData sharedData;
            JsonIntegerParseHelper integerParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(integerParseHelper);

            parseCoordinator.Parse(inputString);
            Assert::AreEqual(1_z, sharedData.Data.Size());
            Assert::AreEqual(100, sharedData.Data.Front());
        }

        TEST_METHOD(IntegerArrayParsing)
        {
            string inputString = R"({ "integer": [ 10, 20, 30, 40 ] })";

            JsonIntegerParseHelper::SharedData sharedData;
            JsonIntegerParseHelper integerParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(integerParseHelper);

            parseCoordinator.Parse(inputString);
            Assert::AreEqual(size_t(4), sharedData.Data.Size());
            Assert::AreEqual(10, sharedData.Data[0]);
            Assert::AreEqual(20, sharedData.Data[1]);
            Assert::AreEqual(30, sharedData.Data[2]);
            Assert::AreEqual(40, sharedData.Data[3]);
        }
    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState JsonParseCoordinatorTest::sStartMemState;
}