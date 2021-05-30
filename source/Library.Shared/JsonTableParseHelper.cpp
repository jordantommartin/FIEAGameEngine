#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include <json\json.h>

using namespace std;
using namespace FIEAGameEngine;
using namespace string_literals;

namespace FIEAGameEngine
{
#pragma region SharedData

	RTTI_DEFINITIONS(JsonTableParseHelper::SharedData)

	JsonTableParseHelper::SharedData::SharedData(Scope& scope) :
		Root(&scope)
	{

	}

	void JsonTableParseHelper::SharedData::Initialize()
	{
		JsonParseCoordinator::SharedData::Initialize();
	}

	gsl::owner<JsonTableParseHelper::SharedData*> JsonTableParseHelper::SharedData::Create() const
	{
		return new JsonTableParseHelper::SharedData();
	}

	Scope* JsonTableParseHelper::SharedData::GetScope()
	{
		return Root;
	}

	const Scope* JsonTableParseHelper::SharedData::GetScope() const
	{
		return Root;
	}

	void JsonTableParseHelper::SharedData::SetScope(Scope* scope)
	{
		Root = scope;
	}

#pragma endregion

#pragma region JsonTableParseHelper

	RTTI_DEFINITIONS(JsonTableParseHelper)

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
	{
		return new JsonTableParseHelper();
	}

	void JsonTableParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		_contextStack.Clear();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseCoordinator::SharedData& sharedData, const string& key, const Json::Value& value, bool isArray, size_t index)
	{
		JsonTableParseHelper::SharedData* customSharedData = sharedData.As<JsonTableParseHelper::SharedData>();
		assert(customSharedData != nullptr);

		if (key == ClassKey)
		{
			StackFrame& contextFrame = _contextStack.Top();
			assert(value.isString());
			contextFrame._className = value.asString();
		}
		else if (key == TypeKey)
		{
			StackFrame& contextFrame = _contextStack.Top();
			contextFrame._currentDatum.SetType(Datum::_typeMap.At(value.asString()));
		}
		else if (key == ValueKey)
		{
			StackFrame& contextFrame = _contextStack.Top();

			if (contextFrame._currentDatum.Type() == Datum::DatumTypes::Table)
			{
				if (!isArray)
				{
					Scope* product = Factory<Scope>::Create(contextFrame._className);
					assert(product != nullptr);
					contextFrame._currentRoot.Adopt(*product, contextFrame._key);
					_contextStack.Push(StackFrame(*product, contextFrame._currentDatum, _contextStack.Top()._className, _contextStack.Top()._key));
				}
			}
			else
			{
				if (contextFrame._currentDatum.Type() == Datum::DatumTypes::Integer)
				{
					contextFrame._currentDatum.IsExternal() ? contextFrame._currentDatum.Set(value.asInt(), index) : contextFrame._currentDatum.PushBack(value.asInt());
				}
				else if (contextFrame._currentDatum.Type() == Datum::DatumTypes::Float)
				{
					contextFrame._currentDatum.IsExternal() ? contextFrame._currentDatum.Set(value.asFloat(), index) : contextFrame._currentDatum.PushBack(value.asFloat());
				}
				else
				{
					contextFrame._currentDatum.IsExternal() ? contextFrame._currentDatum.SetFromString(value.asString(), index) : contextFrame._currentDatum.PushBackFromString(value.asString());
				}
			}
		}
		else 
		{
			Scope& context = _contextStack.IsEmpty() ? *customSharedData->Root : _contextStack.Top()._currentRoot;
			Datum& datum = context.Append(key);
			_contextStack.Push(StackFrame(context, datum, ScopeClassName, key));
		}
		
		return true;
	}

	bool JsonTableParseHelper::EndHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, bool isArray)
	{
		assert(sharedData.Is(JsonTableParseHelper::SharedData::TypeIdClass()));
		UNREFERENCED_LOCAL(sharedData);

		Datum* datum = _contextStack.Top()._currentRoot.Search(_contextStack.Top()._key);
		if ((key != TypeKey && key != ValueKey && key != ClassKey) || (key == ValueKey && datum->Type() == Datum::DatumTypes::Table && !isArray)) 
		{
			_contextStack.Pop();
		}

		return true;
	}

#pragma endregion

#pragma region StackFrame
	JsonTableParseHelper::StackFrame::StackFrame(Scope& scope, Datum& datum, std::string& className, const std::string& key) :
		_currentRoot(scope), _currentDatum(datum), _className(className), _key(key)
	{
		
	}
#pragma endregion
}