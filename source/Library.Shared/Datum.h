#pragma once
#include <glm/glm.hpp>
#include <string>
#include <functional>
#include "DefaultIncrement.h"
#include "DefaultEquality.h"
#include "RTTI.h"
#include "HashMap.h"

using namespace std;

namespace FIEAGameEngine
{
	class Scope;
	class Attributed;

	/// <summary>
	/// Data structure for an array of values that have either a primitive or user-defined type that can be defined at runtime.
	/// </summary>
	class Datum final
	{
		friend Scope;
	public:
		/// <summary>
		/// The types of values that can be stored in a datum.
		/// </summary>
		enum class DatumTypes
		{
			Integer, // Signed Integer
			Float, // Float
			String, // STL std::string
			Vector, // 4-vector (the math kind, not the STL container)
			Matrix, // 4x4 matrix
			Table, // Scope class
			Pointer, // pointer to any RTTI instance.
			Unknown // has no values yet
		};

		/// <summary>
		/// Creates a new instance of Datum. No memory is allocated. Sets the type to the optional parameters (Unknown if nothing is provided).
		/// </summary>
		/// <param name="type">Gives user the option to specify a type upfront. Default value is Unknown.</param>
		explicit Datum(DatumTypes type = DatumTypes::Unknown);
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new Datum and deep copies each member variable from the other Datum into this Datum. 
		/// </summary>
		/// <param name="other">The instance of Datum that is being copied from.</param>
		Datum(const Datum& other);
		/// <summary>
		/// Invokes the move constructor. Creates a new Datum and "steals" the member variables from other Datum into this Datum. 
		/// Shallow copies the member variables and sets the other Datum's member variables to their uninitialized defaults. 
		/// </summary>
		/// <param name="other">The instance of Datum that is being moved from.</param>
		Datum(Datum&& other) noexcept;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this Datum and deep copies each element from other Datum into this Datum.
		/// If the Datum that is being copied from is bigger, that amount of memory will be reserved ahead of time.
		/// If the Datum that is being copied from is smaller, the amount of memory reserved will stay the same.
		/// </summary>
		/// <param name="other">The instance of Datum that is being copied from.</param>
		/// <returns>A Datum reference to this object.</returns>
		Datum& operator=(const Datum& other);
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Datum into this Datum. 
		/// Clears the current Datum and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other Datum's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of Datum that is being moved from.</param>
		/// <returns>A Datum reference to the this object.</returns>
		Datum& operator=(Datum&& other) noexcept;
		/// <summary>
		/// Destructor that clears and frees the memory associated with this Datum.
		/// </summary>
		~Datum();

