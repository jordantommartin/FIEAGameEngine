#pragma once
#include "Scope.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "HashMap.h"
#include "Stack.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Class used as a helper for the JsonParseCoordinator. 
	/// This helper can handle tables and uses a table format when parsing Json files.
	/// </summary>
	class JsonTableParseHelper final : public FIEAGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, FIEAGameEngine::IJsonParseHelper)

	public:
		/// <summary>
		/// The SharedData implementation for JsonTableParseHelper. Stores a Scope pointer to refer to the root of the Json object.
		/// </summary>
		class SharedData final : public FIEAGameEngine::JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FIEAGameEngine::JsonParseCoordinator::SharedData)

		public:
			/// <summary>
			/// Default SharedData constructor to create an empty SharedData.
			/// </summary>
			explicit SharedData() = default;
			/// <summary>
			/// Constructor used to created a SharedData and populate it with the passed in Scope.
			/// </summary>
			/// <param name="scope">The scope that you want to be the Root of this SharedData.</param>
			explicit SharedData(Scope& scope);

			/// <summary>
			/// Performs any start up work associated with this SharedData.
			/// </summary>
			void Initialize() override;
			/// <summary>
			/// Creates a new instance of this type of SharedData on the heap.
			/// </summary>
			/// <returns>The new instance of the SharedData.</returns>
			gsl::owner<SharedData*> Create() const override;
			/// <summary>
			/// Return the address of the Scope associated with this object. For use with const Scope.
			/// </summary>
			/// <returns>Const pointer to Scope associated with this object.</returns>
			Scope* GetScope();
			/// <summary>
			/// Return the address of the Scope associated with this object. For use with const Scope.
			/// </summary>
			/// <returns>Const pointer to Scope associated with this object.</returns>
			const Scope* GetScope() const;
			/// <summary>
			/// Sets the private pointer to the Scope to the passed in pointer.
			/// </summary>
			/// <param name="coordinator">The coordinator that this Scope points to.</param>
			void SetScope(Scope* scope);
			/// <summary>
			/// The Scope pointer that represents the root of all the Json data that will be parsed into this SharedData.
			/// </summary>
			Scope* Root = nullptr;
		};

		/// <summary>
		/// Creates a new instance of this type of JsonTableParseHelper on the heap.
		/// </summary>
		/// <returns>The new instance of the JsonTableParseHelper</returns>
		gsl::owner<IJsonParseHelper*> Create() const override;
		/// <summary>
		/// Performs any start up work associated with this JsonTableParseHelper.
		/// </summary>
		void Initialize() override; 
		/// <summary>
		/// Handles the current key/value pair of Json. For tables, this can either be a type, value, or attribute name.
		/// </summary>
		/// <returns>Whether the pair was handled successfully.</returns>
		bool StartHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value, bool isArray, size_t index) override;
		/// <summary>
		/// Completes the handling of the current key/value pair of Json. For tables, this can either be a type, value, or attribute name.
		/// </summary>
		/// <returns>Whether the handling of the pair was completed successfully.</returns>
		bool EndHandler(FIEAGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, bool isArray) override;

	private:
		/// <summary>
		/// Shorthand way to reference "type" string.
		/// </summary>
		inline static const std::string TypeKey = "type";
		/// <summary>
		/// Shorthand way to reference "value" string.
		/// </summary>
		inline static const std::string ValueKey = "value";
		/// <summary>
		/// Shorthand way to reference "class" string.
		/// </summary>
		inline static const std::string ClassKey = "class";
		/// <summary>
		/// Shorthand way to reference "Scope" string.
		/// </summary>
		inline static std::string ScopeClassName = "Scope";

		/// <summary>
		/// Class that is used to keep track of the current context of scope pointers and keys when parsing Json data
		/// using Start and End Handler calls.
		/// </summary>
		class StackFrame
		{
		public:
			/// <summary>
			/// Constructor to create a StackFrame. Cannot create one without populating its members.
			/// </summary>
			/// <param name="scope">The current root of this Stack Frame.</param>
			/// <param name="datum">The current datum of this Stack Frame.</param>
			/// <param name="key">The current key of this Stack Frame.</param>
			StackFrame(Scope& scope, Datum& datum, std::string& className, const std::string& key);

			/// <summary>
			/// The current scope pointer that is associated with this Stack Frame.
			/// </summary>
			Scope& _currentRoot;
			/// <summary>
			/// The current datum reference associated with this Stack Frame.
			/// </summary>
			Datum& _currentDatum;

			std::string _className;
			/// <summary>
			/// The key that is being parsed in this Stack Frame.
			/// </summary>
			const std::string& _key;
		};

		/// <summary>
		/// Stack data structure used to hold StackFrame objects, so they can be pushed and popped according to Start and End Handler calls.
		/// </summary>
		Stack<StackFrame> _contextStack;
	};
}