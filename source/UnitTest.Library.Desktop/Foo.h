#pragma once
#include "RTTI.h"

namespace UnitTests
{
	/// <summary>
	/// Example of user defined class. Contains a single piece of data.
	/// </summary>
	class Foo : public FIEAGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI)

	public:
		/// <summary>
		/// Create a new instance of Foo.
		/// </summary>
		/// <param name="value">Provides an initial value for the Data() member.</param>
		explicit Foo(int value = 0);
		/// <summary>
		/// Invokes the copy constructor. Creates a new Foo and deep copies each member variable from other Foo into this Foo.
		/// </summary>
		/// <param name="other">The instance of Foo that is being copied from.</param>
		Foo(const Foo& other);
		/// <summary>
		/// Invokes the move constructor. Creates a new Foo and "steals" the member variables from other Foo into this Foo. 
		/// Shallow copies the member variables and sets the other Foo's member variables to nullptr.
		/// </summary>
		/// <param name="other">The instance of Foo that is being moved from.</param>
		Foo(Foo&& other) noexcept;
		/// <summary>
		/// Invokes the copy assignment. Deep copies each member variable from other Foo into this Foo.
		/// </summary>
		/// <param name="other">The instance of Foo that is being copied from.</param>
		/// <returns>A Foo reference to the this object.</returns>
		Foo& operator=(const Foo& other);
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Foo into this Foo. 
		/// Shallow copies the member variables and sets the other Foo's member variables to nullptr.
		/// </summary>
		/// <param name="other">The instance of Foo that is being copied from.</param>
		/// <returns>A Foo reference to the this object.</returns>
		Foo& operator=(Foo&& other) noexcept;
		/// <summary>
		/// Equality operator. Tests whether a Foo is equal to another.
		/// </summary>
		/// <param name="other">The instance of Foo that we are comparing against.</param>
		/// <returns>A boolean representing whether equality test was true.</returns>
		bool operator==(const Foo&) const;
		/// <summary>
		/// Not Equality operator. Tests whether a Foo is not equal to another.
		/// </summary>
		/// <param name="other">The instance of Foo that we are comparing against.</param>
		/// <returns>A boolean representing whether equality test was not true.</returns>
		bool operator!=(const Foo&) const;
		/// <summary>
		/// Destructor that deletes any member variables
		/// </summary>
		~Foo();
		/// <summary>
		/// Provides the internal data member as a reference.
		/// </summary>
		/// <returns>A mutable reference to the internal data member.</returns>
		/// <exception cref="std::runtime_error">Thrown if _data is nullptr (only possible if instance has been moved).</exception>
		/// <remarks> Moving an instance invalidates this member.</remarks>
		int& Data();
		/// <summary>
		///  Provides the internal data member as an int. This requires a copy.
		/// </summary>
		/// <returns>A copy of the internal data member.</returns>
		int Data() const;

		/// <summary>
		/// Overrides the RTTI Equals method.
		/// </summary>
		/// <param name="rhs">The other RTTI this is being compared to.</param>
		/// <returns>Whether the two are equal to each other.</returns>
		bool Equals(const RTTI* rhs) const override;
		/// <summary>
		/// Overrides the RTTI To String method
		/// </summary>
		/// <returns>The data as a string representation.</returns>
		std::string ToString() const override;

	private:
		/// <summary>
		/// Internal data member pointer.
		/// </summary>
		int* _data;
	};
}


