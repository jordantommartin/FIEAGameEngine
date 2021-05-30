#pragma once
#include <cstdint>
#include "JsonParseCoordinator.h"
#include "json/json.h"
#include <gsl/gsl>

namespace FIEAGameEngine
{
	/// <summary>
	/// Interface that will provide the methods that will be implemented in derived JsonParseHelpers.
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)

	public:
		/// <summary>
		/// Does any setup work required of the helper. Gets call before each file is parsed.
		/// </summary>
		virtual void Initialize();
		/// <summary>
		/// Overridden implementations will create an instance of the helper. This is a so-called “virtual constructor”.
		/// </summary>
		/// <returns>An instance of the helper</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;
		/// <summary>
		/// Attempts to handle Json Key/Value pair. 
		/// Derived classes will implement this function to check if the Json key is correct and save the Json Value to the SharedData provided.
		/// </summary>
		/// <param name="sharedData">The SharedData associated with the JsonParseCoordinator you are wanting to use with this helper.</param>
		/// <param name="key">The key for the current Json value.</param>
		/// <param name="value">The Json value that is being parsed.</param>
		/// <param name="isArray">Whether the value is an array or not.</param>
		/// <returns>Whether the pair was handled.</returns>
		virtual bool StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value, bool isArray, size_t index) = 0;
		/// <summary>
		/// Attempts to complete the handling of a Json Key/Value pair. 
		/// Derived classes will implement this function to check if the pair was handled correctly.
		/// </summary>
		/// <param name="sharedData">The SharedData associated with the JsonParseCoordinator you are wanting to use with this helper.</param>
		/// <param name="key">The key for the current Json value.</param>
		/// <param name="value">The Json value that is being parsed.</param>
		/// <param name="isArray">Whether the value is an array or not.</param>
		/// <returns>Whether the pair was handled.</returns>
		virtual bool EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, bool isArray) = 0;
		/// <summary>
		/// Destructor marked as virtual so derived classes can create their own implementation.
		/// </summary>
		virtual ~IJsonParseHelper();
	};
}