		/// <summary>
		/// Equality operator. Tests whether a Datum is equal to another.
		/// </summary>
		/// <param name="other">The instance of Datum that we are comparing against.</param>
		/// <returns>Whether the two datums are equal.</returns>
		bool operator==(const Datum& other) const;
		/// <summary>
		/// Not Equality operator. Tests whether a Datum is not equal to another.
		/// </summary>
		/// <param name="other">The instance of Datum that we are comparing against.</param>
		/// <returns>Whether the two datums are not equal.</returns>
		/// <exception cref="std::runtime_error">Thrown if either of the Datum's have an Unknown type.</exception>
		bool operator!=(const Datum& other) const;
		/// <summary>
		/// Operator [] overload to return the Scope at that index in the Datum
		/// </summary>
		/// <param name="index">The index in the Datum where the Scope is at.</param>
		/// <returns>Reference to the Scope at that index in the Datum.</returns>
		Scope& operator[](size_t index);

#pragma region InitializerList Constructor Overloads
		/// <summary>
		/// Invokes the copy constructor on an initializer list of integers. 
		/// Creates a new Datum and deep copies each member variable from the initialzer list into this Datum. 
		/// </summary>
		/// <param name="other">The initializer listthat is being copied from.</param>
		Datum(std::initializer_list<int> list);
		/// <summary>
		/// Invokes the copy constructor on an initializer list of floats. 
		/// Creates a new Datum and deep copies each member variable from the initialzer list into this Datum. 
		/// </summary>
		/// <param name="other">The initializer list that is being copied from.</param>
		Datum(std::initializer_list<float> list);
		/// <summary>
		/// Invokes the copy constructor on an initializer list of strings. 
		/// Creates a new Datum and deep copies each member variable from the initialzer list into this Datum. 
		/// </summary>
		/// <param name="other">The initializer list that is being copied from.</param>
		Datum(std::initializer_list<std::string> list);
		/// <summary>
		/// Invokes the copy constructor on an initializer list of vectors. 
		/// Creates a new Datum and deep copies each member variable from the initialzer list into this Datum. 
		/// </summary>
		/// <param name="other">The initializer list that is being copied from.</param>
		Datum(std::initializer_list<glm::vec4> list);
		/// <summary>
		/// Invokes the copy constructor on an initializer list of matrices. 
		/// Creates a new Datum and deep copies each member variable from the initialzer list into this Datum. 
		/// </summary>
		/// <param name="other">The initializer list that is being copied from.</param>
		Datum(std::initializer_list<glm::mat4> list);
		/// <summary>
		/// Invokes the copy constructor on an initializer list of RTTI pointers. 
		/// Creates a new Datum and deep copies each member variable from the initialzer list into this Datum. 
		/// </summary>
		/// <param name="other">The initializer list that is being copied from.</param>
		Datum(std::initializer_list<RTTI*> list);
#pragma endregion

#pragma region Copy Constructor Overloads
		/// <summary>
		/// Invokes the copy constructor from a scalar integer. 
		/// Creates a new Datum and sets the size to 1. Sets the one element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar integer that is being copied into the Datum.</param>
		Datum(const int& other);
		/// <summary>
		/// Invokes the copy constructor from a scalar float. 
		/// Creates a new Datum and sets the size to 1. Sets the one element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar float that is being copied into the Datum.</param>
		Datum(const float& other);
		/// <summary>
		/// Invokes the copy constructor from a scalar string. 
		/// Creates a new Datum and sets the size to 1. Sets the one element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar string that is being copied into the Datum.</param>
		Datum(const std::string& other);
		/// <summary>
		/// Invokes the copy constructor from a scalar Vector. 
		/// Creates a new Datum and sets the size to 1. Sets the one element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar Vector that is being copied into the Datum.</param>
		Datum(const glm::vec4& other);
		/// <summary>
		/// Invokes the copy constructor from a scalar Matrix. 
		/// Creates a new Datum and sets the size to 1. Sets the one element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar Matrix that is being copied into the Datum.</param>
		Datum(const glm::mat4& other);
		/// <summary>
		/// Invokes the copy constructor from a scalar RTTI pointer. 
		/// Creates a new Datum and sets the size to 1. Sets the one element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar RTTI pointer that is being copied into the Datum.</param>
		Datum(RTTI*& other);
#pragma endregion

#pragma region Copy Assignment Operator Overloads
		/// <summary>
		/// Invokes the copy assignment from a scalar integer. 
		/// Resizes the Datum to be exactly 1_z size and assigns the only element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar integer that is being copied into the Datum.</param>
		/// <returns>A Datum reference to this object.</returns>
		Datum& operator=(const int& other);
		/// <summary>
		/// Invokes the copy assignment from a scalar float. 
		/// Resizes the Datum to be exactly 1_z size and assigns the only element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar float that is being copied into the Datum.</param>
		/// <returns>A Datum reference to this object.</returns>
		Datum& operator=(const float& other);
		/// <summary>
		/// Invokes the copy assignment from a scalar string. 
		/// Resizes the Datum to be exactly 1_z size and assigns the only element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar string that is being copied into the Datum.</param>
		/// <returns>A Datum reference to this object.</returns>
		Datum& operator=(const std::string& other);
		/// <summary>
		/// Invokes the copy assignment from a scalar Vector. 
		/// Resizes the Datum to be exactly 1_z size and assigns the only element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar Vector that is being copied into the Datum.</param>
		/// <returns>A Datum reference to this object.</returns>
		Datum& operator=(const glm::vec4& other);
		/// <summary>
		/// Invokes the copy assignment from a scalar Matrix. 
		/// Resizes the Datum to be exactly 1_z size and assigns the only element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar Matrix that is being copied into the Datum.</param>
		/// <returns>A Datum reference to this object.</returns>
		Datum& operator=(const glm::mat4& other);
		/// <summary>
		/// Invokes the copy assignment from a scalar Scope pointer. 
		/// Resizes the Datum to be exactly 1_z size and assigns the only element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar Scope pointer that is being copied into the Datum.</param>
		/// <returns>A Datum reference to this object.</returns>
		Datum& operator=(Scope& other);
		/// <summary>
		/// Invokes the copy assignment from a scalar RTTI pointer. 
		/// Resizes the Datum to be exactly 1_z size and assigns the only element to the value of the scalar.
		/// </summary>
		/// <param name="other">The scalar RTTI pointer that is being copied into the Datum.</param>
		/// <returns>A Datum reference to this object.</returns>
		Datum& operator=(RTTI* other);
#pragma endregion

#pragma region Comparison Operator Overloads
		/// <summary>
		/// Equality operator. Tests whether a Datum is equal to scalar integer.
		/// </summary>
		/// <param name="other">The scalar integer that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar integer are equal.</returns>
		bool operator==(const int& other) const;
		/// <summary>
		/// Not Equality operator. Tests whether a Datum is equal to scalar integer.
		/// </summary>
		/// <param name="other">The scalar integer that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar integer are equal.</returns>
		/// <exception cref="std::runtime_error">Thrown if either of the Datum's have an Unknown type.</exception>
		bool operator!=(const int& other) const;
		/// <summary>
		/// Equality operator. Tests whether a Datum is equal to scalar float.
		/// </summary>
		/// <param name="other">The scalar float that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar float are equal.</returns>
		bool operator==(const float& other) const;
		/// <summary>
		/// Not Equality operator. Tests whether a Datum is equal to scalar float.
		/// </summary>
		/// <param name="other">The scalar float that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar float are equal.</returns>
		/// <exception cref="std::runtime_error">Thrown if either of the Datum's have an Unknown type.</exception>
		bool operator!=(const float& other) const;
		/// <summary>
		/// Equality operator. Tests whether a Datum is equal to scalar string.
		/// </summary>
		/// <param name="other">The scalar string that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar string are equal.</returns>
		bool operator==(const std::string& other) const;
		/// <summary>
		/// Not Equality operator. Tests whether a Datum is equal to scalar integer.
		/// </summary>
		/// <param name="other">The scalar integer that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar integer are equal.</returns>
		/// <exception cref="std::runtime_error">Thrown if either of the Datum's have an Unknown type.</exception>
		bool operator!=(const std::string& other) const;
		/// <summary>
		/// Equality operator. Tests whether a Datum is equal to scalar Vector.
		/// </summary>
		/// <param name="other">The scalar Vector that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar Vector are equal.</returns>
		bool operator==(const glm::vec4& other) const;
		/// <summary>
		/// Not Equality operator. Tests whether a Datum is equal to scalar Vector.
		/// </summary>
		/// <param name="other">The scalar Vector that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar Vector are equal.</returns>
		/// <exception cref="std::runtime_error">Thrown if either of the Datum's have an Unknown type.</exception>
		bool operator!=(const glm::vec4& other) const;
		/// <summary>
		/// Equality operator. Tests whether a Datum is equal to scalar Matrix.
		/// </summary>
		/// <param name="other">The scalar Matrix that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar Matrix are equal.</returns>
		bool operator==(const glm::mat4& other) const;
		/// <summary>
		/// Not Equality operator. Tests whether a Datum is equal to scalar Matrix.
		/// </summary>
		/// <param name="other">The scalar Matrix that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar Matrix are equal.</returns>
		/// <exception cref="std::runtime_error">Thrown if either of the Datum's have an Unknown type.</exception>
		bool operator!=(const glm::mat4& other) const;
		/// <summary>
		/// Equality operator. Tests whether a Datum is equal to scalar Pointer.
		/// </summary>
		/// <param name="other">The scalar Pointer that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar Pointer are equal.</returns>
		bool operator==(RTTI* const & other) const;
		/// <summary>
		/// Not Equality operator. Tests whether a Datum is equal to scalar Pointer.
		/// </summary>
		/// <param name="other">The scalar Pointer that we are comparing against.</param>
		/// <returns>Whether the datum and the scalar Pointer are equal.</returns>
		/// <exception cref="std::runtime_error">Thrown if either of the Datum's have an Unknown type.</exception>
		bool operator!=(RTTI* const & other) const;
#pragma endregion

