#include "pch.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "JsonIntegerParseHelper.h"
#include "Bar.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "Factory.h"
#include "Power.h"
#include <algorithm>

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
    std::wstring ToString<JsonTableParseHelper>(JsonTableParseHelper* t)
    {
        RETURN_WIDE_STRING(t);
    }

    template<>
    std::wstring ToString<JsonTableParseHelper::SharedData>(JsonTableParseHelper::SharedData* t)
    {
        RETURN_WIDE_STRING(t);
    }

    template<>
    std::wstring ToString<JsonTableParseHelper::SharedData>(const JsonTableParseHelper::SharedData* t)
    {
        RETURN_WIDE_STRING(t);
    }

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
}

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(JsonTableParseHelperTests)
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

        TEST_METHOD(JsonTableParseHelperRTTI)
        {
            JsonTableParseHelper helper;

            RTTI* rtti = &helper;
            Assert::IsFalse(rtti->Is("Foo"s));
            Assert::IsTrue(rtti->Is("IJsonParseHelper"s));
            Assert::IsTrue(rtti->Is("JsonTableParseHelper"s));
            Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
            Assert::IsTrue(rtti->Is(JsonTableParseHelper::TypeIdClass()));
            Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
            Assert::AreEqual(JsonTableParseHelper::TypeIdClass(), rtti->TypeIdInstance());

            Bar* b = rtti->As<Bar>();
            Assert::IsNull(b);

            JsonTableParseHelper* h = rtti->As<JsonTableParseHelper>();
            Assert::IsNotNull(h);
            Assert::AreEqual(&helper, h);
            RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
            Assert::IsNull(r);
            r = rtti->QueryInterface(JsonTableParseHelper::TypeIdClass());
            Assert::IsNotNull(r);

            r = rtti->QueryInterface(Bar::TypeIdClass());
            Assert::IsNull(r);

            r = rtti->QueryInterface(JsonTableParseHelper::TypeIdClass());
            Assert::IsNotNull(r);
        }

        TEST_METHOD(JsonTableParseHelperSharedDataRTTI)
        {
            JsonTableParseHelper::SharedData sharedData;

            RTTI* rtti = &sharedData;
            Assert::IsFalse(rtti->Is("Bar"s));
            Assert::IsTrue(rtti->Is("SharedData"s));
            Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
            Assert::IsTrue(rtti->Is(JsonTableParseHelper::SharedData::TypeIdClass()));
            Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
            Assert::AreEqual(JsonTableParseHelper::SharedData::TypeIdClass(), rtti->TypeIdInstance());

            Bar* f = rtti->As<Bar>();
            Assert::IsNull(f);

            JsonTableParseHelper::SharedData* h = rtti->As<JsonTableParseHelper::SharedData>();
            Assert::IsNotNull(h);
            Assert::AreEqual(&sharedData, h);
            RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
            Assert::IsNull(r);
            r = rtti->QueryInterface(JsonTableParseHelper::SharedData::TypeIdClass());
            Assert::IsNotNull(r);

            r = rtti->QueryInterface(Bar::TypeIdClass());
            Assert::IsNull(r);

            r = rtti->QueryInterface(JsonTableParseHelper::SharedData::TypeIdClass());
            Assert::IsNotNull(r);
        }

        TEST_METHOD(Constructor)
        {
            Scope scope;
            JsonTableParseHelper::SharedData sharedData(scope);
            JsonParseCoordinator parseCoordinator(sharedData);
            Assert::IsFalse(parseCoordinator.IsClone());
            Assert::AreEqual(0_z, parseCoordinator.Helpers().Size());
            Assert::AreEqual(&sharedData, parseCoordinator.GetSharedData()->As<JsonTableParseHelper::SharedData>());
            Assert::AreEqual(&scope, sharedData.GetScope());

            JsonTableParseHelper tableParseHelper;
            parseCoordinator.AddHelper(tableParseHelper);
            Assert::AreEqual(1_z, parseCoordinator.Helpers().Size());

            Assert::AreEqual(&sharedData, parseCoordinator.GetSharedData()->As<JsonTableParseHelper::SharedData>());
            Assert::AreEqual(&parseCoordinator, sharedData.GetJsonParseCoordinator());

            const auto& constSharedData = sharedData;
            Assert::AreEqual(const_cast<const JsonParseCoordinator*>(&parseCoordinator), constSharedData.GetJsonParseCoordinator());
            Assert::AreEqual(const_cast<const Scope*>(&scope), constSharedData.GetScope());

            const auto& constParseCoordinator = parseCoordinator;
            Assert::AreEqual(const_cast<const JsonTableParseHelper::SharedData*>(&sharedData), constParseCoordinator.GetSharedData()->As<JsonTableParseHelper::SharedData>());

            JsonTableParseHelper::SharedData anotherSharedData;
            parseCoordinator.SetSharedData(&anotherSharedData);
            Assert::AreEqual(&anotherSharedData, parseCoordinator.GetSharedData()->As<JsonTableParseHelper::SharedData>());

            sharedData.SetScope(nullptr);
            Assert::IsNull(sharedData.GetScope());
        }
        

        TEST_METHOD(ParseFromString)
        {
            const string inputString = R"(
            {
                "Street":
                {
                    "type": "string",
                    "value": "123 Anystreet St."
                }
            })"s;

            Scope scope;
            JsonTableParseHelper::SharedData sharedData(scope);
            JsonTableParseHelper tableParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(tableParseHelper);

            parseCoordinator.Parse(inputString);
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

            Scope scope;
            JsonTableParseHelper::SharedData sharedData(scope);
            JsonTableParseHelper tableParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(tableParseHelper);

            parseCoordinator.Parse(inputStream);
        }

        TEST_METHOD(ParseSingleAttribute)
        {
            string inputString = R"({
	        "Name": {
		        "type": "string",
		        "value": "Test Name"
	            }
            })";

            Scope scope;
            JsonTableParseHelper::SharedData sharedData(scope);
            JsonTableParseHelper tableParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(tableParseHelper);

            parseCoordinator.Parse(inputString);
            Assert::AreEqual(1_z, sharedData.Root->Size());
            Assert::AreEqual("Test Name"s, sharedData.Root->At("Name").GetString());
        }


        TEST_METHOD(ParseSingleAttributeArray)
        {
            string inputString = R"({
	        "Name": {
		        "type": "string",
		        "value": [ "Test Name 0", "Test Name 1", "Test Name 2" ] 
	            }
            })";

            Scope scope;
            JsonTableParseHelper::SharedData sharedData(scope);
            JsonTableParseHelper tableParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(tableParseHelper);

            parseCoordinator.Parse(inputString);
            Assert::AreEqual(1_z, sharedData.Root->Size());
            Assert::AreEqual("Test Name 0"s, sharedData.Root->At("Name").GetString(0));
            Assert::AreEqual("Test Name 1"s, sharedData.Root->At("Name").GetString(1));
            Assert::AreEqual("Test Name 2"s, sharedData.Root->At("Name").GetString(2));
        }

        TEST_METHOD(FileParsing)
        {
            ScopeFactory scopeFactory;
            const std::string filename = R"(Content\TestScope.json)";

            Scope scope;
            JsonTableParseHelper::SharedData sharedData(scope);
            JsonTableParseHelper tableParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(tableParseHelper);

            parseCoordinator.ParseFromFile(filename);
            Assert::AreEqual(9_z, sharedData.Root->Size());
            Assert::AreEqual("Test Name"s, sharedData.Root->At("Name").GetString());
            Assert::AreEqual(100, sharedData.Root->At("Health").GetInteger());
            Assert::AreEqual(1.25f, sharedData.Root->At("Speed").GetFloat());
            Assert::AreEqual(glm::vec4(1,0,0,1), sharedData.Root->At("Direction").GetVector());
            Assert::AreEqual(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), sharedData.Root->At("Transform").GetMatrix());
            Assert::AreEqual("Bob"s, sharedData.Root->At("Aliases").GetString(0));
            Assert::AreEqual("Jim"s, sharedData.Root->At("Aliases").GetString(1));
            Assert::AreEqual("Fred"s, sharedData.Root->At("Aliases").GetString(2));

            Scope root;
            Scope& address = root.AppendScope("Address");

            Datum& city = address.Append("City");
            city.SetType(Datum::DatumTypes::String);
            city.PushBack("Orlando");

            Scope& postal = address.AppendScope("Postal Code");

            Datum& zip = postal.Append("Zip Code");
            zip.SetType(Datum::DatumTypes::Integer);
            zip.PushBack(12345);

            Datum& zip4 = postal.Append("Zip Code+4");
            zip4.SetType(Datum::DatumTypes::Integer);
            zip4.PushBack(6789);

            Datum& street = address.Append("Street");
            street.SetType(Datum::DatumTypes::String);
            street.PushBack("123 Anystreet St.");

            Assert::AreEqual(address, sharedData.Root->At("Address").GetScope(0));
            
            Datum emptyDatum;
            emptyDatum.SetType(Datum::DatumTypes::Table);
            Assert::AreEqual(emptyDatum, sharedData.Root->At("EmptyNestedScopeArray"));

            Assert::AreEqual(3_z, sharedData.Root->At("Powers").Size());
            Assert::AreEqual(2_z, sharedData.Root->At("Powers").GetScope(0).Size());
            Assert::AreEqual(2_z, sharedData.Root->At("Powers").GetScope(1).Size());
            Assert::AreEqual(2_z, sharedData.Root->At("Powers").GetScope(2).Size());

            Assert::AreEqual(filename, parseCoordinator.GetFileName());

            const JsonParseCoordinator constParseCoordinator(parseCoordinator);
            Assert::AreEqual(constParseCoordinator.GetFileName(), filename);

            Assert::ExpectException<invalid_argument>([&parseCoordinator] { parseCoordinator.ParseFromFile("NonExistantFile.json"); });
        }

        TEST_METHOD(PowerParsing)
        {
            ScopeFactory scopeFactory;
            PowerFactory powerFactory;
            const std::string filename = R"(Content\TestScopeFactory.json)";

            Scope scope;
            JsonTableParseHelper::SharedData sharedData(scope);
            JsonTableParseHelper tableParseHelper;
            JsonParseCoordinator parseCoordinator(sharedData);
            parseCoordinator.AddHelper(tableParseHelper);

            parseCoordinator.ParseFromFile(filename);
            Assert::AreEqual(9_z, sharedData.Root->Size());
            Assert::AreEqual("Test Name"s, sharedData.Root->At("Name").GetString());
            Assert::AreEqual(100, sharedData.Root->At("Health").GetInteger());
            Assert::AreEqual(1.25f, sharedData.Root->At("Speed").GetFloat());
            Assert::AreEqual(glm::vec4(1, 0, 0, 1), sharedData.Root->At("Direction").GetVector());
            Assert::AreEqual(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), sharedData.Root->At("Transform").GetMatrix());
            Assert::AreEqual("Bob"s, sharedData.Root->At("Aliases").GetString(0));
            Assert::AreEqual("Jim"s, sharedData.Root->At("Aliases").GetString(1));
            Assert::AreEqual("Fred"s, sharedData.Root->At("Aliases").GetString(2));

            Scope root;
            Scope& address = root.AppendScope("Address");

            Datum& city = address.Append("City");
            city.SetType(Datum::DatumTypes::String);
            city.PushBack("Orlando");

            Scope& postal = address.AppendScope("Postal Code");

            Datum& zip = postal.Append("Zip Code");
            zip.SetType(Datum::DatumTypes::Integer);
            zip.PushBack(12345);

            Datum& zip4 = postal.Append("Zip Code+4");
            zip4.SetType(Datum::DatumTypes::Integer);
            zip4.PushBack(6789);

            Datum& street = address.Append("Street");
            street.SetType(Datum::DatumTypes::String);
            street.PushBack("123 Anystreet St.");

            Assert::AreEqual(address, sharedData.Root->At("Address").GetScope(0));

            Datum emptyDatum;
            emptyDatum.SetType(Datum::DatumTypes::Table);
            Assert::AreEqual(emptyDatum, sharedData.Root->At("EmptyNestedScopeArray"));

            Assert::AreEqual(3_z, sharedData.Root->At("Powers").Size());
            Assert::IsTrue((sharedData.Root->At("Powers").GetScope().Is(Power::TypeIdClass())));
            Assert::AreEqual(3_z, sharedData.Root->At("Powers").GetScope(0).Size());
            Assert::AreEqual(Datum::DatumTypes::Pointer, sharedData.Root->At("Powers").GetScope(0).At("this").Type());
            Assert::AreEqual("Laser Vision"s, sharedData.Root->At("Powers").GetScope(0).At("Name").GetString());
            Assert::AreEqual(1.5f, sharedData.Root->At("Powers").GetScope(0).At("Dps").GetFloat());
            Assert::AreEqual(3_z, sharedData.Root->At("Powers").GetScope(1).Size());
            Assert::IsTrue((sharedData.Root->At("Powers").GetScope(1).Is(Power::TypeIdClass())));
            Assert::AreEqual(Datum::DatumTypes::Pointer, sharedData.Root->At("Powers").GetScope(1).At("this").Type());
            Assert::AreEqual("X-Ray Vision"s, sharedData.Root->At("Powers").GetScope(1).At("Name").GetString());
            Assert::AreEqual(0.0f, sharedData.Root->At("Powers").GetScope(1).At("Dps").GetFloat());
            Assert::AreEqual(3_z, sharedData.Root->At("Powers").GetScope(2).Size());
            Assert::IsTrue((sharedData.Root->At("Powers").GetScope(2).Is(Power::TypeIdClass())));
            Assert::AreEqual(Datum::DatumTypes::Pointer, sharedData.Root->At("Powers").GetScope(2).At("this").Type());
            Assert::AreEqual("Heat Ray"s, sharedData.Root->At("Powers").GetScope(2).At("Name").GetString());
            Assert::AreEqual(4.9f, sharedData.Root->At("Powers").GetScope(2).At("Dps").GetFloat());

            Assert::AreEqual(filename, parseCoordinator.GetFileName());

            const JsonParseCoordinator constParseCoordinator(parseCoordinator);
            Assert::AreEqual(constParseCoordinator.GetFileName(), filename);

            Assert::ExpectException<invalid_argument>([&parseCoordinator] { parseCoordinator.ParseFromFile("NonExistantFile.json"); });
        }

        TEST_METHOD(Create)
        {
            const std::string filename = R"(Content\TestScope.json)";

            Scope scope;
            JsonTableParseHelper::SharedData sharedData;
            JsonTableParseHelper::SharedData* copySharedData = sharedData.Create();
            delete copySharedData;
            JsonTableParseHelper tableParseHelper;
            JsonTableParseHelper* copyTableParseHelper = tableParseHelper.Create()->As<JsonTableParseHelper>();
            delete copyTableParseHelper;
        }

    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState JsonTableParseHelperTests::sStartMemState;
}