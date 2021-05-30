#pragma once

#include "DefaultHash.h"
#include "DefaultIncrement.h"
#include "DefaultEquality.h"
#include "SList.h"
#include "Vector.h"
#include <functional>

namespace FIEAGameEngine
{
	/// <summary>
	/// A templated class for a HashMap. HashMap contains a Vector filled with SLists that contains TKey, TValue pairs.
	/// Access to the HashMap is performed through hashing a key.
	/// </summary>
	/// <typeparam name="TKey">The templated type of the key which will be hashed to retrieve to location.</typeparam>
	/// <typeparam name="TValue">The templated type of the value stored in the container.</typeparam>

	template<typename TKey, typename TValue>
	class HashMap
	{
	public:
		using PairType = std::pair<const TKey, TValue>;
		using HashFunctor = std::function<std::size_t(const TKey& key)>;
		using EqualityFunctor = std::function<bool(const TKey& lhs, const TKey& rhs)>;
		using value_type = PairType;
	private:
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using ChainIteratorType = typename ChainType::Iterator;
		using ConstChainIteratorType = typename ChainType::ConstIterator;


	public:
		/// <summary>
		/// Iterator object for the HashMap container. Can access the private members of both HashMap and ConstIterator.
		/// </summary>
		class Iterator final
		{
			friend HashMap;
			friend class ConstIterator;

		public:
			/// <summary>
			/// Creates a new instance of a HashMap Iterator. Default constructor provided by the compiler.
			/// </summary>
			Iterator() = default;
			/// <summary>
			/// Invokes the copy constructor. 
			/// Creates a new HashMap Iterator and deep copies each member variable from other HashMap Iterator into this HashMap Iterator. 
			/// Default copy constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of HashMap Iterator that is being copied from.</param>
			Iterator(const Iterator& other) = default;
			/// <summary>
			/// Invokes the move constructor. Creates a new HashMap Iterator and "steals" the member variables from other HashMap Iterator into this HashMap. 
			/// Shallow copies the member variables and sets the other HashMap Iterator's member variables to nullptr. 
			/// Default move constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of HashMap Iterator that is being moved from.</param>
			Iterator(Iterator&& other) noexcept = default;
			/// <summary>
			/// Invokes the copy assignment. Deep copies each member variable from other HashMap Iterator into this HashMap Iterator.
			/// Clears the current member variables and deep copies the other HashMap Iterator member variables into this HashMap Iterator.
			/// Default copy assignment provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of HashMap Iterator that is being copied from.</param>
			/// <returns>Reference to the newly assigned iterator.</returns>
			Iterator& operator=(const Iterator& other) = default;
			/// <summary>
			/// Invokes the move assignment. "Steals" the member variables from other HashMap Iterator into this HashMap Iterator. 
			/// Clears the current HashMap. Then, shallow copies the member variables and sets the other HashMap Iterator's member variables to nullptr.			
			/// Default move assignment provided by the compiler.		
			/// </summary>
			/// <param name="other">The instance of HashMap Iterator that is being moved from.</param>
			/// <returns>Reference to the newly assigned iterator.</returns>
			Iterator& operator=(Iterator&& other) noexcept = default;
			/// <summary>
			/// Default destructor provided by the compiler.
			/// </summary>
			~Iterator() = default;
			/// <summary>
			/// Equality operator. Tests whether a HashMap Iterator is equal to another.
			/// </summary>
			/// <param name="other">The instance of HashMap Iterator that we are comparing against.</param>
			/// <returns>True if all the members of each iterator are equal.</returns>
			bool operator==(const Iterator& other) const;
			/// <summary>
			/// Not Equality operator. Tests whether a HashMap Iterator is not equal to another.
			/// </summary>
			/// <param name="other">The instance of HashMap Iterator that we are comparing against.</param>
			/// <returns>True if any of the members of each iterator are not equal.</returns>
			bool operator!=(const Iterator& other) const;
			/// <summary>
			/// Prefix Increment operator. Changes the HashMap Iterator to point to the next element in the HashMap.
			/// </summary>
			/// <returns>Iterator pointing to the next element.</returns>
			/// <exception cref="std::runtime_error">Thrown if the Iterator is not associated with any HashMap.</exception>
			Iterator& operator++();
			/// <summary>
			/// Postfix Increment operator. Changes the HashMap Iterator to point to the next element in the HashMap.
			/// </summary>
			/// <param name="int">Fake parameter name.</param>
			/// <returns>Iterator pointing to the next element.</returns>
			Iterator operator++(int);
			/// <summary>
			/// Dereference operator. Exposes the element that the HashMap Iterator is pointing to.
			/// </summary>
			/// <exception cref="std::runtime_error">Thrown if the Iterator is not associated with any HashMap.</exception>
			/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
			PairType& operator*() const;
			/// <summary>
			/// Arrow operator. Dereferences the element that the HashMap Iterator is pointing to and allows you to access any of the public members of that element.
			/// </summary>
			/// <exception cref="std::runtime_error">Thrown if the Iterator is not associated with any HashMap.</exception>
			/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
			PairType* operator->() const;