		/// <summary>
		/// Gets the type of this Datum.
		/// </summary>
		/// <returns>An enum association to the type of this datum.</returns>
		DatumTypes Type() const;
		/// <summary>
		/// Allows user to set the type of the Datum.
		/// </summary>
		/// <param name="type">The new type that the Datum will be set to.</param>
		/// <exception cref="std::runtime_error">Thrown when attempting to change a datum that has already been set to anything other than Unknown type.</exception>
		void SetType(DatumTypes type);
		/// <summary>
		/// Provides the current size of the Datum.
		/// </summary>
		/// <returns>A size_t representing the number of elements in the Datum.</returns>
		size_t Size() const;
		/// <summary>
		/// Provides the current capacity of the Datum.
		/// </summary>
		/// <returns>The amount of elements that can fit in the allocated space for the Datum.</returns>
		size_t Capacity() const;
		/// <summary>
		/// Determines if the Datum is empty or not.
		/// </summary>
		/// <returns>Whether the Datum is empty.</returns>
		bool isEmpty() const;
		/// <summary>
		/// Determines if the Datum is using external memory or not.
		/// </summary>
		/// <returns>Whether the Datum is using external memory or not.</returns>
		bool IsExternal() const;

		/// <summary>
		/// Changes the size of the Datum. 
		/// If the new size is larger, instantiates elements for the Datum up to the specified size. 
		/// If the new size is smaller, destroys elements for the Datum down to the specified size.
		/// </summary>
		/// <param name="size">The new size that the Datum will be set to.</param>
		/// <exception cref="std::runtime_error">Thrown when attempting to reserve memory with an unknown type. There is no way to know the size of an Unknown type.</exception>
		/// <exception cref="std::runtime_error">Thrown if external storage is being used.</exception>
		void Resize(size_t size);
		/// <summary>
		/// Removes all elements from the Datum. Sets the size to 0. Does not change the capacity as the memory is still reserved.
		/// </summary>
		/// <exception cref="std::runtime_error">Thrown if external storage is being used.</exception>
		void Clear();

#pragma region SetStorage
		/// <summary>
		/// Sets the data array to an externally managed array of type integer.
		/// Sets a boolean that says the Datum is managed externally.
		/// </summary>
		/// <param name="array">The externally managed data array.</param>
		/// <param name="size">The size of the externally managed data array.</param>
		/// <exception cref="std::runtime_error">Thrown if the size passed in is 0.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not Unknown or an Integer.</exception>
		/// <exception cref="std::runtime_error">Thrown if internal storage is already set.</exception>
		void SetStorage(int* array, size_t size);
		/// <summary>
		/// Sets the data array to an externally managed array of type float.
		/// Sets a boolean that says the Datum is managed externally.
		/// </summary>
		/// <param name="array">The externally managed data array.</param>
		/// <param name="size">The size of the externally managed data array.</param>
		/// <exception cref="std::runtime_error">Thrown if the size passed in is 0.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not Unknown or an Integer.</exception>
		/// <exception cref="std::runtime_error">Thrown if internal storage is already set.</exception> 
		void SetStorage(float* array, size_t size);
		/// <summary>
		/// Sets the data array to an externally managed array of type string.
		/// Sets a boolean that says the Datum is managed externally.
		/// </summary>
		/// <param name="array">The externally managed data array.</param>
		/// <param name="size">The size of the externally managed data array.</param>
		/// <exception cref="std::runtime_error">Thrown if the size passed in is 0.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not Unknown or an Integer.</exception>
		/// <exception cref="std::runtime_error">Thrown if internal storage is already set.</exception>
		void SetStorage(std::string* array, size_t size);
		/// <summary>
		/// Sets the data array to an externally managed array of type vector.
		/// Sets a boolean that says the Datum is managed externally.
		/// </summary>
		/// <param name="array">The externally managed data array.</param>
		/// <param name="size">The size of the externally managed data array.</param>
		/// <exception cref="std::runtime_error">Thrown if the size passed in is 0.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not Unknown or an Integer.</exception>
		/// <exception cref="std::runtime_error">Thrown if internal storage is already set.</exception>
		void SetStorage(glm::vec4* array, size_t size);
		/// <summary>
		/// Sets the data array to an externally managed array of type matrix.
		/// Sets a boolean that says the Datum is managed externally.
		/// </summary>
		/// <param name="array">The externally managed data array.</param>
		/// <param name="size">The size of the externally managed data array.</param>
		/// <exception cref="std::runtime_error">Thrown if the size passed in is 0.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not Unknown or an Integer.</exception>
		/// <exception cref="std::runtime_error">Thrown if internal storage is already set.</exception>
		void SetStorage(glm::mat4* array, size_t size);
		/// <summary>
		/// Sets the data array to an externally managed array of type pointer.
		/// Sets a boolean that says the Datum is managed externally.
		/// </summary>
		/// <param name="array">The externally managed data array.</param>
		/// <param name="size">The size of the externally managed data array.</param>
		/// <exception cref="std::runtime_error">Thrown if the size passed in is 0.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not Unknown or an Integer.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not the same as currently set type.</exception>
		/// <exception cref="std::runtime_error">Thrown if internal storage is already set.</exception>
		void SetStorage(RTTI** array, size_t size);
#pragma endregion
		/// <summary>
		/// Reserves memory for the Datum using the specified capacity.
		/// </summary>
		/// <param name="capacity">The number of elements that can be stored in the Vector.</param>
		/// <exception cref="std::runtime_error">Thrown if external storage is being used.</exception>
		/// <exception cref="std::runtime_error">Thrown when attempting to reserve memory with an unknown type. There is no way to know the size of an Unknown type.</exception>
		void Reserve(size_t capacity);

#pragma region Set
		/// <summary>
		/// Sets the data at the provided index to the passed in integer.
		/// </summary>
		/// <param name="value">The integer to set the data to.</param>
		/// <param name="index">The index in the data that will be set.</param>
		/// <exception cref="std::runtime_error">Thrown if this function is called with an Unknown type.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		void Set(const int& value, size_t index = 0_z);
		/// <summary>
		/// Sets the data at the provided index to the passed in float.
		/// </summary>
		/// <param name="value">The float to set the data to.</param>
		/// <param name="index">The index in the data that will be set.</param>
		/// <exception cref="std::runtime_error">Thrown if this function is called with an Unknown type.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		void Set(const float& value, size_t index = 0_z);
		/// <summary>
		/// Sets the data at the provided index to the passed in string.
		/// </summary>
		/// <param name="value">The string to set the data to.</param>
		/// <param name="index">The index in the data that will be set.</param>
		/// <exception cref="std::runtime_error">Thrown if this function is called with an Unknown type.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		void Set(const std::string& value, size_t index = 0_z);
		/// <summary>
		/// Sets the data at the provided index to the passed in Vector.
		/// </summary>
		/// <param name="value">The Vector to set the data to.</param>
		/// <param name="index">The index in the data that will be set.</param>
		/// <exception cref="std::runtime_error">Thrown if this function is called with an Unknown type.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		void Set(const glm::vec4& value, size_t index = 0_z);
		/// <summary>
		/// Sets the data at the provided index to the passed in Matrix.
		/// </summary>
		/// <param name="value">The Matrix to set the data to.</param>
		/// <param name="index">The index in the data that will be set.</param>
		/// <exception cref="std::runtime_error">Thrown if this function is called with an Unknown type.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		void Set(const glm::mat4& value, size_t index = 0_z);
		/// <summary>
		/// Sets the data at the provided index to the passed in Pointer.
		/// </summary>
		/// <param name="value">The Pointer to set the data to.</param>
		/// <param name="index">The index in the data that will be set.</param>
		/// <exception cref="std::runtime_error">Thrown if this function is called with an Unknown type.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		void Set(RTTI*& value, size_t index = 0_z);

#pragma endregion

#pragma region Get
		/// <summary>
		/// Get integer from data at the specified index.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to integer.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		int& GetInteger(size_t index = 0_z);
		/// <summary>
		/// Get float from data at the specified index.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to float.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		float& GetFloat(size_t index = 0_z);
		/// <summary>
		/// Get string from data at the specified index.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>		
		/// <exception cref="std::runtime_error">Thrown if the type is not set to string.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		std::string& GetString(size_t index = 0_z);
		/// <summary>
		/// Get vector from data at the specified index.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to Vector.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		glm::vec4& GetVector(size_t index = 0_z);
		/// <summary>
		/// Get matrix from data at the specified index.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to Matrix.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		glm::mat4& GetMatrix(size_t index = 0_z);
		/// <summary>
		/// Get Scope from data at the specified index.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to Scope.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		Scope& GetScope(size_t index = 0_z);
		/// <summary>
		/// Get pointer from data at the specified index.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to Pointer.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		RTTI*& GetPointer(size_t index = 0_z);


