#pragma once

#include <initializer_list>
#include <stdexcept>
#include "DefaultEquality.h"
#include "SizeLiteral.h"


namespace FIEAGameEngine
{

	/// <summary>
	/// A templated class for a singly-linked list. Each element in the list is a Node structure which contains a T-type piece of data.
	/// </summary>
	template <typename T>
	class SList
	{
	private:
		/// <summary>
		/// A structure to hold all elements for a single node in the list. Each node contains a T-type piece of data and a pointer to the next node.
		/// </summary>
		struct Node final
		{
			/// <summary>
			/// Constuctor for a node using a const lvalue. Copies the data from the reference provided into the internal data member.
			/// </summary>
			/// <param name="data">Data to be copied into this node.</param>
			/// <param name="next">Pointer to the next node in the list.</param>
			Node(const T& data, Node* next = nullptr);

			/// <summary>
			/// Constuctor for a node using a rvalue. Takes the data from the reference provided into the internal data member. 
			/// </summary>
			/// <param name="data">Data to be copied into this node.</param>
			/// <param name="next">Pointer to the next node in the list.</param>
			Node(T&& data, Node* next = nullptr);

			/// <summary>
			/// Data that this node represents.
			/// </summary>
			T _data;
			/// <summary>
			/// Pointer to the next node in the list.
			/// </summary>
			Node* _next;
		};

	public:
		/// <summary>
		/// Iterator object for the SList container. Can access the private members of both SList and ConstIterator.
		/// </summary>
		class Iterator final
		{
			friend SList;
			friend class ConstIterator;

		public:
			/// <summary>
			/// Creates a new instance of a SList Iterator. Default constructor provided by the compiler.
			/// </summary>
			Iterator() = default;
			/// <summary>
			/// Invokes the copy constructor. 
			/// Creates a new SList Iterator and deep copies each member variable from other SList Iterator into this SList Iterator. 
			/// Default copy constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of SList Iterator that is being copied from.</param>
			Iterator(const Iterator& other) = default;
			/// <summary>
			/// Invokes the move constructor. Creates a new SList Iterator and "steals" the member variables from other SList Iterator into this SList. 
			/// Shallow copies the member variables and sets the other SList Iterator's member variables to nullptr. 
			/// Default move constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of SList Iterator that is being moved from.</param>
			Iterator(Iterator&& other) noexcept = default;
			/// <summary>
			/// Invokes the copy assignment. Deep copies each member variable from other SList Iterator into this SList Iterator.
			/// Clears the current member variables and deep copies the other SList Iterator member variables into this SList Iterator.
			/// Default copy assignment provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of SList Iterator that is being copied from.</param>
			/// <returns>A reference to this object.</returns>
			Iterator& operator=(const Iterator& other) = default;
			/// <summary>
			/// Invokes the move assignment. "Steals" the member variables from other SList Iterator into this SList Iterator. 
			/// Clears the current list. Then, shallow copies the member variables and sets the other SList Iterator's member variables to nullptr.			
			/// Default move assignment provided by the compiler.		
			/// </summary>
			/// <param name="other">The instance of SList Iterator that is being moved from.</param>
			/// <returns>A reference to this object.</returns>
			Iterator& operator=(Iterator&& other) noexcept = default;
			/// <summary>
			/// Default destructor provided by the compiler.
			/// </summary>
			~Iterator() = default;
			/// <summary>
			/// Equality operator. Tests whether a SList Iterator is equal to another.
			/// </summary>
			/// <param name="other">The instance of SList Iterator that we are comparing against.</param>
			/// <returns>A boolean representing whether equality test was true.</returns>
			bool operator==(const Iterator& other) const;
			/// <summary>
			/// Not Equality operator. Tests whether a SList Iterator is not equal to another.
			/// </summary>
			/// <param name="other">The instance of SList Iterator that we are comparing against.</param>
			/// <returns>A boolean representing whether equality test was true.</returns>
			bool operator!=(const Iterator& other) const;
			/// <summary>
			/// Prefix Increment operator. Changes the SList Iterator to point to the the next element in the SList.
			/// </summary>
			/// <returns>A reference to this object after the increment.</returns>
			/// <exception cref="std::runtime_error">Error is thrown when the Iterator is not associated with any list. No element to increment.</exception>
			Iterator& operator++();
			/// <summary>
			/// Postfix Increment operator. Changes the SList Iterator to point to the the next element in the SList.
			/// </summary>
			/// <param name="int">"Fake parameter name.</param>
			/// <returns>A reference to this object before the increment.</returns>
			Iterator operator++(int);
			/// <summary>
			/// Dereference operator. Exposes the element that the SList Iterator is pointing to.
			/// </summary>
			/// <exception cref="std::runtime_error">Guard against dereferencing a node that is set to nullptr.</exception>
			T& operator*() const;
			/// <summary>
			/// Arrow operator. Dereferences and exposes the members of the SList that the Iterator is pointing to.
			/// </summary>
			/// <exception cref="std::runtime_error">Guard against dereferencing a node that is set to nullptr.</exception>
			T* operator->() const;

