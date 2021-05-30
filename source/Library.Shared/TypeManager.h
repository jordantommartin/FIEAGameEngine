#pragma once
#include "Attributed.h"
#include "Signature.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// A class that contains a static hashmap used for storing all the signatures associated with each type.
	/// RTTI typeIds are associated with Vectors of Signatures. See the signatures header for more information on this.
	/// </summary>
	class TypeManager final
	{
	public:
		/// <summary>
		/// Constructor is deleted because the members and methods of this class are static.
		/// It should be impossible to create multiple TypeManagers.
		/// </summary>
		TypeManager() = delete;
		/// <summary>
		/// Copy Constructor is deleted because the members and methods of this class are static.
		/// It should be impossible to create multiple TypeManagers.
		/// </summary>
		TypeManager(const TypeManager&) = delete;
		/// <summary>
		/// Move Constructor is deleted because the members and methods of this class are static.
		/// It should be impossible to create multiple TypeManagers.
		/// </summary>
		TypeManager(TypeManager&&) = delete;
		/// <summary>
		/// Copy Assignment is deleted because the members and methods of this class are static.
		/// It should be impossible to create multiple TypeManagers.
		/// </summary>
		TypeManager& operator=(const TypeManager&) = delete;
		/// <summary>
		/// Move Assignment is deleted because the members and methods of this class are static.
		/// It should be impossible to create multiple TypeManagers.
		/// </summary>
		TypeManager&& operator=(const TypeManager&&) = delete;
		/// <summary>
		/// Default destructor provided by compiler.
		/// </summary>
		~TypeManager() = default;

		/// <summary>
		/// Provides a copy of the Signature Vector associated with the type that was passed in.
		/// </summary>
		/// <param name="typeId">The RTTI typeId for the type that we are returning the signatures for.</param>
		/// <returns>The signature vector associated with the type.</returns>
		static Vector<Signature> GetSignaturesForType(RTTI::IdType typeId);
		/// <summary>
		/// Adds a type to the TypeManager. 
		/// Stores an entry into the static hashmap with a typeId key and a signature vector value.
		/// </summary>
		/// <param name="typeId">The RTTI typeId for the type you are adding.</param>
		/// <param name="signatures">The vector of signatures for the type you are adding.</param>
		static void AddType(RTTI::IdType typeId, Vector<Signature> signatures);
		/// <summary>
		/// Removes a type from the TypeManager. 
		/// Removes an entry in the static hashmap with a typeId key.
		/// </summary>
		/// <param name="typeId">The RTTI typeId for the type you are adding.</param>
		static void RemoveType(RTTI::IdType typeId);
		/// <summary>
		/// Checks whether the provided type is contained in the TypeManager hashmap.
		/// </summary>
		/// <param name="typeId">The RTTI typeId associated with the type you are checking for.</param>
		/// <returns>Whether the type is in the TypeManager.</returns>
		static bool ContainsType(RTTI::IdType typeId);
		/// <summary>
		/// Returns the static hashmap that contains all RTTI typeId, Signature Vector pairs.
		/// </summary>
		/// <returns>The static hashmap that contains all RTTI typeId, Signature Vector pairs.</returns>
		static const HashMap<RTTI::IdType, Vector<Signature>>& Types();
		/// <summary>
		/// Clears all types from the TypeManager.
		/// Removes all RTTI typeId, Signature Vector pairs from the hashmap.
		/// </summary>
		static void Clear();
	private:
		/// <summary>
		/// Stores all signatures for each type in the type manager.
		/// The static hashmap which associate RTTI IdTypes with Signature Vectors. 
		/// </summary>
		inline static HashMap<RTTI::IdType, Vector<Signature>> _signatureMap;
	};

}