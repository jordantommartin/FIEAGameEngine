#include "pch.h"
#include "JsonIntegerParseHelper.h"
#include <json\json.h>

using namespace std;
using namespace FIEAGameEngine;

namespace UnitTests
{
#pragma region SharedData

	RTTI_DEFINITIONS(JsonIntegerParseHelper::SharedData)

	void JsonIntegerParseHelper::SharedData::Initialize()
	{
		JsonParseCoordinator::SharedData::Initialize();
		Data.Clear();
	}

	gsl::owner<JsonIntegerParseHelper::SharedData*> JsonIntegerParseHelper::SharedData::Create() const
	{
		return new SharedData();
	}

#pragma endregion

#pragma region JsonIntegerParseHelper

	RTTI_DEFINITIONS(JsonIntegerParseHelper)

	gsl::owner<IJsonParseHelper*> JsonIntegerParseHelper::Create() const
	{
		return new JsonIntegerParseHelper();
	}

	void JsonIntegerParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		_parsingData = false;
	}

	bool JsonIntegerParseHelper::StartHandler(JsonParseCoordinator::SharedData& sharedData, const string& key, const Json::Value& value, bool, size_t)
	{
		JsonIntegerParseHelper::SharedData* customSharedData = sharedData.As<JsonIntegerParseHelper::SharedData>();
		if (customSharedData == nullptr)
		{
			return false;
		}

		if (key != IntegerKey)
		{
			return false;
		}

		if (_parsingData)
		{
			throw runtime_error("Received another start element while parsing an integer.");
		}

		_parsingData = true;

		customSharedData->Data.PushBack(value.asInt());

		return true;
	}



	bool JsonIntegerParseHelper::EndHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, bool)
	{
		JsonIntegerParseHelper::SharedData* customSharedData = sharedData.As<JsonIntegerParseHelper::SharedData>();
		if (customSharedData == nullptr || key != IntegerKey || _parsingData == false)
		{
			return false;
		}

		_parsingData = false;

		return true;
	}

#pragma endregion
		
}