		private:
			/// <summary>
			/// Private constructor to create an Iterator with a given Owner SList and Node element. 
			/// </summary>
			/// <param name="owner">The SList that "owns" this Iterator.</param>
			/// <param name="node">The element in the SList that the Iterator points to.</param>
			Iterator(const SList& owner, Node* node);
			/// <summary>
			/// The SList that "owns" this Iterator. Initialized to nullptr.
			/// </summary>
			const SList* _owner{ nullptr };
			/// <summary>
			/// The element in the SList that the Iterator points to. Initialized to nullptr.
			/// </summary>
			Node* _node{ nullptr };
		};

		/// <summary>
		/// ConstIterator object for the SList container. These methods will be invoked against a const SList only. Can access the private members of both SList.
		/// </summary>
		class ConstIterator final
		{
			friend SList;

		public:
			/// <summary>
			/// Creates a new instance of a SList ConstIterator. ConstIterator is for use on a const SList. Default constructor provided by the compiler.
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// Invokes the copy constructor against an ConstIterator provided a non-const SList Iterator. Essentially provides a copy of the Iterator that now has been promoted to have const-ness.
			/// Creates a new SList ConstIterator and deep copies each member variable from other SList ConstIterator into this SList ConstIterator. 
			/// Default copy constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of SList Iterator that is being copied from.</param>
			ConstIterator(const Iterator& other);
			/// <summary>
			/// Invokes the copy constructor against an ConstIterator.
			/// Creates a new SList ConstIterator and deep copies each member variable from other SList ConstIterator into this SList ConstIterator. 
			/// Default copy constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of SList ConstIterator that is being copied from.</param>
			ConstIterator(const ConstIterator& other) = default;
			/// <summary>
			/// Invokes the move constructor. Creates a new SList ConstIterator and "steals" the member variables from other SList ConstIterator into this SList. 
			/// Shallow copies the member variables and sets the other SList ConstIterator's member variables to nullptr. 
			/// Default move constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of SList ConstIterator that is being moved from.</param>
			ConstIterator(ConstIterator&& other) noexcept = default;
			/// <summary>
			/// Invokes the copy assignment. Deep copies each member variable from other SList ConstIterator into this SList ConstIterator.
			/// Clears the current member variables and deep copies the other SList Iterator member variables into this SList ConstIterator.
			/// Default copy assignment provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of SList ConstIterator that is being copied from.</param>
			/// <returns>A reference to this object.</returns>
			ConstIterator& operator=(const ConstIterator& other) = default;
			/// <summary>
			/// Invokes the move assignment. "Steals" the member variables from other SList ConstIterator into this SList ConstIterator. 
			/// Clears the current list. Then, shallow copies the member variables and sets the other SList ConstIterator's member variables to nullptr.			
			/// Default move assignment provided by the compiler.		
			/// </summary>
			/// <param name="other">The instance of SList ConstIterator that is being moved from.</param>
			/// <returns>A reference to this object.</returns>
			ConstIterator& operator=(ConstIterator&& other) noexcept = default;
			/// <summary>
			/// Default destructor provided by the compiler.
			/// </summary>
			~ConstIterator() = default;
			/// <summary>
			/// Equality operator. Tests whether a SList ConstIterator is equal to another.
			/// </summary>
			/// <param name="other">The instance of SList ConstIterator that we are comparing against.</param>
			/// <returns>A boolean representing whether equality test was true.</returns>
			bool operator==(const ConstIterator& other) const;
			/// <summary>
			/// Not Equality operator. Tests whether a SList ConstIterator is not equal to another.
			/// </summary>
			/// <param name="other">The instance of SList ConstIterator that we are comparing against.</param>
			/// <returns>A boolean representing whether equality test was true.</returns>
			bool operator!=(const ConstIterator& other) const;
			/// <summary>
			/// Prefix Increment operator. Changes the SList ConstIterator to point to the next element in the SList.
			/// </summary>
			/// <returns>A reference to this object after the increment.</returns>
			/// <exception cref="std::runtime_error">Error is thrown when the Iterator is not associated with any list. No element to increment.</exception>
			ConstIterator& operator++();
			/// <summary>
			/// Postfix Increment operator. Changes the SList ConstIterator to point to the next element in the SList.
			/// </summary>
			/// <param name="int">"Fake parameter name.</param>
			/// <returns>A reference to this object before the increment.</returns>
			ConstIterator operator++(int);
			/// <summary>
			/// Dereference operator. Exposes the element that the SList ConstIterator is pointing to.
			/// </summary>
			/// <returns>A reference to the element that the Iterator is pointing to.</returns>
			/// <exception cref="std::runtime_error">Guard against dereferencing a node that is set to nullptr.</exception>
			const T& operator*() const;
			/// <summary>
			/// Arrow operator. Dereferences and exposes the members of the SList that the Iterator is pointing to.
			/// </summary>
			/// <exception cref="std::runtime_error">Guard against dereferencing a node that is set to nullptr.</exception>
			const T* operator->() const;

			

