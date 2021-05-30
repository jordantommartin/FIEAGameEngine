#include "HashMap.h"

namespace FIEAGameEngine
{
#pragma region Iterator
	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::Iterator::Iterator(HashMap& owner, size_t index, const ChainIteratorType& chainIt) :
		_owner(&owner), _index(index), _chainIt(chainIt)
	{

	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}
	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::Iterator::operator!=(const Iterator& other) const
	{
		return _owner != other._owner || _index != other._index || _chainIt != other._chainIt;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator& HashMap<TKey, TValue>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated hashmap.");
		}

		if (_index < _owner->BucketSize())
		{
			_chainIt++;
			if(_chainIt == _owner->_buckets[_index].end())
			{
				for (_index += 1; _index < _owner->BucketSize(); ++_index)
				{
					if (!_owner->_buckets[_index].IsEmpty())
					{
						_chainIt = _owner->_buckets[_index].begin();
						break;
					}
				}
				if (_index == _owner->BucketSize())
				{
					_chainIt = ChainIteratorType();
				}
			}
		}

		return *this;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::Iterator::operator++(int)
	{
		Iterator temp(*this);
		operator++();
		return temp;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::PairType& HashMap<TKey, TValue>::Iterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated hashmap.");
		}
		if (_index >= _owner->BucketSize())
		{
			throw std::runtime_error("Iterator bucket is out of bounds.");
		}

		return (*_chainIt);
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::PairType* HashMap<TKey, TValue>::Iterator::operator->() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated hashmap.");
		}
		if (_index >= _owner->BucketSize())
		{
			throw std::runtime_error("Iterator bucket is out of bounds.");
		}

		return &(*_chainIt);
	}


#pragma endregion Iterator

#pragma region ConstIterator

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::ConstIterator::ConstIterator(const HashMap& owner, size_t index, const ConstChainIteratorType& constChainIt) :
		_owner(&owner), _index(index), _constChainIt(constChainIt)
	{

	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _index(other._index)
	{

	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return _owner != other._owner || _index != other._index;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator& HashMap<TKey, TValue>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated hashmap.");
		}

		if (_index < _owner->BucketSize())
		{
			_constChainIt++;
			if (_constChainIt == _owner->_buckets[_index].end())
			{
				for (_index += 1; _index < _owner->BucketSize(); ++_index)
				{
					if (!_owner->_buckets[_index].IsEmpty())
					{
						_constChainIt = _owner->_buckets[_index].begin();
						break;
					}
				}
				if (_index == _owner->BucketSize())
				{
					_constChainIt = ConstChainIteratorType();
				}
			}
		}

		return *this;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::ConstIterator::operator++(int)
	{
		ConstIterator temp(*this);
		operator++();
		return temp;
	}

	template<typename TKey, typename TValue>
	inline const typename HashMap<TKey, TValue>::PairType& HashMap<TKey, TValue>::ConstIterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated hashmap.");
		}
		if (_index >= _owner->BucketSize())
		{
			throw std::runtime_error("Iterator bucket is out of bounds.");
		}

		return (*_constChainIt);
	}

	template<typename TKey, typename TValue>
	inline const typename HashMap<TKey, TValue>::PairType* HashMap<TKey, TValue>::ConstIterator::operator->() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated hashmap.");
		}
		if (_index >= _owner->BucketSize())
		{
			throw std::runtime_error("Iterator bucket is out of bounds.");
		}

		return &(*_constChainIt);
	}

