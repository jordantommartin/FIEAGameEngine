#include "pch.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(JsonParseCoordinator::SharedData)

#pragma region SharedData

	void JsonParseCoordinator::SharedData::Initialize()
	{

	}

	JsonParseCoordinator* JsonParseCoordinator::SharedData::GetJsonParseCoordinator()
	{
		return _coordinator;
	}

	const JsonParseCoordinator* JsonParseCoordinator::SharedData::GetJsonParseCoordinator() const
	{
		return _coordinator;
	}

	void JsonParseCoordinator::SharedData::IncrementDepth()
	{
		if (_depth < UINT32_MAX)
		{
			++_depth;
		}
	}

	void JsonParseCoordinator::SharedData::DecrementDepth()
	{
		if (_depth > 0)
		{
			--_depth;
		}
	}

	std::uint32_t JsonParseCoordinator::SharedData::Depth() const
	{
		return _depth;
	}

	void JsonParseCoordinator::SharedData::SetJsonParseCoordinator(JsonParseCoordinator* coordinator)
	{
		_coordinator = coordinator;
	}

#pragma endregion SharedData

#pragma region JsonParseCoordinator

	JsonParseCoordinator::JsonParseCoordinator(SharedData& sharedData) :
		_sharedData(&sharedData)
	{
		_sharedData->SetJsonParseCoordinator(this);
	}

	JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& other) noexcept :
		_cloned(other._cloned), _filename(std::move(other._filename)), _sharedData(std::move(other._sharedData)), _helpers(std::move(other._helpers))
	{
		_sharedData->SetJsonParseCoordinator(this);
	}

	JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& other) noexcept
	{
		if (this != &other)
		{
			_cloned = other._cloned;
			_filename = std::move(other._filename);
			_sharedData = other._sharedData;
			_helpers = std::move(other._helpers);
			
			_sharedData->SetJsonParseCoordinator(this);

			other._sharedData = nullptr;
			other._cloned = false;
		}

		return *this;
	}

	JsonParseCoordinator::~JsonParseCoordinator()
	{
		if (IsClone())
		{
			for (auto helper : _helpers)
			{
				RemoveHelper(*helper);
			}

			delete _sharedData;
		}
	}

	gsl::owner<JsonParseCoordinator*> JsonParseCoordinator::Clone() const
	{
		JsonParseCoordinator* clone = new JsonParseCoordinator();

		for (auto helper : _helpers)
		{
			IJsonParseHelper* newHelper = helper->Create();
			clone->AddHelper(*newHelper);
		}

		clone->_cloned = true;
		clone->_filename = _filename;
		clone->_sharedData = _sharedData->Create();
		clone->_sharedData->SetJsonParseCoordinator(clone);

		return clone;
	}

	void JsonParseCoordinator::Initialize()
	{
		for (auto helper : _helpers)
		{
			helper->Initialize();
		}
		_sharedData->Initialize();
	}

	void JsonParseCoordinator::AddHelper(IJsonParseHelper& helper)
	{
		if (_cloned)
		{
			throw std::runtime_error("You cannot add a helper to a cloned JsonParseCoordinator. It would no longer be a clone.");
		}
		_helpers.PushBack(&helper);
	}

	void JsonParseCoordinator::RemoveHelper(IJsonParseHelper& helper)
	{
		bool wasRemoved = _helpers.Remove(&helper);
		if (IsClone() && wasRemoved)
		{
			delete& helper;
		}
	}

	void JsonParseCoordinator::Parse(const std::string& input)
	{
		std::istringstream inputStream(input);
		Parse(inputStream);
	}

	void JsonParseCoordinator::ParseFromFile(const std::string& filename)
	{
		std::ifstream fileStream(filename);
		if (!fileStream.good())
		{
			throw std::invalid_argument("File could not be opened.");
		}
		
		Parse(fileStream);
		_filename = filename;
	}

	void JsonParseCoordinator::Parse(std::istream& stream)
	{
		Initialize();

		Json::Value root;
		stream >> root;

		ParseMembers(root);
	}

	const Vector<IJsonParseHelper*>& JsonParseCoordinator::Helpers() const
	{
		return _helpers;
	}

	bool JsonParseCoordinator::IsClone()
	{
		return _cloned;
	}

	std::string& JsonParseCoordinator::GetFileName()
	{
		return _filename;
	}
	const std::string& JsonParseCoordinator::GetFileName() const
	{
		return const_cast<const std::string&>(_filename);
	}

	JsonParseCoordinator::SharedData* JsonParseCoordinator::GetSharedData()
	{
		return _sharedData;
	}

	const JsonParseCoordinator::SharedData* JsonParseCoordinator::GetSharedData() const
	{
		return _sharedData;
	}

	void JsonParseCoordinator::SetSharedData(SharedData* sharedData)
	{
		if (_cloned)
		{
			throw std::runtime_error("You cannot add a set the SharedData on a cloned JsonParseCoordinator. It would no longer be a clone.");
		}
		_sharedData = sharedData;
	}

	void JsonParseCoordinator::ParseMembers(const Json::Value& value)
	{
		if (value.size() > 0)
		{
			const std::vector<std::string> keys = value.getMemberNames();

			_sharedData->IncrementDepth();
			for (auto& key : keys) {
				bool isArray = value[key].isArray();
				Parse(key, value[key], isArray);
			}
			_sharedData->DecrementDepth();
		}

	}

	void JsonParseCoordinator::HandlerLoop(const std::string& key, const Json::Value& value, bool isArray, size_t index)
	{
		
		for (auto helper : _helpers)
		{
			if (helper->StartHandler(*_sharedData, key, value, isArray, index))
			{
				if (value.isObject())
				{
					ParseMembers(value);
				}
				helper->EndHandler(*_sharedData, key, isArray);
				break;
			}
		}
	}

	void JsonParseCoordinator::Parse(const std::string& key, const Json::Value& value, bool isArray)
	{
		if (isArray)
		{
			for (Json::ArrayIndex i = 0; i < value.size(); i++)
			{
				HandlerLoop(key, value[i], isArray, i);
			}
		}
		else
		{
			HandlerLoop(key, value, isArray, 0_z);
		}
	}
#pragma endregion

}

