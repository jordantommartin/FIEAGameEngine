#include "pch.h"
#include "Attributed.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed(RTTI::IdType typeId)
	{
		(*this)["this"] = this;
		Populate(typeId);
	}

	Attributed::Attributed(const Attributed& other) :
		Scope(other)
	{
		(*this)["this"] = this;
		UpdateExternalStorage(other.TypeIdInstance());
	}


	Attributed::Attributed(Attributed&& other) noexcept :
		Scope(std::move(other))
	{
		(*this)["this"] = this;
		UpdateExternalStorage(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		Scope::operator=(other);
		(*this)["this"] = this;
		UpdateExternalStorage(other.TypeIdInstance());
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		Scope::operator=(std::move(other));
		(*this)["this"] = this;
		UpdateExternalStorage(other.TypeIdInstance());
		return *this;
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return _dictionary.ContainsKey(name);
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const 
	{
		if (name.compare("this") == 0) { return true; } 
		if (IsAttribute(name)) 
		{ 
			Vector<Signature> signatures = TypeManager::GetSignaturesForType(TypeIdInstance()); 
			auto it = std::find_if(signatures.begin(), signatures.end(), [&name](const Signature& signature) { return signature.Name == name; }); 
			return it != signatures.end(); 
		} 
		return false; 
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return (IsAttribute(name) && !IsPrescribedAttribute(name));
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::runtime_error("Cannot append an auxiliary attribute with the same name as an already existing prescribed attribute.");
		}
		return Append(name);
	}


	const Vector<Scope::PairType*>& Attributed::Attributes() const
	{
		return _order;
	}

	Vector<Scope::PairType*> Attributed::PrescribedAttributes() const {
		const auto signatures = TypeManager::GetSignaturesForType(TypeIdInstance()); 
		Vector<PairType*> prescribedAttributes; 
		size_t prescribedAttributeCount = signatures.Size() + 1; // +1 for the "this" attribute 
		prescribedAttributes.Reserve(prescribedAttributeCount); 
		for (size_t i = 0; i < prescribedAttributeCount; ++i) 
		{ 
			prescribedAttributes.PushBack(_order[i]); 
		} 
		return prescribedAttributes; 
	} 

	Vector<Scope::PairType*> Attributed::AuxiliaryAttributes() const 
	{
		const auto signatures = TypeManager::GetSignaturesForType(TypeIdInstance()); 
		Vector<PairType*> auxiliaryAttributes; 
		size_t auxiliaryAttributeBeginIndex = signatures.Size() + 1; // +1 for the "this" attribute 
		auxiliaryAttributes.Reserve(_order.Size() - auxiliaryAttributeBeginIndex); 
		for (size_t i = auxiliaryAttributeBeginIndex; i < _order.Size(); ++i)
		{ 
			auxiliaryAttributes.PushBack(_order[i]);
		} 
		return auxiliaryAttributes; 
	} 

	void Attributed::Populate(RTTI::IdType typeId)
	{
		Vector<Signature> signatures = TypeManager::GetSignaturesForType(typeId);
		for (const Signature& signature : signatures)
		{
			assert(signature.Type != Datum::DatumTypes::Unknown);

			Datum& datum = Append(signature.Name);

			if(signature.Type == Datum::DatumTypes::Table)
			{
				datum.SetType(Datum::DatumTypes::Table);
				datum.Reserve(signature.Size);
				for (size_t i = 0; i < signature.Size; ++i)
				{
					AppendScope(signature.Name);
				}
			}
			else
			{
				void* data = reinterpret_cast<std::uint8_t*>(this) + signature.StorageOffset;
				datum.SetStorage(signature.Type, data, signature.Size);
			}
		}
	}

	void Attributed::UpdateExternalStorage(RTTI::IdType typeId) 
	{
		const auto signatures = TypeManager::GetSignaturesForType(typeId); 
		assert(_order.Size() >= signatures.Size() + 1); // +1 for "this" 
		for (size_t i = 1; i < signatures.Size() + 1; ++i) 
		{ 
			const auto& signature = (signatures)[i - 1]; 
			auto&[key, datum] = *(_order[i]);
			assert(key == signature.Name); 
			assert(datum.Type() == signature.Type); 
#if defined(NDEBUG) 
			UNREFERENCED_LOCAL(key); 
#endif 
			if (datum.Type() != Datum::DatumTypes::Table) 
			{ 
				assert(datum._isExternal); 
				assert(datum.Size() == signature.Size); 
				void* data = reinterpret_cast<uint8_t*>(this) + signature.StorageOffset; 
				datum.SetStorage(signature.Type, data, signature.Size); 
			} 
		} 
	} 
}