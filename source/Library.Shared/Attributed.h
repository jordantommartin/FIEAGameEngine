#pragma once
#include "gsl/gsl"
#include "Scope.h"
#include "Signature.h"
#include "TypeManager.h"


namespace FIEAGameEngine
{
	/// <summary>
	/// A pure virtual class that mirrors native C++ classes for outside use with JSON configuration files. 
	/// Derives from Scope Class and intended to sit on top of game object classes.
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, FIEAGameEngine::Scope)

	public:
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new Attributed and deep copies each member variable from the other Attributed into this Attributed. 
		/// Uses the copy constructor from scope and adds an entry for this "this" object. 
		/// Updates external storage pointers to point to this object.
		/// </summary>
		/// <param name="other">The instance of Attributed that is being copied from.</param>
		Attributed(const Attributed& other);
		/// <summary>
		/// Invokes the move constructor. Creates a new Attributed and "steals" the member variables from other Attributed into this Attributed. 
		/// Shallow copies the member variables and sets the other Attributed's member variables to their uninitialized defaults.
		/// Uses the move constructor from scope and adds an entry for this "this" object. 
		/// Updates external storage pointers to point to this object. 
		/// </summary>
		/// <param name="other">The instance of Attributed that is being moved from.</param>
		Attributed(Attributed&& other) noexcept;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this Scope and deep copies each element from other Attributed into this Attributed.
		/// Uses the copy assignment from scope and adds an entry for this "this" object. 
		/// Updates external storage pointers to point to this object. 
		/// </summary>
		/// <param name="other">The instance of Attributed that is being copied from.</param>
		/// <returns>A Attributed reference to this object.</returns>
		Attributed& operator=(const Attributed& other);
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Attributed into this Attributed. 
		/// Clears the current Attributed and frees the memory associated with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other Attributed's member variables to their uninitialized defaults.
		/// Uses the move assignment from scope and adds an entry for this "this" object. 
		/// Updates external storage pointers to point to this object. 
		/// </summary>
		/// <param name="other">The instance of Attributed that is being moved from.</param>
		/// <returns>A Attributed reference to the this object.</returns>
		Attributed& operator=(Attributed&& other) noexcept;

		/// <summary>
		/// Checks whether the name provided is an Attribute for this object
		/// </summary>
		/// <param name="name">The name that would be associated with the Attribute.</param>
		/// <returns>Whether the name is an Attribute for this object.</returns>
		bool IsAttribute(const std::string& name) const;
		/// <summary>
		/// Checks whether the name provided is a Prescribed Attribute for this object.
		/// </summary>
		/// <param name="name">The name that would be associated with the Prescribed Attribute.</param>
		/// <returns>Whether the name is an Prescribed Attribute for this object.</returns>
		bool IsPrescribedAttribute(const std::string& name) const;
		/// <summary>
		/// Checks whether the name provided is an Auxiliary Attribute for this object.
		/// </summary>
		/// <param name="name">The name that would be associated with the Auxiliary Attribute.</param>
		/// <returns>Whether the name is an Auxiliary Attribute for this object.</returns>
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/// <summary>
		/// Appends an Auxiliary Attribute to this object. These would be any attributes added after construction.
		/// This is a wrapper for the Scope's append function with a check to make sure the attribute is not already prescribed.
		/// </summary>
		/// <param name="name">The name of the Auxiliary Attribute to append.</param>
		/// <returns>A reference to that newly added attribute.</returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// Accessor method which provides all attributes in the object.
		/// </summary>
		/// <returns>A const reference to the Vector of pointers to the string, datum pairs within this object.</returns>
		const Vector<Scope::PairType*>& Attributes() const;
		/// <summary>
		/// Accessor method which provides all prescribed attributes in the object. These are all attributes that existed at construction.
		/// </summary>
		/// <returns>A copy of the Vector of pointers to the string, datum pairs within this object that only contains Prescribed attributes.</returns>
		Vector<Scope::PairType*> PrescribedAttributes() const;
		/// <summary>
		/// Accessor method which provides all prescribed attributes in the object. These are all attributes that were appended after construction.
		/// </summary>
		/// <returns>A copy of the Vector of pointers to the string, datum pairs within this object that only contains Auxiliary attributes.</returns>
		Vector<Scope::PairType*> AuxiliaryAttributes() const;

	protected:
		/// <summary>
		/// Default Attributed Constructor is deleted and protected because the class is pure virtual.
		/// </summary>
		Attributed() = delete;
		/// <summary>
		/// Contructor from a given type that is used to prep the derived class.
		/// </summary>
		/// <param name="typeId">The RTTI type of the derived class.</param>
		explicit Attributed(RTTI::IdType typeId);
		/// <summary>
		/// Default destructor that calls the inherited destructor from the Scope class.
		/// </summary>
		~Attributed() = default;

	private:
		/// <summary>
		/// Populates the object with all prescribed attributes from the Signature Vector for this type.
		/// </summary>
		/// <param name="typeId">The RTTI type of the object that is being populated.</param>
		void Populate(RTTI::IdType typeId);
		/// <summary>
		/// Modified version of Populate that only updates the External Storage pointers to point to this object.
		/// </summary>
		/// <param name="typeId">The RTTI type of the object that is having its storage pointers updated.</param>
		void UpdateExternalStorage(RTTI::IdType typeId);
	};
}