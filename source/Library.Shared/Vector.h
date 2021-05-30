#pragma once

#include <cassert>
#include "SizeLiteral.h"
#include "DefaultIncrement.h"
#include "DefaultEquality.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// A templated class for a vector. Each element in the vector is stored in an array which contains T-type pieces of data.
	/// </summary>
	/// <typeparam name="T">The type of element stored in the container.</typeparam>
	template <typename T>
	class Vector
	{
	public:
		/// <summary>
		/// Iterator object for the Vector container. Can access the private members of both Vector and ConstIterator.
		/// </summary>
		class Iterator final
		{
			friend Vector;
			friend class ConstIterator;

		public:
			/// <summary>
			/// Creates a new instance of a Vector Iterator. Default constructor provided by the compiler.
			/// </summary>
			Iterator() = default;
			/// <summary>
			/// Invokes the copy constructor. 
			/// Creates a new Vector Iterator and deep copies each member variable from other Vector Iterator into this Vector Iterator. 
			/// Default copy constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of Vector Iterator that is being copied from.</param>
			Iterator(const Iterator& other) = default;
			/// <summary>
			/// Invokes the move constructor. Creates a new Vector Iterator and "steals" the member variables from other Vector Iterator into this Vector. 
			/// Shallow copies the member variables and sets the other Vector Iterator's member variables to nullptr. 
			/// Default move constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of Vector Iterator that is being moved from.</param>
			Iterator(Iterator&& other) noexcept = default;
			/// <summary>
			/// Invokes the copy assignment. Deep copies each member variable from other Vector Iterator into this Vector Iterator.
			/// Clears the current member variables and deep copies the other Vector Iterator member variables into this Vector Iterator.
			/// Default copy assignment provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of Vector Iterator that is being copied from.</param>
			/// <returns>A Vector Iterator reference to this object.</returns>
			Iterator& operator=(const Iterator& other) = default;
			/// <summary>
			/// Invokes the move assignment. "Steals" the member variables from other Vector Iterator into this Vector Iterator. 
			/// Clears the current vector. Then, shallow copies the member variables and sets the other Vector Iterator's member variables to nullptr.			
			/// Default move assignment provided by the compiler.		
			/// </summary>
			/// <param name="other">The instance of Vector Iterator that is being moved from.</param>
			/// <returns>A Vector Iterator reference to this object.</returns>
			Iterator& operator=(Iterator&& other) noexcept = default;
			/// <summary>
			/// Default destructor provided by the compiler.
			/// </summary>
			~Iterator() = default;
			/// <summary>
			/// Equality operator. Tests whether a Vector Iterator is equal to another.
			/// </summary>
			/// <param name="other">The instance of Vector Iterator that we are comparing against.</param>
			/// <returns>A boolean representing whether equality test was true.</returns>
			bool operator==(const Iterator& other) const;
			/// <summary>
			/// Not Equality operator. Tests whether a Vector Iterator is not equal to another.
			/// </summary>
			/// <param name="other">The instance of Vector Iterator that we are comparing against.</param>
			/// <returns>A boolean representing whether equality test was true.</returns>
			bool operator!=(const Iterator& other) const;
			/// <summary>
			/// Prefix Increment operator. Changes the Vector Iterator to point to the next element in the Vector.
			/// </summary>
			/// <returns>A reference to this Iterator.</returns>
			/// <exception cref="std::runtime_error">Thrown if the Iterator is not associated with any vector.</exception>
			Iterator& operator++();
			/// <summary>
			/// Postfix Increment operator. Changes the Vector Iterator to point to the next element in the Vector.
			/// </summary>
			/// <param name="int">Fake parameter name.</param>
			/// <returns>A reference to this Iterator.</returns>
			Iterator operator++(int);
			/// <summary>
			/// Prefix Decrement operator. Changes the Vector Iterator to point to the previous element in the Vector.
			/// </summary>
			/// <returns>A reference to this Iterator.</returns>
			/// <exception cref="std::runtime_error">Thrown if the Iterator is not associated with any vector.</exception>
			Iterator& operator--();
			/// <summary>
			/// Postfix Decrement operator. Changes the Vector Iterator to point to the previous element in the Vector.
			/// </summary>
			/// <param name="int">Fake parameter name.</param>
			/// <returns>A reference to this Iterator.</returns>
			Iterator operator--(int);
			/// <summary>
			/// Addition operator. Gives an iterator that has been moved forward a certain number of elements.
			/// </summary>
			/// <param name="increment">The amount of elements that the iterator will move forward.</param>
			/// <returns>The new iterator that has been moved forward.</returns>
			/// <exception cref="std::runtime_error">Thrown if the Iterator is not associated with any vector.</exception>
			Iterator operator+(size_t increment) const;
			/// <summary>
			/// Subtraction operator. Gives an iterator that has been moved backward a certain number of elements.
			/// </summary>
			/// <param name="decrement">The amount of elements that the iterator will move backward.</param>
			/// <returns>The new iterator that has been moved backward.</returns>
			/// <exception cref="std::runtime_error">Thrown if the Iterator is not associated with any vector.</exception>
			Iterator operator-(size_t decrement) const;
			/// <summary>
			/// Dereference operator. Exposes the element that the Vector Iterator is pointing to.
			/// </summary>
			/// <exception cref="std::runtime_error">Thrown if the Iterator is not associated with any vector.</exception>
			T& operator*() const;

		private:
			/// <summary>
			/// Private constructor to create an Iterator with a given Owner Vector and size_t index.
			/// </summary>
			/// <param name="owner">The Vector that "owns" this Iterator.</param>
			/// <param name="index">The index of the element in the Vector that the Iterator points to.</param>
			Iterator(Vector& owner, size_t index);
			/// <summary>
			/// The Vector that "owns" this Iterator. Initialized to nullptr.
			/// </summary>
			Vector* _owner{ nullptr };
			/// <summary>
			/// The index that refers that the place of the element in the vector.
			/// </summary>
			size_t _index{ 0_z };
		};

		/// <summary>
		/// ConstIterator object for the Vector container. These methods will be invoked against a const Vector only. Can access the private members of both Vector.
		/// </summary>
		class ConstIterator final
		{
			friend Vector;

		public:
			/// <summary>
			/// Creates a new instance of a Vector ConstIterator. ConstIterator is for use on a const Vector. Default constructor provided by the compiler.
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// Invokes the copy constructor against an ConstIterator provided a non-const Vector Iterator. Provides a copy of the Iterator that now has been promoted to have const-ness.
			/// Creates a new Vector ConstIterator and deep copies each member variable from other Vector ConstIterator into this Vector ConstIterator. 
			/// Default copy constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of Vector Iterator that is being copied from.</param>
			ConstIterator(const Iterator& other);
			/// <summary>
			/// Invokes the copy constructor against an ConstIterator.
			/// Creates a new Vector ConstIterator and deep copies each member variable from other Vector ConstIterator into this Vector ConstIterator. 
			/// Default copy constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of Vector ConstIterator that is being copied from.</param>
			ConstIterator(const ConstIterator& other) = default;
			/// <summary>
			/// Invokes the move constructor. Creates a new Vector ConstIterator and "steals" the member variables from other Vector ConstIterator into this Vector. 
			/// Shallow copies the member variables and sets the other Vector ConstIterator's member variables to nullptr. 
			/// Default move constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of Vector ConstIterator that is being moved from.</param>
			ConstIterator(ConstIterator&& other) noexcept = default;
			/// <summary>
			/// Invokes the copy assignment. Deep copies each member variable from other Vector ConstIterator into this Vector ConstIterator.
			/// Clears the current member variables and deep copies the other Vector Iterator member variables into this Vector ConstIterator.
			/// Default copy assignment provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of Vector ConstIterator that is being copied from.</param>
			/// <returns>A Vector ConstIterator reference to this object.</returns>
			ConstIterator& operator=(const ConstIterator& other) = default;
			/// <summary>
			/// Invokes the move assignment. "Steals" the member variables from other Vector ConstIterator into this Vector ConstIterator. 
			/// Clears the current vector. Then, shallow copies the member variables and sets the other Vector ConstIterator's member variables to nullptr.			
			/// Default move assignment provided by the compiler.		
			/// </summary>
			/// <param name="other">The instance of Vector ConstIterator that is being moved from.</param>
			/// <returns>A Vector ConstIterator reference to this object.</returns>
			ConstIterator& operator=(ConstIterator&& other) noexcept = default;
			/// <summary>
			/// Default destructor provided by the compiler.
			/// </summary>
			~ConstIterator() = default;
			/// <summary>
			/// Equality operator. Tests whether a Vector ConstIterator is equal to another.
			/// </summary>
			/// <param name="other">The instance of Vector ConstIterator that we are comparing against.</param>
			/// <returns>A boolean representing whether equality test was true.</returns>
			bool operator==(const ConstIterator& other) const;
			/// <summary>
			/// Not Equality operator. Tests whether a Vector ConstIterator is not equal to another.
			/// </summary>
			/// <param name="other">The instance of Vector ConstIterator that we are comparing against.</param>
			/// <returns>A boolean representing whether equality test was true.</returns>
			bool operator!=(const ConstIterator& other) const;
			/// <summary>
			/// Addition operator. Gives an ConstIterator that has been moved forward a certain number of elements.
			/// </summary>
			/// <param name="increment">The amount of elements that the ConstIterator will move forward.</param>
			/// <returns>The new ConstIterator that has been moved forward.</returns>
			/// <exception cref="std::runtime_error">Thrown if the ConstIterator is not associated with any vector.</exception>
			ConstIterator operator+(size_t increment) const;
			/// <summary>
			/// Subtraction operator. Gives a ConstIterator that has been moved backward a certain number of elements.
			/// </summary>
			/// <param name="decrement">The amount of elements that the ConstIterator will move backward.</param>
			/// <returns>The new ConstIterator that has been moved backward.</returns>
			/// <exception cref="std::runtime_error">Thrown if the ConstIterator is not associated with any vector.</exception>
			ConstIterator operator-(size_t decrement) const;
			/// <summary>
			/// Prefix Increment operator. Changes the Vector ConstIterator to point to the next element in the Vector.
			/// </summary>
			/// <returns>A reference to this ConstIterator.</returns>
			/// <exception cref="std::runtime_error">Thrown if the ConstIterator is not associated with any vector.</exception>
			ConstIterator& operator++();
			/// <summary>
			/// Postfix Increment operator. Changes the Vector ConstIterator to point to the next element in the Vector.
			/// </summary>
			/// <param name="int">"Fake parameter name.</param>
			/// <returns>A reference to this ConstIterator.</returns>
			ConstIterator operator++(int);
			/// <summary>
			/// Prefix Decrement operator. Changes the Vector ConstIterator to point to the previous element in the Vector.
			/// </summary>
			/// <returns>A reference to this ConstIterator.</returns>
			/// <exception cref="std::runtime_error">Thrown if the ConstIterator is not associated with any vector.</exception>
			ConstIterator& operator--();
			/// <summary>
			/// Postfix Decrement operator. Changes the Vector ConstIterator to point to the previous element in the Vector.
			/// </summary>
			/// <param name="int">"Fake parameter name.</param>
			/// <returns>A reference to this ConstIterator.</returns>
			ConstIterator operator--(int);
			/// <summary>
			/// Dereference operator. Exposes the element that the Vector ConstIterator is pointing to.
			/// </summary>
			/// <returns>A reference to the underlying element that this Iterator points to.</returns>
			/// <exception cref="std::runtime_error">Thrown if the ConstIterator is not associated with any vector.</exception>
			const T& operator*() const;



		private:
			/// <summary>
			/// Private constructor to create an ConstIterator with a given Owner Vector and size_t index.
			/// </summary>
			/// <param name="owner">The Vector that "owns" this Iterator.</param>
			/// <param name="node">The index of the element in the Vector that the ConstIterator points to.</param>
			ConstIterator(const Vector& owner, size_t index);
			/// <summary>
			/// The Vector that "owns" this Iterator. Initialized to nullptr.
			/// </summary>
			const Vector* _owner{ nullptr };
			/// <summary>
			/// The index of the element that this Iterator points to in the Vector.
			/// </summary>
			size_t _index{ 0_z };

		};
		
		/// <summary>
		/// Creates a new instance of Vector. Sets the capacity to the optional parameters (0 if nothing is provided).
		/// </summary>
		/// <param name="capacity">Gives user the option to specify a capacity upfront. Default value is 0.</param>
		Vector(size_t capacity = 0_z);
		/// <summary>
		/// Creates a new instance of Vector from an initializer list.
		/// </summary>
		/// <param name="vector">Initializer list used to populate the vector.</param>
		Vector(std::initializer_list<T> list);
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new Vector and deep copies each member variable from other Vector into this Vector. 
		/// </summary>
		/// <param name="other">The instance of Vector that is being copied from.</param>
		Vector(const Vector& other);
		/// <summary>
		/// Invokes the move constructor. Creates a new Vector and "steals" the member variables from other Vector into this Vector. 
		/// Shallow copies the member variables and sets the other Vector's member variables to nullptr. 
		/// </summary>
		/// <param name="other">The instance of Vector that is being moved from.</param>
		Vector(Vector&& other) noexcept;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in the vector and deep copies each element from other Vector into this Vector.
		/// If the vector that is being copied from is bigger, that amount of memory will be reserved ahead of time.
		/// If the vector that is being copied from is smaller, the amount of memory reserved will stay the same. You will need to call ShrinkToFit if you want your new size to match your capacity.
		/// </summary>
		/// <param name="other">The instance of Vector that is being copied from.</param>
		/// <returns>A Vector reference to this object.</returns>
		Vector& operator=(const Vector& other);
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Vector into this Vector. 
		/// Clears the current vector. Then, shallow copies the member variables and sets the other Vector's member variables to nullptr.
		/// </summary>
		/// <param name="other">The instance of Vector that is being moved from.</param>
		/// <returns>A Vector reference to the this object.</returns>
		Vector& operator=(Vector&& other) noexcept;
		/// <summary>
		/// Returns a reference to the element at the provided index.
		/// </summary>
		/// <param name="index">The index of the element being referenced.</param>
		/// <returns>A reference to the element at this index in the vector.</returns>
		/// <exception cref="std::runtime_error">Thrown if the vector has no elements or the index is larger than size.</exception>
		T& operator[](size_t index);
		/// <summary>
		/// Returns a const reference to the element at the provided index. Invoked against a const vector.
		/// </summary>
		/// <param name="index">The index of the element being referenced.</param>
		/// <returns>A reference to the element at this index in the vector.</returns>
		/// <exception cref="std::runtime_error">Thrown if the vector has no elements or the index is larger than size.</exception>
		const T& operator[](size_t index) const;
		/// <summary>
		/// Destructor that clears the vector.
		/// </summary>
		~Vector();

		/// <summary>
		/// Returns a reference to the element at the provided index.
		/// </summary>
		/// <param name="index">The index of the element being referenced.</param>
		/// <returns>A reference to the element at this index in the vector.</returns>
		/// <exception cref="std::runtime_error">Thrown if the vector has no elements or the index is larger than size.</exception>
		T& At(size_t index);
		/// <summary>
		/// Returns a const reference to the element at the provided index. Invoked against a const vector.
		/// </summary>
		/// <param name="index">The index of the element being referenced.</param>
		/// <returns>A reference to the element at this index in the vector.</returns>
		/// <exception cref="std::runtime_error">Thrown if the vector has no elements or the index is larger than size.</exception>
		const T& At(size_t index) const;

		/// <summary>
		/// Returns a boolean valule representing whether the vector has no elements.
		/// </summary>
		/// <returns>A boolean value representing whether the size is 0.</returns>
		bool IsEmpty() const;
		/// <summary>
		/// Provides the current size of the vector.
		/// </summary>
		/// <returns>A size_t representing the number of elements in the vector.</returns>
		size_t Size() const;
		/// <summary>
		/// Provides the current size of the vector.
		/// </summary>
		/// <returns>The amount of elements that can fit in the allocated space for the vector.</returns>
		size_t Capacity() const;
		/// <summary>
		/// Provides the front element as a reference.
		/// </summary>
		/// <returns>A mutable reference to the element at the front of the vector.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the vector is empty. Cannot provide the front element if there aren't any elements in the vector.</exception>
		T& Front();
		/// <summary>
		/// Provides the front element as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the element at the front of the vector.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the vector is empty. Cannot provide the front element if there aren't any elements in the vector.</exception>
		const T& Front() const;
		/// <summary>
		/// Provides the back element as a reference.
		/// </summary>
		/// <returns>A mutable reference to the element at the back of the vector.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the vector is empty. Cannot provide the front element if there aren't any elements in the vector.</exception>
		T& Back();
		/// <summary>
		/// Provides the back element as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the element at the back of the vector.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the vector is empty. Cannot provide the back element if there aren't any elements in the vector.</exception>
		const T& Back() const;


		/// <summary>
		/// Adds a T element onto the back of the Vector using an lvalue. Requires a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The T element that is being added.</param>
		/// <returns>An iterator pointing to the element that was just pushed onto the back of the Vector.</returns>
		template <typename IncrementFunctor = DefaultIncrement>
		Iterator PushBack(const T& value);
		/// <summary>
		/// Adds a T element onto the back of the Vector using an rvalue. Does not require a copy. Updates the capacity and size.
		/// </summary>
		/// <param name="value">The T element that is being added.</param>
		/// <returns>An iterator pointing to the element that was just pushed onto the back of the Vector.</returns>
		template <typename IncrementFunctor = DefaultIncrement>
		Iterator PushBack(T&& value);
		/// <summary>
		/// Removes a T element onto the back of the list. Updates the size.
		/// </summary>
		void PopBack();
		/// <summary>
		/// Reserves memory for the Vector using the specified capacity. 
		/// </summary>
		/// <param name="capacity">The number of elements that can be stored in the Vector.</param>
		void Reserve(size_t capacity);
		/// <summary>
		/// Instantiates elements for the vector up to the specified size. Default constructed.
		/// </summary>
		/// <param name="size">The number of elements that will be instantiated in the Vector.</param>
		void Resize(size_t size);
		/// <summary>
		/// Removes all elements from the Vector. Sets the size and capacity to zero and sets the _data pointer to nullptr.
		/// </summary>
		void Clear();

		/// <summary>
		/// Reduces the capacity of the Vector to the current size. Reallocates memory to the new capacity.
		/// </summary>
		void ShrinkToFit();

		Iterator begin();
		/// <summary>
		/// Returns an Vector ConstIterator that points to the first element in the vector.
		/// </summary>
		/// <returns>An  Vector ConstIterator that points to the first element in the vector.</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Returns an Vector ConstIterator that points to the first element in the vector. This version is for use on a non-const Vector.
		/// </summary>
		/// <returns>An  Vector ConstIterator that points to the first element in the vector.</returns>
		ConstIterator cbegin() const;
		/// <summary>
		/// Returns an Vector Iterator that points to one past the last element in the vector.
		/// </summary>
		/// <returns>An Vector Iterator that points to one past the last element in the vector.</returns>
		Iterator end();
		/// <summary>
		/// Returns an Vector ConstIterator that points to one past the last element in the vector.
		/// </summary>
		/// <returns>An Vector ConstIterator that points to one past the last element in the vector.</returns>
		ConstIterator end() const;
		/// <summary>
		/// Returns an Vector ConstIterator that points to the last element in the vector. This version is for use on a non-const Vector.
		/// </summary>
		/// <returns>An Vector ConstIterator that points to the last element in the vector.</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Searches for the given element and returns a Vector Iterator pointing to that found element.
		/// </summary>
		/// <param name="value">The element we are searching for.</param>
		/// <returns>A Vector Iterator pointing to that found element.</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		Iterator Find(const T& value);
		/// <summary>
		/// Searches for the given element and returns a Vector ConstIterator pointing to that found element.
		/// </summary>
		/// <param name="value">The element we are searching for.</param>
		/// <returns>A Vector Iterator pointing to that found element.</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		ConstIterator Find(const T& value) const;
		/// <summary>
		/// Removes the element in the list that has the given element. Helper function that finds the element and provides a Vector Iterator pointing to the element that will be removed.
		/// This Vector Iterator is passed onto the other Remove function.
		/// </summary>
		/// <param name="value">The element to remove from the list.</param>
		/// <returns>Boolean that represents whether the remove was successful or not.</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		bool Remove(const T& value);
		/// <summary>
		/// Removes the element in the list that has the given element. Main function that removes the element that is pointed to by the Vector Iterator given.
		/// </summary>
		/// <param name="it">The Vector Iterator that points to the element that will be removed.</param>
		/// <returns>Boolean that represents whether the remove was successful or not.</returns>
		/// <exception cref="std::runtime_error">Thrown if the iterator passed in is not associated with this vector.</exception>
		bool Remove(const Iterator& it);
		/// <summary>
		/// Removes a range of contiguous elements in the list. Main function that removes the range of contiguous elements that is pointed to by the start and last Vector Iterator given.
		/// </summary>
		/// <param name="start">The Vector Iterator that points to the first element in the continuous range.</param>
		/// <param name="end">The Vector Iterator that points to the last element in the continuous range.</param>
		/// <returns>Boolean that represents whether the remove was successful or not.</returns>
		/// <exception cref="std::runtime_error">Thrown if either of the iterator passed in is not associated with this vector.</exception>
		void Remove(const Iterator& start, const Iterator& last);

	private:
		/// <summary>
		/// The array where elements will be stored in the vector.
		/// </summary>
		T* _data{ nullptr };
		/// <summary>
		/// The number of elements currently in the vector.
		/// </summary>
		size_t _size{ 0 };
		/// <summary>
		/// The number of elements that can be currently stored in the vector. Can also be thought of as the amount of space allocated for the vector.
		/// </summary>
		size_t _capacity{ 0 };
	};
}

#include "Vector.inl"