		/// <summary>
		/// Get integer from data at the specified index. For use on a const Datum.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to integer.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		int& GetInteger(size_t index = 0_z) const;
		/// <summary>
		/// Get float from data at the specified index. For use on a const Datum.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to float.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		float& GetFloat(size_t index = 0_z) const;
		/// <summary>
		/// Get string from data at the specified index. For use on a const Datum.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>		
		/// <exception cref="std::runtime_error">Thrown if the type is not set to string.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		std::string& GetString(size_t index = 0_z) const;
		/// <summary>
		/// Get vector from data at the specified index. For use on a const Datum.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to Vector.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		glm::vec4& GetVector(size_t index = 0_z) const;
		/// <summary>
		/// Get matrix from data at the specified index. For use on a const Datum.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to Matrix.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		glm::mat4& GetMatrix(size_t index = 0_z) const;
		/// <summary>
		/// Get Scope from data at the specified index. For use on a const Datum.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to Scope.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		Scope& GetScope(size_t index = 0_z) const;
		/// <summary>
		/// Get pointer from data at the specified index. For use on a const Datum.
		/// </summary>
		/// <param name="index">The index used to access an element of data.</param>
		/// <returns>The element located in data at the index provided.</returns>
		/// <exception cref="std::runtime_error">Thrown if the type is not set to Pointer.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		RTTI*& GetPointer(size_t index = 0_z) const;
#pragma endregion

