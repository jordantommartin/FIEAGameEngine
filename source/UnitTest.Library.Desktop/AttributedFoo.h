#pragma once
#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"

namespace UnitTests
{
	/// <summary>
	/// An example class that derives from Attributed in order to test the functionality of Attributed.
	/// </summary>
	class AttributedFoo final : public FIEAGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, FIEAGameEngine::Attributed)

	public:
		/// <summary>
		/// Variable used for testing array attributes. Determines the size of those arrays.
		/// </summary>
		static const std::size_t ArraySize = 5;
		/// <summary>
		/// Creates a new instance of AttributedFoo. 
		/// </summary>
		AttributedFoo();
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new AttributedFoo and deep copies each member variable from the other AttributedFoo into this AttributedFoo. 
		/// </summary>
		/// <param name="other">The instance of AttributedFoo that is being copied from.</param>
		AttributedFoo(const AttributedFoo&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new AttributedFoo and "steals" the member variables from other AttributedFoo into this AttributedFoo. 
		/// Shallow copies the member variables and sets the other AttributedFoo's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of AttributedFoo that is being moved from.</param>
		AttributedFoo(AttributedFoo&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this AttributedFoo and deep copies each element from other AttributedFoo into this AttributedFoo.
		/// </summary>
		/// <param name="other">The instance of AttributedFoo that is being copied from.</param>
		/// <returns>A AttributedFoo reference to this object.</returns>
		AttributedFoo& operator=(const AttributedFoo&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other AttributedFoo into this AttributedFoo. 
		/// Clears the current AttributedFoo and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other AttributedFoo's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of AttributedFoo that is being moved from.</param>
		/// <returns>A AttributedFoo reference to the this object.</returns>
		AttributedFoo& operator=(AttributedFoo&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this AttributedFoo.
		/// </summary>
		~AttributedFoo() = default;

		/// <summary>
		/// Example integer used to create a prescribed attribute.
		/// </summary>
		int ExternalInteger = 0;
		/// <summary>
		/// Example float used to create a prescribed attribute.
		/// </summary>
		float ExternalFloat = 0.f;
		/// <summary>
		/// Example string used to create a prescribed attribute.
		/// </summary>
		std::string ExternalString;
		/// <summary>
		/// Example vector used to create a prescribed attribute.
		/// </summary>
		glm::vec4 ExternalVector;
		/// <summary>
		/// Example matrix used to create a prescribed attribute.
		/// </summary>
		glm::mat4 ExternalMatrix;

		/// <summary>
		/// Example integer array used to create a prescribed attribute.
		/// </summary>
		int ExternalIntegerArray[ArraySize];
		/// <summary>
		/// Example float array used to create a prescribed attribute.
		/// </summary>
		float ExternalFloatArray[ArraySize];
		/// <summary>
		/// Example string array used to create a prescribed attribute.
		/// </summary>
		std::string ExternalStringArray[ArraySize];
		/// <summary>
		/// Example vector array used to create a prescribed attribute.
		/// </summary>
		glm::vec4 ExternalVectorArray[ArraySize];
		/// <summary>
		/// Example matrix array used to create a prescribed attribute.
		/// </summary>
		glm::mat4 ExternalMatrixArray[ArraySize];

		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an Attributed Foo.
		/// </summary>
		/// <returns>An Attributed Foo pointer to the heap allocated memory.</returns>
		gsl::owner<AttributedFoo*> Clone() const override;
		/// <summary>
		/// Determines if two AttributedFoos are equal to one another.
		/// </summary>
		/// <param name="rhs">The other AttributedFoo which is being compared to this AttributedFoo.</param>
		/// <returns>Whether the two AttributedFoos are equal to each other.</returns>
		bool Equals(const RTTI* rhs) const override;
		/// <summary>
		/// Provides a string representaion of this AttributedFoo.
		/// </summary>
		/// <returns>The string representation of this AttributedFoo.</returns>
		std::string ToString() const override;
		
		/// <summary>
		/// Provides the Signatures Vector for an AttributedFoo with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an AttributedFoo.</returns>
		static const FIEAGameEngine::Vector<FIEAGameEngine::Signature> Signatures();
	};
}