		private:
			/// <summary>
			/// Private constructor to create an Iterator with a given Owner HashMap, an index of the Vector, and a chain iterator of the SList.
			/// </summary>
			/// <param name="owner">The HashMap that "owns" this Iterator.</param>
			/// <param name="index">The index of the Vector in the HashMap that the Iterator points to.</param>
			/// <param name="chainIt">The Chain Iterator that points to the current position in the current SList.</param>
			Iterator(HashMap& owner, size_t index, const ChainIteratorType& chainIt);
			/// <summary>
			/// The HashMap that "owns" this Iterator. Initialized to nullptr.
			/// </summary>
			HashMap* _owner{ nullptr };
			/// <summary>
			/// The index of the Vector in the HashMap that the Iterator points to.
			/// </summary>
			size_t _index{ 0 };
			/// <summary>
			/// The Chain Iterator that points to the current position in the current SList.
			/// </summary>
			ChainIteratorType _chainIt;
		};

		/// <summary>
		/// ConstIterator object for the HashMap container. These methods will be invoked against a const HashMap only. Can access the private members of both HashMap.
		/// </summary>
		class ConstIterator final
		{
			friend HashMap;

		public:
			/// <summary>
			/// Creates a new instance of a HashMap ConstIterator. ConstIterator is for use on a const HashMap. Default constructor provided by the compiler.
			/// </summary>
			ConstIterator() = default;
			/// <summary>
			/// Invokes the copy constructor against an ConstIterator provided a non-const HashMap Iterator. Provides a copy of the Iterator that now has been promoted to have const-ness.
			/// Creates a new HashMap ConstIterator and deep copies each member variable from other HashMap Iterator into this HashMap ConstIterator. 
			/// Default copy constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of HashMap Iterator that is being copied from.</param>
			ConstIterator(const Iterator& other);
			/// <summary>
			/// Invokes the copy constructor against an ConstIterator.
			/// Creates a new HashMap ConstIterator and deep copies each member variable from other HashMap ConstIterator into this HashMap ConstIterator. 
			/// Default copy constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of HashMap ConstIterator that is being copied from.</param>
			ConstIterator(const ConstIterator& other) = default;
			/// <summary>
			/// Invokes the move constructor. Creates a new HashMap ConstIterator and "steals" the member variables from other HashMap ConstIterator into this HashMap. 
			/// Shallow copies the member variables and sets the other HashMap ConstIterator's member variables to nullptr. 
			/// Default move constructor provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of HashMap ConstIterator that is being moved from.</param>
			ConstIterator(ConstIterator&& other) noexcept = default;
			/// <summary>
			/// Invokes the copy assignment. Deep copies each member variable from other HashMap ConstIterator into this HashMap ConstIterator.
			/// Clears the current member variables and deep copies the other HashMap Iterator member variables into this HashMap ConstIterator.
			/// Default copy assignment provided by the compiler.
			/// </summary>
			/// <param name="other">The instance of HashMap ConstIterator that is being copied from.</param>
			/// <returns>Reference to the newly assigned ConstIterator.</returns>
			ConstIterator& operator=(const ConstIterator& other) = default;
			/// <summary>
			/// Invokes the move assignment. "Steals" the member variables from other HashMap ConstIterator into this HashMap ConstIterator. 
			/// Clears the current HashMap. Then, shallow copies the member variables and sets the other HashMap ConstIterator's member variables to nullptr.			
			/// Default move assignment provided by the compiler.		
			/// </summary>
			/// <param name="other">The instance of HashMap ConstIterator that is being moved from.</param>
			/// <returns>Reference to the newly assigned ConstIterator.</returns>
			ConstIterator& operator=(ConstIterator&& other) noexcept = default;
			/// <summary>
			/// Default destructor provided by the compiler.
			/// </summary>
			~ConstIterator() = default;
			/// <summary>
			/// Equality operator. Tests whether a HashMap ConstIterator is equal to another.
			/// </summary>
			/// <param name="other">The instance of HashMap ConstIterator that we are comparing against.</param>
			/// <returns>True if all the members of each iterator are equal.</returns>
			bool operator==(const ConstIterator& other) const;
			/// <summary>
			/// Not Equality operator. Tests whether a HashMap ConstIterator is not equal to another.
			/// </summary>
			/// <param name="other">The instance of HashMap ConstIterator that we are comparing against.</param>
			/// <returns>True if any of the members of each iterator are not equal.</returns>
			bool operator!=(const ConstIterator& other) const;
			/// <summary>
			/// Prefix Increment operator. Changes the HashMap ConstIterator to point to the next element in the HashMap.
			/// </summary>
			/// <returns>Iterator pointing to the next element.</returns>
			/// <exception cref="std::runtime_error">Thrown if the ConstIterator is not associated with any HashMap.</exception>
			ConstIterator& operator++();
			/// <summary>
			/// Postfix Increment operator. Changes the HashMap ConstIterator to point to the next element in the HashMap.
			/// </summary>
			/// <param name="int">"Fake parameter name.</param>
			/// <returns>Iterator pointing to the next element.</returns>
			ConstIterator operator++(int);
			/// <summary>
			/// Dereference operator. Exposes the element that the HashMap ConstIterator is pointing to.
			/// </summary>
			/// <returns>A reference to the underlying element that this Iterator points to.</returns>
			/// <exception cref="std::runtime_error">Thrown if the ConstIterator is not associated with any HashMap.</exception>
			/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>
			const PairType& operator*() const;
			/// <summary>
			/// Arrow operator. Dereferences the element that the HashMap ConstIterator is pointing to and allows you to access any of the public members of that element.
			/// </summary>
			/// <exception cref="std::runtime_error">Thrown if the ConstIterator is not associated with any HashMap.</exception>
			/// <exception cref="std::runtime_error">Thrown if the index is out of bounds.</exception>			
			const PairType* operator->() const;

