#pragma once

#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "Vector.h"

namespace UnitTests
{
	class JsonIntegerParseHelper final : public FIEAGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerParseHelper, FIEAGameEngine::IJsonParseHelper)

	public:
		class SharedData final : public FIEAGameEngine::JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FIEAGameEngine::JsonParseCoordinator::SharedData)

		public:
			virtual void Initialize() override;
			virtual gsl::owner<SharedData*> Create() const override;

			FIEAGameEngine::Vector<std::int32_t> Data;
		};

		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		virtual void Initialize() override;
		virtual bool StartHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value, bool isArray, size_t index) override;
		virtual bool EndHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, bool isArray) override;

	private:
		inline static const std::string IntegerKey = "integer";
		bool _parsingData = false;
	};
}