		/// <summary>
		/// Sets an index in data from a given string of the current type of Datum.
		/// The Datum must have already been set to use this function.
		/// </summary>
		/// <param name="str">The string to convert to a value.</param>
		/// <param name="index">The index of where to set in the data.</param>
		/// <exception cref="std::runtime_error">Thrown if the type is set to Unknown. No concept of an Unknown string. </exception>
		/// <exception cref="std::runtime_error">Thrown if the type is set to Pointer. No concept of an Pointer string. </exception>
		void SetFromString(const std::string& str, size_t index = 0_z);
		/// <summary>
		/// Provides a string representation of the element at this index in the data array.
		/// </summary>
		/// <param name="index">The index of the element.</param>
		/// <returns>A string representation of the element.</returns>
		std::string ToString(size_t index = 0_z) const;
		//void SetFromString(const std::string& value, std::size_t index = 0);
		void PushBackFromString(const std::string& value);

#pragma region PushBack
		/// <summary>
		/// Adds a integer element onto the back of the Datum using an lvalue. Requires a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The integer element that is being added.</param>
		/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not an integer.</exception>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(const int& value);
		/// <summary>
		/// Adds a integer element onto the back of the Datum using an rvalue. Does not require a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The integer element that is being added.</param>
		/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not an integer.</exception>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(int&& value);
		/// <summary>
		/// Adds a float element onto the back of the Datum using an lvalue. Requires a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The T element that is being added.</param>
		/// <returns>An iterator pointing to the element that was just pushed onto the back of the Vector.</returns>
		/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not an float.</exception>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(const float& value);
		/// <summary>
		/// Adds a float element onto the back of the Datum using an rvalue. Does not require a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The float element that is being added.</param>
		/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not an float.</exception>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(float&& value);
		/// <summary>
		/// Adds a string element onto the back of the Datum using an lvalue. Requires a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The string element that is being added.</param>
		/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not an string.</exception>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(const std::string& value);
		/// <summary>
		/// Adds a string element onto the back of the Datum using an rvalue. Does not require a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The string element that is being added.</param>
		/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not an string.</exception>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(std::string&& value);
		/// <summary>
		/// Adds a vector element onto the back of the Datum using an lvalue. Requires a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The vector element that is being added.</param>
		/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not an Vector.</exception>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(const glm::vec4& value);
		/// <summary>
		/// Adds a matrix element onto the back of the Datum using an lvalue. Requires a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The matrix element that is being added.</param>
		/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not an matrix.</exception>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(const glm::mat4& value);
		/// <summary>
		/// Adds a pointer element onto the back of the Datum using an lvalue. Requires a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The pointer element that is being added.</param>
		/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not an pointer.</exception>
		template <typename IncrementFunctor = DefaultIncrement>
		void PushBack(RTTI*& value);
#pragma endregion

#pragma region Front
		/// <summary>
		/// Provides the front integer as a reference.
		/// </summary>
		/// <returns>A mutable reference to the integer at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an integer type.</exception>
		int& FrontInteger();
		/// <summary>
		/// Provides the front integer as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the integer at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an integer type.</exception>
		const int& FrontInteger() const;
		/// <summary>
		/// Provides the front float as a reference.
		/// </summary>
		/// <returns>A mutable reference to the float at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Float type.</exception>
		float& FrontFloat();
		/// <summary>
		/// Provides the front float as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the float at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Float type.</exception>
		const float& FrontFloat() const;
		/// <summary>
		/// Provides the front string as a reference.
		/// </summary>
		/// <returns>A mutable reference to the string at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an String type.</exception>
		std::string& FrontString();
		/// <summary>
		/// Provides the front string as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the string at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an String type.</exception>
		const std::string& FrontString() const;
		/// <summary>
		/// Provides the front vector as a reference.
		/// </summary>
		/// <returns>A mutable reference to the vector at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Vector type.</exception>
		glm::vec4& FrontVector();
		/// <summary>
		/// Provides the front vector as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the vector at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Vector type.</exception>
		const glm::vec4& FrontVector() const;
		/// <summary>
		/// Provides the front matrix as a reference.
		/// </summary>
		/// <returns>A mutable reference to the matrix at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Matrix type.</exception>
		glm::mat4& FrontMatrix();
		/// <summary>
		/// Provides the front matrix as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the matrix at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Matrix type.</exception>
		const glm::mat4& FrontMatrix() const;
		/// <summary>
		/// Provides the front pointer as a reference.
		/// </summary>
		/// <returns>A mutable reference to the pointer at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an pointer type.</exception>
		RTTI*& FrontPointer();
		/// <summary>
		/// Provides the front pointer as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the pointer at the front of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an pointer type.</exception>
		RTTI* const & FrontPointer() const;
#pragma endregion

#pragma region Back
		/// <summary>
		/// Provides the Back integer as a reference.
		/// </summary>
		/// <returns>A mutable reference to the integer at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an integer type.</exception>
		int& BackInteger();
		/// <summary>
		/// Provides the Back integer as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the integer at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an integer type.</exception>
		const int& BackInteger() const;
		/// <summary>
		/// Provides the Back float as a reference.
		/// </summary>
		/// <returns>A mutable reference to the float at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Float type.</exception>
		float& BackFloat();
		/// <summary>
		/// Provides the Back float as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the float at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Float type.</exception>
		const float& BackFloat() const;
		/// <summary>
		/// Provides the Back string as a reference.
		/// </summary>
		/// <returns>A mutable reference to the string at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an String type.</exception>
		std::string& BackString();
		/// <summary>
		/// Provides the Back string as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the string at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an String type.</exception>
		const std::string& BackString() const;
		/// <summary>
		/// Provides the Back vector as a reference.
		/// </summary>
		/// <returns>A mutable reference to the vector at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Vector type.</exception>
		glm::vec4& BackVector();
		/// <summary>
		/// Provides the Back vector as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the vector at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Vector type.</exception>
		const glm::vec4& BackVector() const;
		/// <summary>
		/// Provides the Back matrix as a reference.
		/// </summary>
		/// <returns>A mutable reference to the matrix at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Matrix type.</exception>
		glm::mat4& BackMatrix();
		/// <summary>
		/// Provides the Back matrix as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the matrix at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an Matrix type.</exception>
		const glm::mat4& BackMatrix() const;
		/// <summary>
		/// Provides the Back pointer as a reference.
		/// </summary>
		/// <returns>A mutable reference to the pointer at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an pointer type.</exception>
		RTTI*& BackPointer();
		/// <summary>
		/// Provides the Back pointer as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the pointer at the Back of the Datum.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is empty.</exception>
		/// <exception cref="std::runtime_error">Throws an runtime error if the Datum is not an pointer type.</exception>
		RTTI* const & BackPointer() const;
#pragma endregion 