		private:
			/// <summary>
			/// Private constructor to create a ConstIterator with a given Owner HashMap, an index of the Vector, and a chain iterator of the SList.
			/// </summary>
			/// <param name="owner">The HashMap that "owns" this Iterator.</param>
			/// <param name="index">The index of the Vector in the HashMap that the Iterator points to.</param>
			/// <param name="chainIt">The Chain Iterator that points to the current position in the current SList.</param>
			ConstIterator(const HashMap& owner, size_t index, const ConstChainIteratorType& constChainIt);
			/// <summary>
			/// The HashMap that "owns" this Iterator. Initialized to nullptr.
			/// </summary>
			const HashMap* _owner{ nullptr };
			size_t _index{ 0 };
			ConstChainIteratorType _constChainIt;
		};

		/// <summary>
		/// Creates a new instance of HashMap. Allows user to provide custom size, hash functor, and equality functor.
		/// </summary>
		/// <param name="size">The amount of buckets in the vector.</param>
		/// <param name="hFunctor">The function object that provides a function for hashing keys.</param>
		/// <param name="eqFunctor">The function object that provides a function for determining equality between two items.</param>
		/// <exception cref="std::runtime_error">Thrown when the HashMap size is 0.</exception>
		explicit HashMap(size_t size = 11_z, HashFunctor hFunctor = DefaultHash<TKey>{}, EqualityFunctor eqFunctor = DefaultEquality<TKey>{});

		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new HashMap and deep copies each member variable from other HashMap into this HashMap. 
		/// </summary>
		/// <param name="other">The instance of HashMap that is being copied from.</param>
		HashMap(const HashMap& other) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new HashMap and "steals" the member variables from other HashMap into this HashMap. 
		/// Shallow copies the member variables and sets the other HashMap's member variables to nullptr. 
		/// </summary>
		/// <param name="other">The instance of HashMap that is being moved from.</param>
		HashMap(HashMap&& other) noexcept = default;
		/// <summary>
		/// Creates a HashMap from an initializer list, similar to Copy Constructor.
		/// </summary>
		/// <param name="list">The initializer list you are copying from.</param>
		HashMap(std::initializer_list<PairType> list);
		/// <summary>
		/// Creates a HashMap from an initializer list, similar to Copy Constructor.
		/// </summary>
		/// <param name="list">The initializer list you are copying from.</param>
		HashMap(std::initializer_list<PairType> list, HashFunctor hashFunctor, EqualityFunctor equalityFunctor);
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in the HashMap and deep copies each element from other HashMap into this HashMap.
		/// If the HashMap that is being copied from is bigger, that amount of memory will be reserved ahead of time.
		/// If the HashMap that is being copied from is smaller, the amount of memory reserved will stay the same. You will need to call ShrinkToFit if you want your new size to match your capacity.
		/// </summary>
		/// <param name="other">The instance of HashMap that is being copied from.</param>
		/// <returns>Reference to the newly created HashMap.</returns>
		HashMap& operator=(const HashMap& other) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other HashMap into this HashMap. 
		/// Clears the current HashMap. Then, shallow copies the member variables and sets the other HashMap's member variables to nullptr.
		/// </summary>
		/// <param name="other">The instance of HashMap that is being moved from.</param>
		/// <returns>Reference to the newly created HashMap.</returns>
		HashMap& operator=(HashMap&& other) noexcept = default;
		/// <summary>
		/// Destructor that clears the HashMap.
		/// </summary>
		~HashMap() = default;
		/// <summary>
		/// Using a key, searches for a TValue and returns a reference to that TValue. 
		/// If the key cannot be found, a <TKey, TValue> pair is inserted where the TKey is the key provided and the TValue is default constructed. 
		/// </summary>
		/// <param name="key">The key that you want to search on.</param>
		/// <returns>Reference to the TValue at this spot in the HashMap.</returns>
		TValue& operator[](const TKey& key);

