#include "pch.h"
#include "Scope.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(size_t capacity) :
		_order(capacity)
	{

	}

	Scope::Scope(const Scope& other) : 
		_dictionary(other._dictionary.BucketSize()), _order(other._order.Size()) 
	{ 
		for (auto& item : other._order) 
		{ 
			auto& existingDatum = item->second; 
			Datum& newDatum = Append(item->first); 
			if (existingDatum.Type() == Datum::DatumTypes::Table) 
			{ 
				newDatum.SetType(Datum::DatumTypes::Table); 
				newDatum.Reserve(existingDatum.Size()); 
				for (std::size_t i = 0; i < existingDatum.Size(); ++i) 
				{ 
					Scope* scope = existingDatum.GetScope(i).Clone(); 
					newDatum.PushBack(*scope); 
					scope->_parent = this; 
				} 
			} 
			else 
			{ 
				newDatum = existingDatum; 
			} 
		} 
	}

	Scope::Scope(Scope&& other) noexcept : 
		_dictionary(std::move(other._dictionary)), _order(std::move(other._order))
	{
		_parent = other._parent;
		if (_parent != nullptr)
		{
			auto result = other._parent->FindContainedScope(other);
			Datum* datum = result.first;
			std::uint32_t index = result.second;

			if (datum != nullptr)
			{
				datum->Set(*this, index);
				other._parent = nullptr;
			}
		}

		for (auto& item : _order)
		{
			auto& datum = item->second;
			if (datum.Type() == Datum::DatumTypes::Table)
			{
				for (std::uint32_t i = 0; i < datum.Size(); ++i)
				{
					datum[i]._parent = this;
				}
			}

		}
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (this != &other)
		{
			Clear();

			_dictionary.Resize(other._dictionary.BucketSize());
			_order.Reserve(other._order.Size());

			for (auto& item : other._order)
			{
				Datum& existingDatum = item->second;
				Datum& newDatum = Append(item->first);
				if (existingDatum.Type() == Datum::DatumTypes::Table)
				{
					newDatum.SetType(Datum::DatumTypes::Table);
					newDatum.Reserve(existingDatum.Size());

					for (size_t i = 0; i < existingDatum.Size(); ++i)
					{
						Scope& existingScope = existingDatum.GetScope(i);
						Scope* scope = existingScope.Clone();
						scope->_parent = this;
						newDatum.PushBack(*scope);
					}
				}
				else
				{
					newDatum = existingDatum;
				}
			}
		}
		return *this;
	}
	
	Scope& Scope::operator=(Scope&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			
			_dictionary = std::move(other._dictionary);
			_order = std::move(other._order);
			_parent = other._parent;

			if (_parent != nullptr)
			{
				auto result = other._parent->FindContainedScope(other);
				Datum* datum = result.first;
				std::uint32_t index = result.second;

				if (datum != nullptr)
				{
					datum->Set(*this, index);
					other._parent = nullptr;
				}
			}

			for (auto& item : _order)
			{
				auto& datum = item->second;
				if (datum.Type() == Datum::DatumTypes::Table)
				{
					for (std::uint32_t i = 0; i < datum.Size(); ++i)
					{
						datum[i]._parent = this;
					}
				}

			}
		}
		return *this;
	}

	Scope::~Scope()
	{
		Clear();
	}

	bool Scope::operator==(const Scope& other) const
	{
		if (this != &other)
		{
			if (Size() != other.Size())
			{
				return false;
			}

			for (size_t i = 0; i < _order.Size(); ++i)
			{
				auto& lhsPair = *_order[i];
				auto& rhsPair = *other._order[i];

				if (lhsPair.first == "this")
				{
					continue;
				}

				if (lhsPair != rhsPair)
				{
					return false;
				}
			}
		}
		return true;
	}

	bool Scope::operator!=(const Scope& other) const
	{
		return !operator==(other);
	}

	Datum& Scope::operator[](std::uint32_t index)
	{
		return _order[index]->second;
	}

	Datum& Scope::operator[](std::uint32_t index) const
	{
		return _order[index]->second;
	}

	Datum& Scope::operator[](const std::string name)
	{
		return Append(name);
	}

	size_t Scope::Size() const
	{
		return _order.Size();
	}

	bool Scope::IsEmpty() const
	{
		return _order.IsEmpty();
	}

	void Scope::Clear()
	{
		for (auto& item : _dictionary)
		{
			if (item.second.Type() == Datum::DatumTypes::Table)
			{
				for (std::uint32_t i = 0; i < item.second.Size(); ++i)
				{
					delete& item.second.GetScope(i);
				}
			}
		}
		_dictionary.Clear();
		_order.Clear();
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	Datum* Scope::Find(const std::string& name)
	{
		auto result = _dictionary.Find(name);
		return (result == _dictionary.end()) ? nullptr : &(result->second);
	}

	const Datum* Scope::Find(const std::string& name) const
	{
		auto result = _dictionary.Find(name);
		return (result == _dictionary.end()) ? nullptr : &(result->second);
	}

	Datum* Scope::Search(const std::string& name)
	{
		Scope* found;
		return Search(name, found);
	}

	const Datum* Scope::Search(const std::string& name) const
	{
		const Scope* found;
		return Search(name, found);
	}

	Datum* Scope::Search(const std::string& name, Scope*& scope)
	{
		Datum* datum = Find(name);

		scope = this;

		if (datum == nullptr)
		{
			scope = nullptr;

			if (_parent != nullptr)
			{
				datum = _parent->Search(name, scope);
			}
		}

		return datum;
	}

	const Datum* Scope::Search(const std::string& name, const Scope*& scope) const
	{
		const Datum* datum = Find(name);

		scope = this;

		if (datum == nullptr)
		{
			scope = nullptr;

			if (_parent != nullptr)
			{
				datum = _parent->Search(name, scope);
			}
		}

		return datum;
	}

	Datum& Scope::Append(const std::string& name)
	{
		if (name.empty())
		{
			throw std::invalid_argument("Cannot append with an empty string.");
		}

		const auto& result = _dictionary.Insert(std::make_pair(name, Datum()));

		auto& it = result.first;
		Datum& datum = it->second;
		bool wasInserted = result.second;

		if (wasInserted)
		{
			_order.PushBack(&(*(it)));
		}

		return datum;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		Datum& datum = Append(name);
		
		if (datum.Type() != Datum::DatumTypes::Unknown && datum.Type() != Datum::DatumTypes::Table)
		{
			throw std::runtime_error("Cannot append a scope to a Datum that is not of type Unknown or type Table.");
		}

		Scope* s = new Scope();

		s->_parent = this;
		
		if (datum.Type() != Datum::DatumTypes::Table)
		{
			datum.SetType(Datum::DatumTypes::Table);
		}

		datum.PushBack(*s);

		return *s;
	}

	void Scope::Adopt(Scope& child, const std::string& name)
	{
		if (name.empty())
		{
			throw std::invalid_argument("Cannot adopt with an empty string.");
		}
		
		if (this == &child)
		{
			throw std::invalid_argument("Child argument cannot be adopted into this scope. Are you attempting to self-adopt?");
		}
		
		if (child.IsAncestorOf(*this))
		{
			throw std::invalid_argument("Child argument cannot be adopted into this scope. Are you attempting to adopt into an ancestor?");
		}

		Datum& datum = Append(name);

		if (datum.Type() != Datum::DatumTypes::Unknown && datum.Type() != Datum::DatumTypes::Table)
		{
			throw std::runtime_error("Cannot adopt a scope to a Datum that is not of type Unknown or type Table.");
		}

		if (child._parent != nullptr)
		{
			child.Orphan();
		}

		child._parent = this;

		if (datum.Type() != Datum::DatumTypes::Table)
		{
			datum.SetType(Datum::DatumTypes::Table);
		}

		datum.PushBack(child);
	}

	void Scope::Orphan()
	{
		Scope* parent = _parent;
		auto result = parent->FindContainedScope(*this);
		Datum* datum = result.first;
		std::uint32_t index = result.second;
		if (datum != nullptr)
		{
			datum->RemoveAt(index);
			_parent = nullptr;
		}
	}

	bool Scope::IsAncestorOf(const Scope& scope) const
	{
		if (this == &scope)
		{
			return false;
		}
		else if (this == scope.GetParent())
		{
			return true;
		}
		else if(scope.GetParent() != nullptr)
		{
			return IsAncestorOf(*scope.GetParent());
		}

		return false;
	}

	bool Scope::IsDescendantOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}

	Scope* Scope::GetParent() const
	{
		return _parent;
	}

	std::pair<Datum*, std::uint32_t> Scope::FindContainedScope(const Scope& scope)
	{
		for (auto& item : _dictionary)
		{
			if (item.second.Type() == Datum::DatumTypes::Table)
			{
				for (std::uint32_t i = 0; i < item.second.Size(); ++i)
				{
					if (&scope == &item.second.GetScope(i))
					{
						return std::make_pair(&item.second, i);
					}
				}
			}
		}

		return std::make_pair(nullptr, 0);
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		const Scope* other = rhs->As<Scope>();
		return other != nullptr ? *this == *other : false;
	}

	std::string Scope::ToString() const
	{
		return std::to_string(Size());
	}

	FIEAGameEngine::Datum& Scope::At(const std::string& name)
	{
		FIEAGameEngine::Datum* datum = Find(name);
		assert(datum != nullptr);
		return *datum;
	}

	const FIEAGameEngine::Datum& Scope::At(const std::string& name) const
	{
		return const_cast<Scope*>(this)->At(name);
	}
}


