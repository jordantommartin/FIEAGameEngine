#pragma once
#include "Attributed.h"
#include "Vector.h"
#include "Factory.h"

namespace UnitTests
{
	class Power : public FIEAGameEngine::Attributed
	{
		RTTI_DECLARATIONS(Power, FIEAGameEngine::Attributed)

	public:
		/// <summary>
		/// Creates a new instance of Power. 
		/// </summary>
		Power();
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new Power and deep copies each member variable from the other Power into this Power. 
		/// </summary>
		/// <param name="other">The instance of Power that is being copied from.</param>
		Power(const Power&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new Power and "steals" the member variables from other Power into this Power. 
		/// Shallow copies the member variables and sets the other Power's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of Power that is being moved from.</param>
		Power(Power&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this Power and deep copies each element from other Power into this Power.
		/// </summary>
		/// <param name="other">The instance of Power that is being copied from.</param>
		/// <returns>A Power reference to this object.</returns>
		Power& operator=(const Power&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Power into this Power. 
		/// Clears the current Power and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other Power's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of Power that is being moved from.</param>
		/// <returns>A Power reference to the this object.</returns>
		Power& operator=(Power&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this Power.
		/// </summary>
		~Power() = default;

		std::string Name;
		float Dps;
		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an Power.
		/// </summary>
		/// <returns>An Power pointer to the heap allocated memory.</returns>
		gsl::owner<Power*> Clone() const override;
		/// <summary>
		/// Determines if two Powers are equal to one another.
		/// </summary>
		/// <param name="rhs">The other Power which is being compared to this Power.</param>
		/// <returns>Whether the two Powers are equal to each other.</returns>
		bool Equals(const RTTI* rhs) const override;
		/// <summary>
		/// Provides a string representaion of this Power.
		/// </summary>
		/// <returns>The string representation of this Power.</returns>
		std::string ToString() const override;

		/// <summary>
		/// Provides the Signatures Vector for an Power with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an Power.</returns>
		static const FIEAGameEngine::Vector<FIEAGameEngine::Signature> Signatures();
	};

	ConcreteFactory(Power, FIEAGameEngine::Scope)
}