		/// <summary>
		/// Searches for the given element and returns a HashMap Iterator pointing to that found element.
		/// </summary>
		/// <param name="key">The key we are searching for.</param>
		/// <returns>A HashMap Iterator pointing to that found element.</returns>
		Iterator Find(const TKey& key);
		/// <summary>
		/// Searches for the given element and returns a HashMap ConstIterator pointing to that found element.
		/// </summary>
		/// <param name="key">The key we are searching for.</param>
		/// <returns>A HashMap Iterator pointing to that found element.</returns>
		ConstIterator Find(const TKey& key) const;

		/// <summary>
		/// Inserts an entry into the HashMap and returns a newly created HashMap Iterator pointing to the location where this entry was stored.
		/// If the HashMap already contains this entry, does not insert the entry again or modify. Just returns an Iterator pointing
		/// </summary>
		/// <param name="entry">The pair of <TKey, TValue> that is being inserted into the list.</param>
		/// <returns>A pair with 1) the Iterator pointing to the inserted element and 2) a bool representing whether the insert was successful (also whether the Iterator provided is valid).</returns>
		std::pair<Iterator, bool> Insert(const PairType& entry);


		/// <summary>
		/// Resizes the HashMap to have a new number of buckets.
		/// </summary>
		/// <param name="bucketSize">The amount of buckets that will be in the new HashMap.</param>
		void Resize(size_t bucketSize);


