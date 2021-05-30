#pragma once
#include "RTTI.h"
#include "Factory.h"

namespace UnitTests
{
	/// <summary>
	/// Example of user defined class. Contains a single piece of data.
	/// </summary>
	class Bar : public FIEAGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Bar, RTTI);
	public:
		/// <summary>
		/// Create a new instance of Bar.
		/// </summary>
		/// <param name="value">Provides an initial value for the Data() member.</param>
		explicit Bar(int value = 0);
		/// <summary>
		/// Invokes the copy constructor. Creates a new Bar and deep copies each member variable from other Bar into this Bar.
		/// </summary>
		/// <param name="other">The instance of Bar that is being copied from.</param>
		Bar(const Bar& other);
		/// <summary>
		/// Invokes the move constructor. Creates a new Bar and "steals" the member variables from other Bar into this Bar. 
		/// Shallow copies the member variables and sets the other Bar's member variables to nullptr.
		/// </summary>
		/// <param name="other">The instance of Bar that is being moved from.</param>
		Bar(Bar&& other) noexcept;
		/// <summary>
		/// Invokes the copy assignment. Deep copies each member variable from other Bar into this Bar.
		/// </summary>
		/// <param name="other">The instance of Bar that is being copied from.</param>
		/// <returns>A Bar reference to the this object.</returns>
		Bar& operator=(const Bar& other);
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Bar into this Bar. 
		/// Shallow copies the member variables and sets the other Bar's member variables to nullptr.
		/// </summary>
		/// <param name="other">The instance of Bar that is being copied from.</param>
		/// <returns>A Bar reference to the this object.</returns>
		Bar& operator=(Bar&& other) noexcept;
		/// <summary>
		/// Equality operator. Tests whether a Bar is equal to another.
		/// </summary>
		/// <param name=""></param>
		/// <returns>A boolean representing whether equality test was true.</returns>
		~Bar();
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

	private:
		/// <summary>
		/// Internal data member pointer.
		/// </summary>
		int* _data;
	};

	ConcreteFactory(Bar, FIEAGameEngine::RTTI)
}


