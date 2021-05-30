#pragma once
#include "Datum.h"
#include "HashMap.h"
#include "RTTI.h"
#include "Factory.h"
#include "gsl/gsl"

namespace FIEAGameEngine
{
	/// <summary>
	/// Data structure that has a dictionary of name-value pairs where Datum objects are the values. 
	/// Each entry in a scope has a name and a Datum. The Datum is an array of values of one of the available types.
	/// Scopes can refer to another Scope table.
	/// </summary>
	class Scope : public FIEAGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

	public:
		using PairType = HashMap<const std::string, Datum>::PairType;
		/// <summary>
		/// Creates a new instance of Scope. Takes in a capacity if user knows how much memory to allocate.
		/// No memory is allocated by default.
		/// </summary>
		/// <param name="capacity">Gives user the option to specify a capacity upfront. Default value is 0.</param>
		explicit Scope(size_t capacity = 11_z);

		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new Scope and deep copies each member variable from the other Scope into this Scope. 
		/// Does not retain the parent of the original scope. Parent is now nullptr.
		/// </summary>
		/// <param name="other">The instance of Scope that is being copied from.</param>
		Scope(const Scope& other);

		/// <summary>
		/// Invokes the move constructor. Creates a new Scope and "steals" the member variables from other Scope into this Scope. 
		/// Shallow copies the member variables and sets the other Scope's member variables to their uninitialized defaults. 
		/// Keeps the original parent from the moved Scope. Resets pointers of parent to point to this scope. Also resets parent pointers of children to point to this scope.
		/// </summary>
		/// <param name="other">The instance of Scope that is being moved from.</param>
		Scope(Scope&& other) noexcept;

		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this Scope and deep copies each element from other Scope into this Scope.
		/// If the Scope that is being copied from is bigger, that amount of memory will be reserved ahead of time.
		/// If the Scope that is being copied from is smaller, the amount of memory reserved will stay the same.
		/// Does not retain the parent of the original scope. Parent is now nullptr.
		/// </summary>
		/// <param name="other">The instance of Scope that is being copied from.</param>
		/// <returns>A Scope reference to this object.</returns>
		Scope& operator=(const Scope& other);

		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Scope into this Scope. 
		/// Clears the current Scope and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other Scope's member variables to their uninitialized defaults.
		/// Keeps the original parent from the moved Scope. Resets pointers of parent to point to this scope. Also resets parent pointers of children to point to this scope.
		/// </summary>
		/// <param name="other">The instance of Scope that is being moved from.</param>
		/// <returns>A Scope reference to the this object.</returns>
		Scope& operator=(Scope&& other) noexcept;

		/// <summary>
		/// Destructor that clears the memory associated with this Scope.
		/// </summary>
		~Scope();

		/// <summary>
		/// Equality operator. Tests whether a Scope is equal to another.
		/// </summary>
		/// <param name="other">The instance of Scope that we are comparing against.</param>
		/// <returns>Whether the two scopes are equal.</returns>
		bool operator==(const Scope& other) const;
		/// <summary>
		/// Not Equality operator. Tests whether a Scope is not equal to another.
		/// </summary>
		/// <param name="other">The instance of Scope that we are comparing against.</param>
		/// <returns>Whether the two scopes are not equal.</returns>
		bool operator!=(const Scope& other) const;
		/// <summary>
		/// Bracket overload to return the Scope at that index in the Datum.
		/// The index represents the order which the Datum was inserted into the Scope.
		/// </summary>
		/// <param name="index">The index in the Datum where the Scope is at.</param>
		/// <returns>Reference to the Scope at that index in the Datum.</returns>
		Datum& operator[](std::uint32_t index);
		/// <summary>
		/// Bracket overload to return the Scope at that index in the Datum.
		/// The index represents the order which the Datum was inserted into the Scope.
		/// </summary>
		/// <param name="index">The index in the Datum where the Scope is at.</param>
		/// <returns>Reference to the Scope at that index in the Datum.</returns>
		Datum& operator[](std::uint32_t index) const;
		/// <summary>
		/// Bracket overload to return the Scope with the provided name in the Datum.
		/// </summary>
		/// <param name="index">The name of the Datum inside this Scope.</param>
		/// <returns>Reference to the Scope with the provided name in the Datum.</returns>
		Datum& operator[](const std::string name);