	/// <summary>
	/// Removes an element on the back of the list. Updates the size.
	/// </summary>
	void PopBack();


#pragma region IndexOf
	/// <summary>
	/// Searches for the given integer in data array and returns the index of that found element.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const int& value);
	/// <summary>
	/// Searches for the given integer in data array and returns the index of that found element. For use on a const Datum.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const int& value) const;
	/// <summary>
	/// Searches for the given float in data array and returns the index of that found element.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const float& value);
	/// <summary>
	/// Searches for the given float in data array and returns the index of that found element. For use on a const Datum.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const float& value) const;
	/// <summary>
	/// Searches for the given string in data array and returns the index of that found element.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const std::string& value);
	/// <summary>
	/// Searches for the given string in data array and returns the index of that found element. For use on a const Datum.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const std::string& value) const;
	/// <summary>
	/// Searches for the given vector in data array and returns the index of that found element.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const glm::vec4& value);
	/// <summary>
	/// Searches for the given vector in data array and returns the index of that found element. For use on a const Datum.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const glm::vec4& value) const;
	/// <summary>
	/// Searches for the given matrix in data array and returns the index of that found element.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const glm::mat4& value);
	/// <summary>
	/// Searches for the given matrix in data array and returns the index of that found element. For use on a const Datum.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const glm::mat4& value) const;
	/// <summary>
	/// Searches for the given pointer in data array and returns the index of that found element.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const RTTI*& value);
	/// <summary>
	/// Searches for the given pointer in data array and returns the index of that found element. For use on a const Datum.
	/// </summary>
	/// <param name="value">The element we are searching for.</param>
	/// <returns>An index of to that found element.</returns>
	size_t IndexOf(const RTTI*& value) const;
#pragma endregion

#pragma region Remove
	/// <summary>
	/// Removes the integer in the Datum that has the given value. Helper function that finds the integer and provides an index pointing to the element that will be removed.
	/// This index is passed onto the other Remove function.
	/// </summary>
	/// <param name="value">The integer to remove from the list.</param>
	/// <returns>Boolean that represents whether the remove was successful or not.</returns>
	/// <exception cref="std::runtime_error">Thrown if the type is not an integer.</exception>
	bool Remove(const int& value);
	/// <summary>
	/// Removes the float in the Datum that has the given value. Helper function that finds the float and provides an index pointing to the element that will be removed.
	/// This index is passed onto the other Remove function.
	/// </summary>
	/// <param name="value">The float to remove from the list.</param>
	/// <returns>Boolean that represents whether the remove was successful or not.</returns>
	/// <exception cref="std::runtime_error">Thrown if the type is not an float.</exception>
	bool Remove(const float& value);
	/// <summary>
	/// Removes the string in the Datum that has the given value. Helper function that finds the string and provides an index pointing to the element that will be removed.
	/// This index is passed onto the other Remove function.
	/// </summary>
	/// <param name="value">The string to remove from the list.</param>
	/// <returns>Boolean that represents whether the remove was successful or not.</returns>
	/// <exception cref="std::runtime_error">Thrown if the type is not an string.</exception>
	bool Remove(const std::string& value);
	/// <summary>
	/// Removes the vector in the Datum that has the given value. Helper function that finds the vector and provides an index pointing to the element that will be removed.
	/// This index is passed onto the other Remove function.
	/// </summary>
	/// <param name="value">The vector to remove from the list.</param>
	/// <returns>Boolean that represents whether the remove was successful or not.</returns>
	/// <exception cref="std::runtime_error">Thrown if the type is not an vector.</exception>
	bool Remove(const glm::vec4& value);
	/// <summary>
	/// Removes the matrix in the Datum that has the given value. Helper function that finds the matrix and provides an index pointing to the element that will be removed.
	/// This index is passed onto the other Remove function.
	/// </summary>
	/// <param name="value">The matrix to remove from the list.</param>
	/// <returns>Boolean that represents whether the remove was successful or not.</returns>
	/// <exception cref="std::runtime_error">Thrown if the type is not an matrix.</exception>
	bool Remove(const glm::mat4& value);
	/// <summary>
	/// Removes the pointer in the Datum that has the given value. Helper function that finds the pointer and provides an index pointing to the element that will be removed.
	/// This index is passed onto the other Remove function.
	/// </summary>
	/// <param name="value">The pointer to remove from the list.</param>
	/// <returns>Boolean that represents whether the remove was successful or not.</returns>
	/// <exception cref="std::runtime_error">Thrown if the type is not an pointer.</exception>
	bool Remove(const RTTI*& value);
	/// <summary>
	/// Removes the element in the list that has the given value. Main function that removes the element that is pointed to by the index given.
	/// </summary>
	/// <param name="index">The index that points to the element that will be removed.</param>
	/// <returns>Boolean that represents whether the remove was successful or not.</returns>
	/// <exception cref="std::runtime_error">Thrown if external storage is being used.</exception>
	/// <exception cref="std::runtime_error">Thrown if the type is Unknown.</exception>
	bool RemoveAt(size_t index);

	using RemoveFunction = void(Datum::*)(size_t index);
	/// <summary>
	/// Array of functions used to remove an element from the data array for each type that Datum can be.
	/// </summary>
	static RemoveFunction _removeFunctions[static_cast<int>(DatumTypes::Unknown) + 1];
	/// <summary>
	/// Removes an integer at the index provided in the data array.
	/// </summary>
	/// <param name="index">The index of the integer remove.</param>
	void RemoveIntegers(size_t index = 0_z);
	/// <summary>
	/// Removes an integer at the index provided in the data array.
	/// </summary>
	/// <param name="index">The index of the integer remove.</param>
	void RemoveFloats(size_t index = 0_z);
	/// <summary>
	/// Removes an integer at the index provided in the data array.
	/// </summary>
	/// <param name="index">The index of the integer remove.</param>
	void RemoveStrings(size_t index = 0_z);
	/// <summary>
	/// Removes an integer at the index provided in the data array.
	/// </summary>
	/// <param name="index">The index of the integer remove.</param>
	void RemoveVectors(size_t index = 0_z);
	/// <summary>
	/// Removes an integer at the index provided in the data array.
	/// </summary>
	/// <param name="index">The index of the integer remove.</param>
	void RemoveMatrices(size_t index = 0_z);
	/// <summary>
	/// Removes an integer at the index provided in the data array.
	/// </summary>
	/// <param name="index">The index of the integer remove.</param>
	void RemovePointers(size_t index = 0_z);

	/// <summary>
	/// Adds a pointer element onto the back of the Datum using an lvalue. Requires a copy. Updates the capacity and size.
	/// </summary>
	/// <param name="value">The pointer element that is being added.</param>
	/// <exception cref="std::runtime_error">Thrown if the memory is set to external.</exception>
	/// <exception cref="std::runtime_error">Thrown if the type is not an pointer.</exception>
	template <typename IncrementFunctor = DefaultIncrement>
	void PushBack(Scope& value);

	/// <summary>
	/// HashMap that contains each a conversion from a string to a Datum type.
	/// </summary>
	static const HashMap<const std::string, Datum::DatumTypes> _typeMap;

#pragma endregion

