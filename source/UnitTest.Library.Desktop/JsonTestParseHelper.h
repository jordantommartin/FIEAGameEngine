#pragma once
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"

namespace UnitTests
{
	class JsonTestParseHelper final : public FIEAGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTestParseHelper, FIEAGameEngine::IJsonParseHelper);

	public:
		class SharedData final : public FIEAGameEngine::JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FIEAGameEngine::JsonParseCoordinator::SharedData)

		public:
			virtual void Initialize() override;
			virtual gsl::owner<SharedData*> Create() const override;

			std::uint32_t MaxDepth = 0;
		};

		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		virtual void Initialize() override;
		virtual bool StartHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value, bool wasHandled, size_t) override;
		virtual bool EndHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, bool isArray) override;

		bool InitializeCalled = false;
		std::uint32_t _startHandlerCount = 0;
		std::uint32_t _endHandlerCount = 0;
		std::uint32_t _arrayElementCount = 0;
	};
}