#pragma endregion ConstIterator


	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(size_t size, HashFunctor hFunctor, EqualityFunctor eqFunctor) :
		_hashFunctor(hFunctor), _equalityFunctor(eqFunctor)
	{
		if (size == 0_z)
		{
			throw std::runtime_error("HashMap cannot have a size of 0.");
		}
		_buckets.Resize(size);
	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(std::initializer_list<PairType> list) :
		HashMap(list, DefaultHash<TKey>(), DefaultEquality<TKey>())
	{

	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(std::initializer_list<PairType> list, HashFunctor hashFunctor, EqualityFunctor equalityFunctor) :
		HashMap(list.size(), hashFunctor, equalityFunctor)
	{
		for (const auto& pair : list)
		{
			Insert(pair);
		}
	}

	template<typename TKey, typename TValue>
	inline TValue& HashMap<TKey, TValue>::operator[](const TKey& key)
	{
		auto [it, wasInserted] = Insert(std::make_pair(key, TValue()));
		return it->second;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::Find(const TKey& key) 
	{
		size_t index = _hashFunctor(key) % BucketSize();
		ChainIteratorType chainIt = _buckets[index].begin();

		for (; chainIt != _buckets[index].end(); chainIt++)
		{
			if (_equalityFunctor(chainIt->first, key))
			{
				break;
			}
		}

		if (chainIt == _buckets[index].end())
		{
			index = BucketSize();
			chainIt = ChainIteratorType();
		}

		return Iterator(*this, index, chainIt);
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::Find(const TKey& key) const
	{
		size_t index = _hashFunctor(key) % BucketSize();
		ConstChainIteratorType chainIt = _buckets[index].begin();

		for (; chainIt != _buckets[index].end(); chainIt++)
		{
			if (_equalityFunctor((*chainIt).first, key))
			{
				break;
			}
		}

		if (chainIt == _buckets[index].end())
		{
			index = BucketSize();
			chainIt = ConstChainIteratorType();
		}

		return ConstIterator(*this, index, chainIt);
	}

	template<typename TKey, typename TValue> 
	inline std::pair<typename HashMap<TKey, TValue>::Iterator, bool> HashMap<TKey, TValue>::Insert(const PairType& entry)
	{
		size_t index = _hashFunctor(entry.first) % BucketSize();
		ChainIteratorType chainIt = _buckets[index].begin();

		bool wasInserted = false;
		for (; chainIt != _buckets[index].end(); chainIt++)
		{
			if (_equalityFunctor(chainIt->first, entry.first))
			{
				break;
			}
		}

		if (chainIt == _buckets[index].end())
		{
			chainIt = _buckets[index].PushFront(entry);
			++_size;
			wasInserted = true;
		}

		Iterator hashMapIt(*this, index, chainIt);
		return std::pair<Iterator, bool>(hashMapIt, wasInserted);
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Resize(size_t bucketSize)
	{
		HashMap map(bucketSize, _hashFunctor, _equalityFunctor);

		for (auto& item : *this)
		{
			map.Insert(std::move(item));
		}

		*this = std::move(map);
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Remove(const TKey& key)
	{
		auto it = Find(key);

		if (it != end())
		{
			_buckets[it._index].Remove(it._chainIt);
			--_size;
		}
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Clear()
	{
		for (ChainType& bucket : _buckets)
		{
			bucket.Clear();
		}

		_size = 0;
	}

	template<typename TKey, typename TValue>
	inline size_t HashMap<TKey, TValue>::Size() const
	{
		return _size;
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::IsEmpty() const
	{
		return _size == 0;
	}

	template<typename TKey, typename TValue>
	inline size_t HashMap<TKey, TValue>::BucketSize() const
	{
		return _buckets.Size();
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::ContainsKey(const TKey& key) const
	{
		return !(Find(key) == end());
	}

	template<typename TKey, typename TValue>
	inline TValue& HashMap<TKey, TValue>::At(TKey key)
	{
		Iterator it = Find(key);
		if (it == end())
		{
			throw std::runtime_error("Key has not inserted into the HashMap.");
		}

		return it->second;

	}

	template<typename TKey, typename TValue>
	inline const TValue& HashMap<TKey, TValue>::At(TKey key) const
	{
		ConstIterator it = Find(key);
		if (it == end())
		{
			throw std::runtime_error("Key has not inserted into the HashMap.");
		}

		return it->second;

	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::begin()
	{
		for (size_t i = 0_z; i < BucketSize(); ++i)
		{
			if (!_buckets[i].IsEmpty())
			{
				return Iterator(*this, i, _buckets[i].begin());
			}
		}

		return end();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::begin() const
	{
		for (size_t i = 0_z; i < BucketSize(); ++i)
		{
			if (!_buckets[i].IsEmpty())
			{
				return ConstIterator(*this, i, _buckets[i].begin());
			}
		}

		return end();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::cbegin() const
	{
		for (size_t i = 0_z; i < BucketSize(); ++i)
		{
			if (!_buckets[i].IsEmpty())
			{
				return ConstIterator(*this, i, _buckets[i].cbegin());
			}
		}

		return cend();
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::end()
	{
		return Iterator(*this, BucketSize(), ChainIteratorType());
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::end() const
	{
		return ConstIterator(*this, BucketSize(), ConstChainIteratorType());
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::cend() const
	{
		return ConstIterator(*this, BucketSize(), ConstChainIteratorType());
	}
}