#include "pch.h"
#include "JsonTestParseHelper.h"
#include <json\json.h>



using namespace std;
using namespace FIEAGameEngine;

namespace UnitTests
{
#pragma region SharedData
	RTTI_DEFINITIONS(JsonTestParseHelper::SharedData)

	void JsonTestParseHelper::SharedData::Initialize()
	{
		JsonParseCoordinator::SharedData::Initialize();
		MaxDepth = 0;
	}

	gsl::owner<JsonTestParseHelper::SharedData*> JsonTestParseHelper::SharedData::Create() const
	{
		return new SharedData();
	}

#pragma endregion

#pragma region SharedData
	RTTI_DEFINITIONS(JsonTestParseHelper)

	gsl::owner<IJsonParseHelper*> JsonTestParseHelper::Create() const
	{
		return new JsonTestParseHelper();
	}

	void JsonTestParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		InitializeCalled = true;
		_startHandlerCount = 0;
		_endHandlerCount = 0;
	}

	bool JsonTestParseHelper::StartHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string&, const Json::Value&, bool, size_t)
	{
		JsonTestParseHelper::SharedData* customSharedData = sharedData.As<JsonTestParseHelper::SharedData>();
		if (customSharedData == nullptr)
		{
			return false;
		}

		++_startHandlerCount;

		if (customSharedData->Depth() > customSharedData->MaxDepth)
		{
			customSharedData->MaxDepth = customSharedData->Depth();
		}

		return true;
	}

	bool JsonTestParseHelper::EndHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string&, bool)
	{
		if (!sharedData.Is(JsonTestParseHelper::SharedData::TypeIdClass()))
		{
			return false;
		}

		++_endHandlerCount;
		return true;
	}

#pragma endregion
}