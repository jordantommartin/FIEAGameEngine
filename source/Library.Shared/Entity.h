#pragma once
#include "GameTime.h"
#include "Factory.h"
#include "Attributed.h"
#include "Action.h"
#include "WorldState.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Class that represents what is typically known as a Game Object. 
	/// An attributed scope that can be derived from and parsed from Json.
	/// </summary>
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed);
	public:

		/// <summary>
		/// Creates a new instance of Entity. 
		/// </summary>
		Entity();

		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new Entity and deep copies each member variable from the other Entity into this Entity. 
		/// </summary>
		/// <param name="other">The instance of Entity that is being copied from.</param>
		Entity(const Entity&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new Entity and "steals" the member variables from other Entity into this Entity. 
		/// Shallow copies the member variables and sets the other Entity's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of Entity that is being moved from.</param>
		Entity(Entity&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this Entity and deep copies each element from other Entity into this Entity.
		/// </summary>
		/// <param name="other">The instance of Entity that is being copied from.</param>
		/// <returns>A Entity reference to this object.</returns>
		Entity& operator=(const Entity&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other Entity into this Entity. 
		/// Clears the current Entity and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other Entity's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of Entity that is being moved from.</param>
		/// <returns>A Entity reference to the this object.</returns>
		Entity& operator=(Entity&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this Entity.
		/// </summary>
		virtual ~Entity() = default;

		/// <summary>
		/// Returns a string with the name of the entity.
		/// </summary>
		/// <returns>The name of this entity as a string.</returns>
		const std::string& Name() const;
		/// <summary>
		/// Sets the name of the entity.
		/// </summary>
		/// <param name="name">The passed in string that the name will be set to.</param>
		void SetName(const std::string& name);
		/// <summary>
		/// Updates this entity as well as all of the children entities/actions.
		/// </summary>
		/// <param name="worldState">The WorldState that is managed during each update call.</param>
		virtual void Update(WorldState& worldState);

		/// <summary>
		/// Uses the entity factory to make a new object of the given type, adopts the entity into the children datum, and returns the address of the new entity.
		/// </summary>
		/// <param name="className">The name of the derived class to instantiate. (Entity if not derived) </param>
		/// <param name="instanceName">The name to give this instantiation of the object.</param>
		/// <returns>A reference to the child that was created.</returns>
		Entity& CreateChild(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Returns the the contained "children" entities.
		/// </summary>
		/// <returns>The Datum reference that stores the children entities.</returns>
		Datum& GetChildren();

		/// <summary>
		/// Returns the the contained "children" entities.
		/// </summary>
		/// <returns>The Datum reference that stores the children entities.</returns>
		const Datum& GetChildren() const;

		/// <summary>
		/// Uses the action factory to make a new object of the given type, adopts the action into the actions datum, and returns the address of the new action.
		/// </summary>
		/// <param name="className">The name of the derived class to instantiate. </param>
		/// <param name="instanceName">The name to give this instantiation of the object.</param>
		/// <returns>A reference to the action that was created.</returns>
		Action& CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Returns the the contained "actions" datum.
		/// </summary>
		/// <returns>The Datum reference that stores the actions.</returns>
		Datum& Actions();

		///// <summary>
		///// Returns the the contained "actions" datum.
		///// </summary>
		///// <returns>The Datum reference that stores the actions.</returns>
		const Datum& Actions() const;

		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an Entity.
		/// </summary>
		/// <returns>An Entity pointer to the heap allocated memory.</returns>
		gsl::owner<Entity*> Clone() const override;
		/// <summary>
		/// Determines if two Entitys are equal to one another.
		/// </summary>
		/// <param name="rhs">The other Entity which is being compared to this Entity.</param>
		/// <returns>Whether the two Entitys are equal to each other.</returns>
		bool Equals(const RTTI* rhs) const override;
		/// <summary>
		/// Provides a string representaion of this Entity.
		/// </summary>
		/// <returns>The string representation of this Entity.</returns>
		std::string ToString() const override;

		/// <summary>
		/// Provides the Signatures Vector for an Entity with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an Entity.</returns>
		static const FIEAGameEngine::Vector<FIEAGameEngine::Signature> Signatures();

	protected:
		/// <summary>
		/// Pass through contructor so that we can a call the Attributed constructor from a derived class of entity.
		/// </summary>
		/// <param name="type">The type of the derived class.</param>
		explicit Entity(RTTI::IdType type);
		/// <summary>
		/// The name of this entity.
		/// </summary>
		std::string _name;
		/// <summary>
		/// Index that will be used to get the children of this entity.
		/// </summary>
		inline static const size_t childrenIndex = 2;
		/// <summary>
		/// Index that will be used to get the actions of this entity.
		/// </summary>
		inline static const size_t actionsIndex = 3;
		/// <summary>
		/// String that will be used to get an entity.
		/// </summary>
		inline static std::string EntityStringLiteral = "Entity";
		/// <summary>
		/// String that will be used to get the name of this entity.
		/// </summary>
		inline static std::string NameStringLiteral = "Name";
		/// <summary>
		/// String that will be used to get the children of this entity.
		/// </summary>
		inline static std::string ChildrenStringLiteral = "Children";
		/// <summary>
		/// String that will be used to get the actions of this entity.
		/// </summary>
		inline static std::string ActionStringLiteral = "Actions";
	};

	ConcreteFactory(Entity, Scope)
}