		/// <summary>
		/// Provides the number of elements currently in the dictionary.
		/// </summary>
		/// <returns>The number of elements in the dictionary.</returns>
		size_t Size() const;

		/// <summary>
		/// Determines if the dictionary has no elements.
		/// </summary>
		/// <returns>True if the dictionary has no elements.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Removes all elements from the dictionary and order vector. Must call cascading delete call on each nested scope.
		/// </summary>
		void Clear();

		/// <summary>
		/// Creates a new Scopee from the value of this Scope and returns a pointer to it.
		/// </summary>
		/// <returns>Pointer to the newly created Scope.</returns>
		virtual gsl::owner<Scope*> Clone() const;

		/// <summary>
		/// Finds a Datum in the dictionary of this scope based on its name.
		/// </summary>
		/// <param name="name">The name associated with the Datum.</param>
		/// <returns>The address of the Datum associated with the given name. </returns>
		Datum* Find(const std::string& name);

		/// <summary>
		/// Finds a Datum in the dictionary of this scope based on its name.
		/// For use with a const Scope.
		/// </summary>
		/// <param name="name">The name associated with the Datum.</param>
		/// <returns>The address of the Datum associated with the given name. </returns>
		const Datum* Find(const std::string& name) const;

		/// <summary>
		/// Searches through this scope and its ancestors to find the most-closely nested Datum associated with the given name.
		/// If it does not find the datum, returns nullptr.
		/// </summary>
		/// <param name="name">The name of the Datum to find.</param>
		/// <returns>Pointer to the Datum being searched for if found and nullptr otherwise.</returns>
		Datum* Search(const std::string& name);

		/// <summary>
		/// Searches through this scope and its ancestors to find the most-closely nested Datum associated with the given name.
		/// For use with a const Scope.
		/// If it does not find the datum, returns nullptr.
		/// </summary>
		/// <param name="name">The name of the Datum to find.</param>
		/// <returns>Pointer to the Datum being searched for if found and nullptr otherwise.</returns>
		const Datum* Search(const std::string& name) const;


		/// <summary>
		/// Searches through this scope and its ancestors to find the most-closely nested Datum associated with the given name.
		/// If it does not find the datum, returns nullptr. 
		/// This version takes in a scope pointer that will be set to the scope in which the datum was found.
		/// </summary>
		/// <param name="name">The name of the Datum to find.</param>
		/// <param name="scope">The scope reference that will have the address of the Scope object which contains the match.</param>
		/// <returns>Pointer to the Datum being searched for if found and nullptr otherwise.</returns>
		Datum* Search(const std::string& name, Scope*& scope);

		/// <summary>
		/// Searches through this scope and its ancestors to find the most-closely nested Datum associated with the given name.
		/// For use with a const Scope.
		/// If it does not find the datum, returns nullptr. 
		/// This version takes in a scope pointer that will be set to the scope in which the datum was found.
		/// </summary>
		/// <param name="name">The name of the Datum to find.</param>
		/// <param name="scope">The scope reference that will have the address of the Scope object which contains the match.</param>
		/// <returns>Pointer to the Datum being searched for if found and nullptr otherwise.</returns>
		const Datum* Search(const std::string& name, const Scope*& scope) const;

		/// <summary>
		/// Returns a Datum in the dictionary. 
		/// If there is not a Datum with this name in the Scope, a new default constructed Datum is created and returned.
		/// </summary>
		/// <param name="name">The name associated with the Datum.</param>
		/// <returns>Reference to the Datum associated with the name provided.</returns>
		/// <exception cref="std::invalid_argument">Thrown when a empty string is passed in for the name.</exception>
		Datum& Append(const std::string& name);

		/// <summary>
		/// Returns a Scope in the dictionary. (Special case of Append)
		/// If there is not a Datum with this name in the Scope, a new Datum of type Scope is created and returned.
		/// </summary>
		/// <param name="name">The name associated with the Datum.</param>
		/// <returns>Reference to the Scope associated with the name provided.</returns>
		/// <exception cref="std::runtime_error">Thrown when attempting to append a scope to a datum that is not of type Unknown or Table (int, float, etc.)</exception>
		Scope& AppendScope(const std::string& name);