		private:
			/// <summary>
			/// Private constructor to create an ConstIterator with a given Owner const SList and Node element. 
			/// </summary>
			/// <param name="owner">The const SList that "owns" this Iterator.</param>
			/// <param name="node">The element that the Iterator points to.</param>
			ConstIterator(const SList& owner, Node* node);
			/// <summary>
			/// The const SList that "owns" this Iterator. Initialized to nullptr.
			/// </summary>
			const SList* _owner{ nullptr };
			/// <summary>
			/// The element in the SList that the const Iterator points to. Initialized to nullptr.
			/// </summary>
			const Node* _node{ nullptr };
		};

		/// <summary>
		/// Creates a new instance of SList. Default constructor provided by the compiler.
		/// </summary>
		SList() = default;
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new SList and deep copies each member variable from other SList into this SList. 
		/// Deep copies each node using the PushBack() method.
		/// </summary>
		/// <param name="other">The instance of SList that is being copied from.</param>
		SList(const SList& other);
		/// <summary>
		/// Invokes the move constructor. Creates a new SList and "steals" the member variables from other SList into this SList. 
		/// Shallow copies the member variables and sets the other SList's member variables to nullptr. 
		/// Does not deep copy each node into this SList.
		/// </summary>
		/// <param name="other">The instance of SList that is being moved from.</param>
		SList(SList&& other) noexcept;
		/// <summary>
		/// Invokes the copy assignment. Deep copies each member variable from other SList into this SList.
		/// Clears the current list and deep copies each node using the PushBack() method.
		/// </summary>
		/// <param name="other">The instance of SList that is being copied from.</param>
		/// <returns>A SList reference to this object.</returns>
		SList& operator=(const SList& other);
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other SList into this SList. 
		/// Clears the current list. Then, shallow copies the member variables and sets the other SList's member variables to nullptr.
		/// Does not deep copy each node into this SList.
		/// </summary>
		/// <param name="other">The instance of SList that is being moved from.</param>
		/// <returns>A SList reference to the this object.</returns>
		SList& operator=(SList&& other) noexcept;
		/// <summary>
		/// Destructor that clears the list.
		/// </summary>
		~SList();

		/// <summary>
		/// Adds a T element onto the front of the list using an lvalue. Requires a copy. Updates the front pointer and size.
		/// </summary>
		/// <param name="value">The T element that is being added.</param>
		/// <returns>An iterator pointing to the element that was just pushed onto the front of the list.</returns>
		Iterator PushFront(const T& value);
		/// <summary>
		/// Adds a T element onto the front of the list using an rvalue. Does not require a copy. Updates the front pointer and size.
		/// </summary>
		/// <param name="value">The T element that is being added.</param>
		/// <returns>An iterator pointing to the element that was just pushed onto the front of the list.</returns>
		Iterator PushFront(T&& value);
		/// <summary>
		/// Adds a T element onto the back of the list using an lvalue. Requires a copy. Updates the back pointer and size.
		/// </summary>
		/// <param name="value">The T element that is being added.</param>
		/// <returns>An iterator pointing to the element that was just pushed onto the back of the list.</returns>
		Iterator PushBack(const T& value);
		/// <summary>
		/// Adds a T element onto the back of the list using an rvalue. Does not require a copy. Updates the back pointer and size.
		/// </summary>
		/// <param name="value">The T element that is being added.</param>
		/// <returns>An iterator pointing to the element that was just pushed onto the back of the list.</returns>
		Iterator PushBack(T&& value);
		/// <summary>
		/// Removes a T element onto the front of the list. Updates the back pointer and size.
		/// </summary>
		void PopFront();
		/// <summary>
		/// Removes a T element onto the back of the list. Updates the front pointer and size.
		/// </summary>
		void PopBack();
		/// <summary>
		/// Returns a boolean valule representing whether the list has no elements.
		/// </summary>
		/// <returns>A boolean value representing whether the size is 0.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Provides the front element as a reference.
		/// </summary>
		/// <returns>A mutable reference to the element at the front of the list.</returns>
		T& Front();
		/// <summary>
		/// Provides the front element as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the element at the front of the list.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the list is empty. Cannot provide the front element if there aren't any elements in the list.</exception>
		const T& Front() const;
		/// <summary>
		/// Provides the back element as a reference.
		/// </summary>
		/// <returns>A mutable reference to the element at the back of the list.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the list is empty. Cannot provide the front element if there aren't any elements in the list.</exception>
		T& Back();
		/// <summary>
		/// Provides the back element as a const reference.
		/// </summary>
		/// <returns>An immutable reference to the element at the back of the list.</returns>
		/// <exception cref="std::runtime_error">Throws an runtime error if the list is empty. Cannot provide the back element if there aren't any elements in the list.</exception>
		const T& Back() const;