	private:
		/// <summary>
		/// Union type for storing and accessing Datum's elements. 
		/// Each pointer can be thought of as a lens to store and access elements of that type.
		/// </summary>
		union DatumValues
		{
			int* i;
			float* f;
			std::string* s;
			glm::vec4* v;
			glm::mat4* m;
			Scope** t;
			RTTI** p;
			void* vp;
		};

		/// <summary>
		/// The number of elements currently in the Datum.
		/// </summary>
		size_t _size{ 0 };
		/// <summary>
		/// The number of elements that can be currently stored in the Datum. Can also be thought of as the amount of space allocated for the vector.
		/// </summary>
		size_t _capacity{ 0 };
		/// <summary>
		/// Determines if the Datum's memory is managed externally.
		/// </summary>
		bool _isExternal{ false };
		/// <summary>
		/// The type of data that the Datum holds.
		/// </summary>
		DatumTypes _type{ DatumTypes::Unknown };
		/// <summary>
		/// The union where elements will be stored in the Datum.
		/// </summary>
		DatumValues _data{ nullptr };

		/// <summary>
		/// Sets the data at the provided index to the passed in Scope.
		/// </summary>
		/// <param name="value">The Scope pointer to set the data to.</param>
		/// <param name="index">The index in the data that will be set.</param>
		/// <exception cref="std::runtime_error">Thrown if this function is called with an Unknown type.</exception>
		/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
		void Set(Scope& value, size_t index);

		/// <summary>
		/// Sets the data array to an externally managed array of the passed in type.
		/// Sets a boolean that says the Datum is managed externally.
		/// Used void* union lens to set the data array
		/// </summary>
		/// <param name="array">The externally managed data array.</param>
		/// <param name="size">The size of the externally managed data array.</param>
		/// <exception cref="std::runtime_error">Thrown if the size passed in is 0.</exception>
		/// <exception cref="std::runtime_error">Thrown if the type is not Unknown or an Integer.</exception>
		/// <exception cref="std::runtime_error">Thrown if internal storage is already set.</exception>
		friend Attributed;
		void SetStorage(DatumTypes type, void* data, size_t size);

		/// <summary>
		/// Array that contains each size of each type that the Datum can be.
		/// </summary>
		static const size_t _sizeMap[static_cast<int>(DatumTypes::Unknown)];

#pragma region CreateFunction
		using CreateFunction = void(Datum::*)(size_t startIndex, size_t size) const;
		/// <summary>
		/// Array of functions used to create each type of data that Datum can be (default uninitialized values).
		/// </summary>
		static const CreateFunction _createFunctions[static_cast<int>(DatumTypes::Unknown) + 1];
		/// <summary>
		/// Use a loop to create default value integers.
		/// </summary>
		/// <param name="startIndex">Starting index to create integers at.</param>
		/// <param name="size">Ending index to create integers, which will be the size at the end of the loop.</param>
		void CreateIntegers(size_t startIndex, size_t size) const;
		/// <summary>
		/// Use a loop to create default value floats.
		/// </summary>
		/// <param name="startIndex">Starting index to create floats at.</param>
		/// <param name="size">Ending index to create floats, which will be the size at the end of the loop.</param>
		void CreateFloats(size_t startIndex, size_t size) const;
		/// <summary>
		/// Use a loop to create default value strings.
		/// </summary>
		/// <param name="startIndex">Starting index to create strings at.</param>
		/// <param name="size">Ending index to create strings, which will be the size at the end of the loop.</param>
		void CreateStrings(size_t startIndex, size_t size) const;
		/// <summary>
		/// Use a loop to create default value Vectors.
		/// </summary>
		/// <param name="startIndex">Starting index to create Vectors at.</param>
		/// <param name="size">Ending index to create Vectors, which will be the size at the end of the loop.</param>
		void CreateVectors(size_t startIndex, size_t size) const;
		/// <summary>
		/// Use a loop to create default value Matrices.
		/// </summary>
		/// <param name="startIndex">Starting index to create Matrices at.</param>
		/// <param name="size">Ending index to create Matrices, which will be the size at the end of the loop.</param>
		void CreateMatrices(size_t startIndex, size_t size) const;
		/// <summary>
		/// Use a loop to create default value RTTI pointers.
		/// </summary>
		/// <param name="startIndex">Starting index to create RTTI pointers at.</param>
		/// <param name="size">Ending index to create RTTI pointers, which will be the size at the end of the loop.</param>
		void CreatePointers(size_t startIndex, size_t size) const;
#pragma endregion

#pragma region CopyFunction
		using CopyFunction = void(Datum::*)(const Datum& other) const;
		/// <summary>
		/// Array of functions used to copy data for each type that Datum can be.
		/// </summary>
		static const CopyFunction _copyFunctions[static_cast<int>(DatumTypes::Unknown) + 1];
		/// <summary>
		/// Use a loop to copy integers from an existing Datum's data.
		/// </summary>
		/// <param name="startIndex">Starting index to copy integers from and to.</param>
		/// <param name="size">Ending index to copy integers from and to, which will be the size at the end of the loop.</param>
		/// <param name="data">The data from the other Datum that is being copied from.</param>
		void CopyIntegers(const Datum& other) const;
		/// <summary>
		/// Use a loop to copy floats from an existing Datum's data.
		/// </summary>
		/// <param name="startIndex">Starting index to copy floats from and to.</param>
		/// <param name="size">Ending index to copy floats from and to, which will be the size at the end of the loop.</param>
		/// <param name="data">The data from the other Datum that is being copied from.</param>
		void CopyFloats(const Datum& other) const;
		/// <summary>
		/// Use a loop to copy strings from an existing Datum's data.
		/// </summary>
		/// <param name="startIndex">Starting index to copy strings from and to.</param>
		/// <param name="size">Ending index to copy strings from and to, which will be the size at the end of the loop.</param>
		/// <param name="data">The data from the other Datum that is being copied from.</param>
		void CopyStrings(const Datum& other) const;
		/// <summary>
		/// Use a loop to copy Vectors from an existing Datum's data.
		/// </summary>
		/// <param name="startIndex">Starting index to copy Vectors from and to.</param>
		/// <param name="size">Ending index to copy Vectors from and to, which will be the size at the end of the loop.</param>
		/// <param name="data">The data from the other Datum that is being copied from.</param>
		void CopyVectors(const Datum& other) const;
		/// <summary>
		/// Use a loop to copy Matrices from an existing Datum's data.
		/// </summary>
		/// <param name="startIndex">Starting index to copy Matrices from and to.</param>
		/// <param name="size">Ending index to copy Matrices from and to, which will be the size at the end of the loop.</param>
		/// <param name="data">The data from the other Datum that is being copied from.</param>
		void CopyMatrices(const Datum& other) const;
		/// <summary>
		/// Use a loop to copy RTTI pointers from an existing Datum's data.
		/// </summary>
		/// <param name="startIndex">Starting index to copy RTTI pointers from and to.</param>
		/// <param name="size">Ending index to copy RTTI pointers from and to, which will be the size at the end of the loop.</param>
		/// <param name="data">The data from the other Datum that is being copied from.</param>
		void CopyPointers(const Datum& other) const;
#pragma endregion

#pragma region CompareFunction
		using CompareFunction = bool(Datum::*)(const Datum& rhs) const;
		/// <summary>
		/// Array of functions used to compare data from a one datum to another.
		/// </summary>
		static const CompareFunction _compareFunctions[static_cast<int>(DatumTypes::Unknown) + 1];