		/// <summary>
		/// Adds an orphaned Scope to this Scope at the Datum associated with the provided name.
		/// </summary>
		/// <param name="child">The orphaned Scope that will be adopted into this Scope.</param>
		/// <param name="name">The name of key for the Datum to use for storing the child.</param>
		/// <exception cref="std::invalid_argument">Thrown when a empty string is passed in for the name.</exception>
		/// <exception cref="std::invalid_argument">Thrown when the child is self or an existing ancestor.</exception>
		/// <exception cref="std::invalid_argument">Thrown when the child is an existing ancestor.</exception>
		/// <exception cref="std::runtime_error">Thrown when attempting to append a scope to a datum that is not of type Unknown or Table (int, float, etc.)</exception>
		void Adopt(Scope& child, const std::string& name);

		/// <summary>
		/// Removes the parent of a scope. Parent is notified of this removal.
		/// </summary>
		/// <param name="orphan">The Scope that will be orphaned.</param>
		void Orphan();

		/// <summary>
		/// Determines if the passed in scope is an ancestor of this scope.
		/// </summary>
		/// <param name="scope">The scope that is to be determined if it is a descendant of this scope.</param>
		/// <returns>Whether the passed in scope is an anscestor of this scope.</returns>
		bool IsAncestorOf(const Scope& scope) const;

		/// <summary>
		/// Determines if the passed in scope is an descendant of this scope.
		/// </summary>
		/// <param name="scope">The scope that is to be determined if it is a ancestor of this scope.</param>
		/// <returns>Whether the passed in scope is an descendant of this scope.</returns>
		bool IsDescendantOf(const Scope& scope) const;

		/// <summary>
		/// Gets the value of the parent of the current Scope.
		/// </summary>
		/// <returns>The Scope which contains this one.</returns>
		template <typename T>
		T& GetParent() const;

		/// <summary>
		/// Gets the parent pointer of the current Scope.
		/// </summary>
		/// <returns>Pointer to the Scope which contains this one.</returns>
		Scope* GetParent() const;

		/// <summary>
		/// Finds the Datum that contains the provided Scope.
		/// </summary>
		/// <param name="scope">The scope that we are looking to be contained in a Datum.</param>
		/// <returns>A pointer to the Datum that contains the provided scope, as well as the index that represents the order which the Datum was appended.</returns>
		std::pair<Datum*, std::uint32_t> FindContainedScope(const Scope& scope);

		/// <summary>
		/// Overrides the RTTI Equals method.
		/// </summary>
		/// <param name="rhs">The other RTTI this is being compared to.</param>
		/// <returns>Whether the two are equal to each other.</returns>
		bool Equals(const RTTI* rhs) const override;
		/// <summary>
		/// Overrides the RTTI To String method
		/// </summary>
		/// <returns>The Scope as a string representation.</returns>
		std::string ToString() const override;

		/// <summary>
		/// Provides the Datum associated with the name inside this object.
		/// The datum must exist in this object, otherwise an assert is thrown.
		/// </summary>
		/// <param name="name">The name associated with the Datum to be searched for.</param>
		/// <returns>The datum associated with the name provided.</returns>
		FIEAGameEngine::Datum& At(const std::string& name);
		/// <summary>
		/// For use with a const Scope
		/// Provides the Datum associated with the name inside this object.
		/// The datum must exist in this object, otherwise an assert is thrown.
		/// </summary>
		/// <param name="name">The name associated with the Datum to be searched for.</param>
		/// <returns>The datum associated with the name provided.</returns>
		const FIEAGameEngine::Datum& At(const std::string& name) const;

	protected:
		/// <summary>
		/// Hashmap that maps names to datums of an available type.
		/// </summary>
		HashMap<const std::string, Datum> _dictionary;
		/// <summary>
		/// Vector that tracks the order of insertions into the dictionary by storing pointers to those pairs.
		/// </summary>
		Vector<HashMap<const std::string, Datum>::PairType*> _order;
		/// <summary>
		/// Pointer to scope for its parent
		/// </summary>
		Scope* _parent{ nullptr };
	};
	ConcreteFactory(Scope, Scope)
}