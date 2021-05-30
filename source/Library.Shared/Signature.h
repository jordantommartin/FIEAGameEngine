#pragma once
#include "Datum.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Signature structure used to define the parts of every Attribute that will be populated.
	/// </summary>
	struct Signature
	{
		/// <summary>
		/// The name of Attribute.
		/// </summary>
		std::string Name;
		/// <summary>
		/// The type of the Datum that holds the Attribute.
		/// </summary>
		Datum::DatumTypes Type;
		/// <summary>
		/// The amount of elements in the Datum that holds the Attributed.
		/// </summary>
		size_t Size;
		/// <summary>
		/// The amount of bytes from the pointer to this object to the Attribute member in the class.
		/// </summary>
		size_t StorageOffset;
	};
}