		/// <summary>
		/// Compares two datums that have POD type.
		/// </summary>
		/// <param name="rhs">The right hand side datum on the comparison.</param>
		/// <returns>Whether the comparison was true.</returns>
		bool ComparePODs(const Datum& rhs) const;
		/// <summary>
		/// Compares two datums that have String type.
		/// </summary>
		/// <param name="rhs">The right hand side datum on the comparison.</param>
		/// <returns>Whether the comparison was true.</returns>
		bool CompareStrings(const Datum& rhs) const;
		/// <summary>
		/// Compares two datums that have RTTI pointer type.
		/// </summary>
		/// <param name="rhs">The right hand side datum on the comparison.</param>
		/// <returns>Whether the comparison was true.</returns>
		bool ComparePointers(const Datum& rhs) const;
#pragma endregion

#pragma region ToStringFunction
		using ToStringFunction = std::string(Datum::*)(size_t index) const;
		/// <summary>
		/// Array of functions used to create strings from each type that Datum can be.
		/// </summary>
		static ToStringFunction _toStringFunctions[static_cast<int>(DatumTypes::Unknown) + 1];
		/// <summary>
		/// Creates a string from a Datum of type integer at the index provided.
		/// </summary>
		/// <param name="index">The index of the element that we want to generate the string for.</param>
		std::string ToStringIntegers(size_t index = 0_z) const;
		/// <summary>
		/// Creates a string from a Datum of type float at the index provided.
		/// </summary>
		/// <param name="index">The index of the element that we want to generate the string for.</param>
		std::string ToStringFloats(size_t index = 0_z) const;
		/// <summary>
		/// Creates a string from a Datum of type string at the index provided.
		/// </summary>
		/// <param name="index">The index of the element that we want to generate the string for.</param>
		std::string ToStringStrings(size_t index = 0_z) const;
		/// <summary>
		/// Creates a string from a Datum of type vector at the index provided.
		/// </summary>
		/// <param name="index">The index of the element that we want to generate the string for.</param>
		std::string ToStringVectors(size_t index = 0_z) const;
		/// <summary>
		/// Creates a string from a Datum of type matrix at the index provided.
		/// </summary>
		/// <param name="index">The index of the element that we want to generate the string for.</param>
		std::string ToStringMatrices(size_t index = 0_z) const;
		/// <summary>
		/// Creates a string from a Datum of type pointer at the index provided.
		/// </summary>
		/// <param name="index">The index of the element that we want to generate the string for.</param>
		std::string ToStringPointers(size_t index = 0_z) const;
#pragma endregion

#pragma region FromStringFunctions
		int32_t FromStringInteger(const std::string& value) const;
		float FromStringFloat(const std::string& value) const;
		glm::vec4 FromStringVector(const std::string& value) const;
		glm::mat4 FromStringMatrix(const std::string& value) const;
#pragma endregion

#pragma region SetFromString
		using SetFromStringFunction = void(Datum::*)(const std::string& str, size_t index);
		/// <summary>
		/// Array of functions used to set data from a string for each type that Datum can be.
		/// </summary>
		static SetFromStringFunction _setFromStringFunctions[static_cast<int>(DatumTypes::Unknown) + 1];
		/// <summary>
		/// Set integers from a string.
		/// </summary>
		void SetFromStringIntegers(const std::string& str, size_t index);
		/// <summary>
		/// Set floats from a string.
		/// </summary>
		void SetFromStringFloats(const std::string& str, size_t index);
		/// <summary>
		/// Set strings from a string.
		/// </summary>
		void SetFromStringStrings(const std::string& str, size_t index);
		/// <summary>
		/// Set vectors from a string.
		/// </summary>
		void SetFromStringVectors(const std::string& str, size_t index);
		/// <summary>
		/// Set matrices from a string.
		/// </summary>
		void SetFromStringMatrices(const std::string& str, size_t index);
#pragma endregion


#pragma region PushBackFromString
		using PushBackFromStringFunction = void(Datum::*)(const std::string& str);
		/// <summary>
		/// Array of functions used to PushBack data from a string for each type that Datum can be.
		/// </summary>
		static PushBackFromStringFunction _pushBackFromStringFunctions[static_cast<int>(DatumTypes::Unknown) + 1];
		/// <summary>
		/// PushBack integers from a string.
		/// </summary>
		void PushBackFromStringIntegers(const std::string& str);
		/// <summary>
		/// PushBack floats from a string.
		/// </summary>
		void PushBackFromStringFloats(const std::string& str);
		/// <summary>
		/// PushBack strings from a string.
		/// </summary>
		void PushBackFromStringStrings(const std::string& str);
		/// <summary>
		/// PushBack vectors from a string.
		/// </summary>
		void PushBackFromStringVectors(const std::string& str);
		/// <summary>
		/// PushBack matrices from a string.
		/// </summary>
		void PushBackFromStringMatrices(const std::string& str);
#pragma endregion
	};
}

#include "Datum.inl"
