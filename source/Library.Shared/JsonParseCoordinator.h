#pragma once
#include "json/json.h"
#include "gsl/gsl"
#include "RTTI.h"
#include "Vector.h"
#include <fstream>

namespace FIEAGameEngine
{
	/// <summary>
	/// Forward declaration of IJsonParseHelper
	/// </summary>
	class IJsonParseHelper;

	/// <summary>
	/// Coordinator for Parsing Json Strings into SharedData objects.
	/// Loops through Helper classes to handle parsed Json values.
	/// </summary>
	class JsonParseCoordinator final
	{
	public:
		/// <summary>
		/// Class for storing data that has been received from parsed Json values.
		/// Shared between both the JsonParseCoordinator and the Helper classes.
		/// </summary>
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI)

		public:
			/// <summary>
			/// Destructor marked as virtual so derived classes can provide an implementation.
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// Initializes the SharedData. Provided to be implemented in derived classes. Does any work before using the SharedData.
			/// </summary>
			virtual void Initialize();
			/// <summary>
			/// Used to create an instance of SharedData. Meant to be overriden in derived class.
			/// Uses heap allocation, so SharedData must be freed at some point.
			/// </summary>
			/// <returns>Pointer to the newly created SharedData.</returns>
			virtual gsl::owner<SharedData*> Create() const = 0;
			/// <summary>
			/// Return the address of the JsonParseCoordinator associated with this object.
			/// </summary>
			/// <returns>JsonParseCoordinator associated with this object.</returns>
			JsonParseCoordinator* GetJsonParseCoordinator();
			/// <summary>
			/// Return the address of the JsonParseCoordinator associated with this object. For use with const JsonParseCoordinator.
			/// </summary>
			/// <returns>Const pointer to JsonParseCoordinator associated with this object.</returns>
			const JsonParseCoordinator* GetJsonParseCoordinator() const;
			/// <summary>
			/// Sets the private pointer to the JsonParseCoordinator to the passed in pointer.
			/// </summary>
			/// <param name="coordinator">The coordinator that this SharedData points to.</param>
			void SetJsonParseCoordinator(JsonParseCoordinator* coordinator);
			/// <summary>
			/// Increment a “nesting depth” counter. This gets incremented upon the start of each element.
			/// </summary>
			void IncrementDepth();
			/// <summary>
			/// Decrement a “nesting depth” counter. This gets decremented upon the end of each element.
			/// </summary>
			void DecrementDepth();
			/// <summary>
			/// Return the current nesting depth.
			/// </summary>
			/// <returns>Current nesting depth.</returns>
			std::uint32_t Depth() const;
		private:
			/// <summary>
			/// Pointer to the JsonParseCoordinator associated with this object.
			/// </summary>
			JsonParseCoordinator* _coordinator = nullptr;
			/// <summary>
			/// Tracks the current nesting depth.
			/// </summary>
			std::uint32_t _depth = 0;
		};

	public:
		/// <summary>
		/// Creates a new instance of JsonParseCoordinator. Default constructor for JsonParseCoordinator provided by compiler.
		/// </summary>
		explicit JsonParseCoordinator() = default;
		/// <summary>
		/// Creates a new instance of JsonParseCoordinator.
		/// Associates the sharedData pointer with the JsonParseCoordinator.
		/// Associates the JsonParseCoordinator pointer (in the sharedData) with the JsonParseCoordinator.
		/// </summary>
		/// <param name="sharedData">The sharedData associated with this JsonParseCoordinator.</param>
		explicit JsonParseCoordinator(SharedData& sharedData);
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new JsonParseCoordinator and deep copies each member variable from the other JsonParseCoordinator into this JsonParseCoordinator. 
		/// </summary>
		/// <param name="other">The instance of JsonParseCoordinator that is being copied from.</param>
		JsonParseCoordinator(const JsonParseCoordinator& other) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new JsonParseCoordinator and "steals" the member variables from other JsonParseCoordinator into this JsonParseCoordinator. 
		/// Shallow copies the member variables and sets the other JsonParseCoordinator's member variables to their uninitialized defaults. 
		/// </summary>
		/// <param name="other">The instance of JsonParseCoordinator that is being moved from.</param>
		JsonParseCoordinator(JsonParseCoordinator&& other) noexcept;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this JsonParseCoordinator and deep copies each element from other JsonParseCoordinator into this JsonParseCoordinator.
		/// </summary>
		/// <param name="other">The instance of JsonParseCoordinator that is being copied from.</param>
		/// <returns>A JsonParseCoordinator reference to this object.</returns>
		JsonParseCoordinator& operator=(const JsonParseCoordinator& other) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other JsonParseCoordinator into this JsonParseCoordinator. 
		/// Clears the current JsonParseCoordinator and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other JsonParseCoordinator's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of JsonParseCoordinator that is being moved from.</param>
		/// <returns>A JsonParseCoordinator reference to the this object.</returns>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& other) noexcept;
		/// <summary>
		/// Destructor that clears and frees the memory associated with this JsonParseCoordinator.
		/// </summary>
		~JsonParseCoordinator();

		/// <summary>
		///Dduplicate this object and return its address.
		/// </summary>
		/// <returns>Pointer to the newly created JsonParseCoordinator.</returns>
		virtual gsl::owner<JsonParseCoordinator*> Clone() const;
		/// <summary>
		/// Initializes the JsonParseCoordinator. Does any work before using the JsonParseCoordinator.
		/// </summary>
		void Initialize();
		/// <summary>
		/// Given a reference to an IJsonParseHelper object, add it to the _helpers list.
		/// </summary>
		/// <param name="helper">The helper to be added to the _helpers list.</param>
		void AddHelper(IJsonParseHelper& helper);
		/// <summary>
		/// Given a reference to an IJsonParseHelper object, remove it from the _helpers list.
		/// </summary>
		/// <param name="helper">The helper to be removed from the _helpers list.</param>
		void RemoveHelper(IJsonParseHelper& helper);
		/// <summary>
		/// Converts the Json string provided into a stringstream and calls the Parse method for streams.
		/// </summary>
		/// <param name="input">A string representing valid Json.</param>
		void Parse(const std::string& input);
		/// <summary>
		/// Converts the Json stringstream into a Json::Value object and calls ParseMembers on that value.
		/// </summary>
		/// <param name="input">A stringstream representing valid Json.</param>
		void Parse(std::istream& stream);
		/// <summary>
		/// Converts the content of a Json file into a stringstream and calls the Parse method for streams.
		/// </summary>
		/// <param name="input">A stringstream representing valid Json.</param>
		void ParseFromFile(const std::string& name);
		/// <summary>
		/// Return the path for the file being parsed, passed into ParseFromFile.
		/// </summary>
		/// <returns>The path for the file being parsed.</returns>
		std::string& GetFileName();
		/// <summary>
		/// Return the path for the file being parsed, passed into ParseFromFile.
		/// For use on a const JsonParseCoordinator.
		/// </summary>
		/// <returns>The path for the file being parsed.</returns>
		const std::string& GetFileName() const;
		/// <summary>
		/// Return the address of the SharedData associated with this object.
		/// </summary>
		/// <returns>The address of the SharedData associated with this object.</returns>
		SharedData* GetSharedData();
		/// <summary>
		/// Return the address of the SharedData associated with this object.
		/// For use with a const JsonParseCoordinator.
		/// </summary>
		/// <returns>The address of the SharedData associated with this object.</returns>
		const SharedData* GetSharedData() const;
		/// <summary>
		/// Given the address of a SharedData object, associate it with this object.
		/// </summary>
		/// <param name="sharedData">Address of a SharedData object to be associated with this object.</param>
		void SetSharedData(SharedData* sharedData);
		/// <summary>
		/// Returns the array of helpers (IJsonParseHelpers)
		/// </summary>
		/// <returns>The array of IJsonParseHelpers.</returns>
		const Vector<IJsonParseHelper*>& Helpers() const;
		/// <summary>
		/// Checks if this JsonParseCoordinator is a clone.
		/// </summary>
		/// <returns>The private member that tracks if it is a clone.</returns>
		bool IsClone();
		
	private:
		/// <summary>
		/// For the Json value passed in, calls Parse on all the keys and values within.
		/// Also accounts for increment and decrement of depth.
		/// </summary>
		/// <param name="value">The current Json value that is being parsed.</param>
		void ParseMembers(const Json::Value& value);
		/// <summary>
		/// When parsing a Json value, loops through the helper functions and calls StartHandler to see if it can handle to value.
		/// If it can handle the value, call EndHandler and check if it is an object.
		/// Helper function to reduce code duplication in Parse function.
		/// </summary>
		/// <param name="key">The key for the current Json value.</param>
		/// <param name="value">The Json value that is being parsed.</param>
		/// <param name="isArray">Whether the value is an array or not.</param>
		void HandlerLoop(const std::string& key, const Json::Value& value, bool isArray, size_t index);
		/// <summary>
		/// Parses a Json value. Calls helper loop to parse the value.
		/// Accounts for arrays by calling helper on each item in the array.
		/// </summary>
		/// <param name="key">The key for the current Json value.</param>
		/// <param name="value">The Json value that is being parsed.</param>
		/// <param name="isArray">Whether the value is an array or not.</param>
		void Parse(const std::string& key, const Json::Value& value, bool isArray);

		/// <summary>
		/// Whether this object is a clone or not.
		/// </summary>
		bool _cloned = false;
		/// <summary>
		/// The filename for the Json file that is being parsed.
		/// </summary>
		std::string _filename;
		/// <summary>
		/// The address of the Shared Data associated with this object.
		/// </summary>
		SharedData* _sharedData = nullptr;
		/// <summary>
		/// The vector of helpers that will process the parsing of Json values.
		/// </summary>
		Vector<IJsonParseHelper*> _helpers;
	};
}