		/// <summary>
		/// Provides the current size of the list.
		/// </summary>
		/// <returns>A size_t representing the number of elements in the list.</returns>
		size_t Size() const;

		/// <summary>
		/// Removes all elements from the list. Sets the size to zero and sets the front and back pointers to nullptr.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns an SList Iterator that points to the first element in the list.
		/// </summary>
		/// <returns>An  SList Iterator that points to the first element in the list.</returns>
		Iterator begin();
		/// <summary>
		/// Returns an SList ConstIterator that points to the first element in the list.
		/// </summary>
		/// <returns>An  SList ConstIterator that points to the first element in the list.</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Returns an SList ConstIterator that points to the first element in the list. This version is for use on a non-const SList.
		/// </summary>
		/// <returns>An  SList ConstIterator that points to the first element in the list.</returns>
		ConstIterator cbegin() const;
		/// <summary>
		/// Returns an SList Iterator that points to one past the last element in the list.
		/// </summary>
		/// <returns>An SList Iterator that points to one past the last element in the list.</returns>
		Iterator end();
		/// <summary>
		/// Returns an SList ConstIterator that points to one past the last element in the list.
		/// </summary>
		/// <returns>An SList ConstIterator that points to one past the last element in the list.</returns>
		ConstIterator end() const;
		/// <summary>
		/// Returns an SList ConstIterator that points to the last element in the list. This version is for use on a non-const SList.
		/// </summary>
		/// <returns>An SList ConstIterator that points to the last element in the list.</returns>
		ConstIterator cend() const;
		/// <summary>
		/// Inserts a new element with the given value after the given iterator.
		/// </summary>
		/// <param name="value">The value to set the new element to.</param>
		/// <param name="it">The SList Iterator that points to the element that will be inserted after.</param>
		/// <returns>An SList Iterator pointing to the newly inserted element.</returns>
		Iterator InsertAfter(const T& value, const Iterator& it);
		/// <summary>
		/// Searches for the given element and returns a SList Iterator pointing to that found element.
		/// </summary>
		/// <param name="value">The element we are searching for.</param>
		/// <returns>A SList Iterator pointing to that found element.</returns>
		/// <exception cref="std::runtime_error">Thrown if this function is called using an Iterator that is not associated with this list.</exception>
		template <typename EqualityFunctor = DefaultEquality<T>>
		Iterator Find(const T& value);
		/// <summary>
		/// Searches for the given element and returns a SList ConstIterator pointing to that found element.
		/// </summary>
		/// <param name="value">The element we are searching for.</param>
		/// <returns>A SList Iterator pointing to that found element.</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		ConstIterator Find(const T& value) const;
		/// <summary>
		/// Removes the element in the list that has the given element. Helper function that finds the element and provides a SList Iterator pointing to the element that will be removed.
		/// This SList Iterator is passed onto the other Remove function.
		/// </summary>
		/// <param name="value">The element to remove from the list.</param>
		/// <returns>Boolean that represents whether the remove was successful or not.</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		bool Remove(const T& value);
		/// <summary>
		/// Removes the element in the list that has the given element. Main function that removes the element that is pointed to by the SList Iterator given.
		/// </summary>
		/// <param name="it">The SList Iterator that points to the element that will be removed.</param>
		/// <returns>Boolean that represents whether the remove was successful or not.</returns>
		/// <exception cref="std::runtime_error">Thrown if this function is called using an Iterator that is not associated with this list.</exception>
		bool Remove(const Iterator& it);

	private:
		/// <summary>
		/// Node pointer to the T element at the front of the list.
		/// </summary>
		Node* _front{ nullptr };
		/// <summary>
		/// Node pointer to the T element at the back of the list.
		/// </summary>
		Node* _back{ nullptr };
		/// <summary>
		/// Internal size_t member representing the number of T elements in the list.
		/// </summary>
		size_t _size{ 0 };
	};

}

#include "SList.inl"


