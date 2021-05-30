#include "pch.h"
#include "json\json.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{


}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonCppTest)
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

		TEST_METHOD(TestSimpleDeserialization)
		{
			stringstream inputStream;
			inputStream << R"({ "Health": { "type": "integer", "value": 100 } })";

			Json::Value root;
			inputStream >> root;

			stringstream outputStream;
			OutputJsonToStream(outputStream, root);
			Logger::WriteMessage(outputStream.str().c_str());
		}

		void Indent(ostream& ofs, int indent)
		{
			for (int i = 0; i < indent; i++)
			{
				ofs << ' ';
			}
		}

		void OutputJsonToStream(ostream& ofs, const Json::Value& val, int indent = 0)
		{
			switch (val.type())
			{
			case Json::nullValue : ofs << "null"; break;
			case Json::booleanValue: ofs << (val.asBool() ? "true" : "false"); break;
			case Json::intValue: ofs << val.asLargestInt(); break;
			case Json::uintValue: ofs << val.asLargestUInt(); break;
			case Json::realValue: ofs << val.asDouble(); break;
			case Json::stringValue: ofs << '"' << val.asString() << '"'; break;
			case Json::arrayValue: {
				Json::ArrayIndex size = val.size();
				if (size == 0)
					ofs << "[]";
				else {
					ofs << "[\n";
					int newIndent = indent + 4;
					for (Json::ArrayIndex i = 0; i < size; i++) {
						Indent(ofs, newIndent);
						OutputJsonToStream(ofs, val[i], newIndent);
						ofs << (i + 1 == size ? "\n" : ",\n");
					}
					Indent(ofs, indent);
					ofs << ']';
				}
				break;
			}
			case Json::objectValue: {
				if (val.empty())
					ofs << "{}";
				else {
					ofs << "{\n";
					int newIndent = indent + 4;
					const vector<string> keys = val.getMemberNames();
					for (size_t i = 0; i < keys.size(); i++)
					{
						Indent(ofs, newIndent);
						const string& key = keys[i];
						ofs << '"' << key << '"' << " : ";
						OutputJsonToStream(ofs, val[key], newIndent);
						ofs << (i + 1 == keys.size() ? "\n" : ",\n");
					}
					Indent(ofs, indent);
					ofs << '}';
				}
				break;
			}
			default:
				cerr << "Wrong type!" << endl;
				exit(0);
			}
		}
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonCppTest::sStartMemState;
}