		/// <summary>
		/// Removes an entry in the HashMap that matches with the key provided.
		/// If the key does not exist in the HashMap, the function does nothing.
		/// </summary>
		/// <param name="key">The key that matches the entry to remove from the list.</param>
		void Remove(const TKey& key);

		/// <summary>
		/// Removes all elements from the HashMap and deletes any memory allocated for elements in the chains.
		/// </summary>
		void Clear();

		/// <summary>
		/// Provides the number of elements currently in the HashMap.
		/// </summary>
		/// <returns>The number of elements in the HashMap.</returns>
		size_t Size() const;

		/// <summary>
		/// Determines if the HashMap has no elements.
		/// </summary>
		/// <returns>True if the HashMap has no elements.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Provides the number of buckets currently in the HashMap.
		/// </summary>
		/// <returns>The size of the vector in the HashMap.</returns>
		size_t BucketSize() const;

		/// <summary>
		/// Determines if the key is in the HashMap.
		/// </summary>
		/// <returns>True if the key is in the HashMap.</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Provides the TValue that matches with the TKey passed in. Requires that the <TKey, TValue> pair has already been inserted.
		/// </summary>
		/// <param name="key">The key that matches the value you are searching for.</param>
		/// <returns>That value that matches the key passed in.</returns>
		/// <exception cref="std::runtime_error">Thrown if the key is not in the HashMap.</exception>
		TValue& At(TKey key);
		/// <summary>
		/// Provides the TValue that matches with the TKey passed in. Requires that the <TKey, TValue> pair has already been inserted.
		/// Invoked against a const HashMap.
		/// </summary>
		/// <param name="key">The key that matches the value you are searching for.</param>
		/// <returns>That value that matches the key passed in.</returns>
		/// <exception cref="std::runtime_error">Thrown if the key is not in the HashMap.</exception>
		/// <exception cref="std::runtime_error">Thrown if the key is not in the HashMap.</exception>
		const TValue& At(TKey key) const;

		/// <summary>
		/// Returns a HashMap Iterator that points to one past the first element in the HashMap.
		/// </summary>
		/// <returns>Iterator pointing to the first element in the HashMap.</returns>
		Iterator begin();
		/// <summary>
		/// Returns an HashMap ConstIterator that points to the first element in the HashMap.
		/// </summary>
		/// <returns>Iterator pointing to the first element in the HashMap.</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Returns an HashMap ConstIterator that points to the first element in the HashMap. This version is for use on a non-const HashMap.
		/// </summary>
		/// <returns>ConstIterator pointing to the first element in the HashMap</returns>
		ConstIterator cbegin() const;
		/// <summary>
		/// Returns an HashMap Iterator that points to one past the last element in the HashMap.
		/// </summary>
		/// <returns>Iterator that points to one past the last element in the HashMap.</returns>
		Iterator end();
		/// <summary>
		/// Returns an HashMap ConstIterator that points to one past the last element in the HashMap.
		/// </summary>
		/// <returns>ConstIterator that points to one past the last element in the HashMap.</returns>
		ConstIterator end() const;
		/// <summary>
		/// Returns an HashMap ConstIterator that points to the last element in the HashMap. This version is for use on a non-const HashMap.
		/// </summary>
		/// <returns>ConstIterator that points to the last element in the HashMap.</returns>
		ConstIterator cend() const;


	private:
		/// <summary>
		/// The vector that contains all the chains for the HashMap.
		/// </summary>
		BucketType _buckets;
		/// <summary>
		/// The number of elements currently inserted into the HashMap.
		/// </summary>
		size_t _size{ 0 };
		/// <summary>
		/// The function object that provides a function for hashing keys.
		/// </summary>
		HashFunctor _hashFunctor;
		/// <summary>
		/// The function object that provides a function for determining equality between two items.
		/// </summary>
		EqualityFunctor _equalityFunctor;
	};
}

#include "HashMap.inl"