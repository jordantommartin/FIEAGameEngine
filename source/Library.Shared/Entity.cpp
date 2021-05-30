#include "pch.h"
#include "Entity.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Entity)

	Entity::Entity() :
		Attributed(Entity::TypeIdClass())
	{

	}

	Entity::Entity(RTTI::IdType type) :
		Attributed(type)
	{

	}

	const FIEAGameEngine::Vector< FIEAGameEngine::Signature> Entity::Signatures()
	{
		return Vector< FIEAGameEngine::Signature>
		{
			{ NameStringLiteral, Datum::DatumTypes::String, 1, offsetof(Entity, _name) },
			{ ChildrenStringLiteral, Datum::DatumTypes::Table, 0, 0 },
			{ ActionStringLiteral, Datum::DatumTypes::Table, 0, 0 }
		};
	}

	const std::string& Entity::Name() const
	{
		return _name;
	}

	void Entity::SetName(const std::string& name)
	{
		_name = name;
	}

	void Entity::Update(WorldState& worldState)
	{
		worldState._currentEntity = this;

		if (worldState._eventQueue != nullptr)
		{
			worldState._eventQueue->Update(worldState.GetGameTime());
		}

		if (GetParent() == nullptr)
		{
			worldState.ProcessQueues();
		}

		for (size_t i = 0; i < GetChildren().Size(); ++i)
		{
			Scope& child = GetChildren().GetScope(i);
			assert(child.Is(Entity::TypeIdClass()));
			static_cast<Entity&>(child).Update(worldState);
		}

		for (size_t i = 0; i < Actions().Size(); ++i)
		{
			Scope& action = Actions().GetScope(i);
			assert(action.Is(Action::TypeIdClass()));
			static_cast<Action&>(action).Update(worldState);
		}
	}

	Entity& Entity::CreateChild(const std::string& className, const std::string& instanceName)
	{
		Scope* scope = Factory<Scope>::Create(className);
		assert(scope != nullptr);
		assert(scope->Is(Entity::TypeIdClass()));

		Entity& child = static_cast<Entity&>(*scope);
		child.SetName(instanceName);
		Adopt(child, ChildrenStringLiteral);
		return child;
	}

	Datum& Entity::GetChildren()
	{
		assert((_order.Size() - 1) >= childrenIndex && _order[childrenIndex]->second.Type() == Datum::DatumTypes::Table);
		return _order[childrenIndex]->second;
	}

	const Datum& Entity::GetChildren() const
	{
		assert((_order.Size() - 1) >= childrenIndex && _order[childrenIndex]->second.Type() == Datum::DatumTypes::Table);
		return _order[childrenIndex]->second;
	}

	Action& Entity::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* scope = Factory<Scope>::Create(className);
		assert(scope != nullptr);
		assert(scope->Is(Action::TypeIdClass()));

		Action& action = static_cast<Action&>(*scope);
		action.SetName(instanceName);
		Adopt(action, ActionStringLiteral);
		return action;
	}

	Datum& Entity::Actions()
	{
		assert((_order.Size() - 1) >= actionsIndex && _order[actionsIndex]->second.Type() == Datum::DatumTypes::Table);
		return _order[actionsIndex]->second;
	}

	const Datum& Entity::Actions() const
	{
		assert((_order.Size() - 1) >= actionsIndex && _order[actionsIndex]->second.Type() == Datum::DatumTypes::Table);
		return _order[actionsIndex]->second;
	}

	gsl::owner<Entity*> Entity::Clone() const
	{
		return new Entity(*this);
	}

	bool Entity::Equals(const RTTI* rhs) const
	{
		const Entity* other = rhs->As<Entity>();
		return other != nullptr ? *this == *other : false;
	}

	std::string Entity::ToString() const
	{
		return "Entity"s;
	}

}