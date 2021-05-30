#include "Factory.h"

namespace FIEAGameEngine
{
	template <typename T>
	gsl::owner<T*> Factory<T>::Create(const std::string& className)
{
		auto ptr = Find(className);
		if (ptr != nullptr)
		{
			return ptr->Create();
		}
		return nullptr;
	}

	template <typename T>
	inline bool Factory<T>::IsEmpty()
	{
		return _factories.Size() == 0;
	}

	template <typename T>
	inline size_t Factory<T>::Size()
	{
		return _factories.Size();
	}

	template<typename T>
	inline const Factory<T>* const FIEAGameEngine::Factory<T>::Find(const std::string& className)
	{
		auto it = _factories.Find(className);
		if (it != _factories.end())
		{
			
			return it->second;
		}
		
		return nullptr;
	}

	template<typename T>
	inline void Factory<T>::Add(const Factory& factory)
	{
		auto pair = std::make_pair(factory.ClassName(), &factory);
		auto [it, wasInserted] = _factories.Insert(pair);
		if (!wasInserted)
		{
			throw std::runtime_error("Factory has already been inserted.");
		}
	}

	template<typename T>
	inline void Factory<T>::Remove(const Factory& factory)
	{
		_factories.Remove(factory.ClassName());
	}
}