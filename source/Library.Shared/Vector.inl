#include "Vector.h"

namespace FIEAGameEngine
{

#pragma region Iterator
	template<typename T>
	inline Vector<T>::Iterator::Iterator(Vector& owner, size_t index) :
		_owner(&owner), _index(index)
	{

	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}
	template<typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& other) const
	{
		return _owner != other._owner || _index != other._index;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated vector.");
		}
		if (_index <= _owner->_size)
		{
			_index++;
		}

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp(*this);
		operator++();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated vector.");
		}
		if (_index > 0_z)
		{
			_index--;
		}

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator temp(*this);
		operator--();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator+(size_t increment) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated vector.");
		}

		return Iterator(*_owner, _index + increment);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator-(size_t decrement) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated vector.");
		}

		return Iterator (*_owner, _index - decrement);
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated vector.");
		}
		return _owner->At(_index);
	}


#pragma endregion Iterator

#pragma region ConstIterator

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Vector& owner, size_t index) :
		_owner(&owner), _index(index)
	{

	}

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _index(other._index)
	{

	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return _owner != other._owner || _index != other._index;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator+(size_t increment) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated vector.");
		}

		return ConstIterator(*_owner, _index + increment);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator-(size_t decrement) const
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated vector.");
		}

		return ConstIterator(*_owner, _index - decrement);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated vector.");
		}
		if (_index <= _owner->_size)
		{
			_index++;
		}
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp(*this);
		operator++();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated vector.");
		}
		if (_index > 0_z)
		{
			_index--;
		}

		return *this;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator temp(*this);
		operator--();
		return temp;
	}

	template<typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		if (_owner == nullptr)
		{ 
			throw std::runtime_error("Null reference. Does this iterator == end()?");
		}
		return _owner->At(_index);
	}



#pragma endregion ConstIterator

	template<typename T>
	inline Vector<T>::Vector(size_t capacity)
	{
		if(capacity > 0_z)
		{
			Reserve(capacity);
		}
		
	}

	template<typename T>
	inline Vector<T>::Vector(std::initializer_list<T> list)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data + _size++)T(value);
		}

	}

	template<typename T>
	inline Vector<T>::Vector(const Vector& other)
	{
		Reserve(other._size);
		for (const auto& value : other)
		{
			new(_data + _size++)T(value);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& other) noexcept : 
		_capacity(other._capacity), _size(other._size), _data(other._data)
	{
		other._capacity = 0_z;
		other._size = 0_z;
		other._data = nullptr;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Clear();

			_size = other._size;
			Reserve(_size);

			for (size_t i = 0_z; i < _size; ++i)
			{
				new(_data + i)T(other[i]);
			}
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& other) noexcept	
	{
		if (this != &other)
		{
			Clear();
			ShrinkToFit();

			_capacity = other._capacity;
			_size = other._size;
			_data = other._data;

			other._capacity = 0_z;
			other._size = 0_z;
			other._data = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline T& Vector<T>::operator[](size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid index location.");
		}
		return _data[index];
	}

	template<typename T>
	inline const T& Vector<T>::operator[](size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid index location.");
		}
		return _data[index];
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		Clear();
		ShrinkToFit();
	}

	template<typename T>
	inline T& Vector<T>::At(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid index location.");
		}
		return _data[index];
	}

	template<typename T>
	inline const T& Vector<T>::At(size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid index location.");
		}
		return _data[index];
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return _size == 0_z;
	}

	template<typename T>
	inline size_t Vector<T>::Size() const
	{
		return _size;
	}

	template<typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return _capacity;
	}

	template<typename T>
	inline T& Vector<T>::Front()
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Empty vector.");
		}
		return _data[0];
	}

	template<typename T>
	inline const T& Vector<T>::Front() const
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Empty vector.");
		}
		return _data[0];
	}

	template<typename T>
	inline T& Vector<T>::Back()
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Empty vector.");
		}
		return _data[_size - 1];
	}

	template<typename T>
	inline const T& Vector<T>::Back() const
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Empty vector.");
		}
		return _data[_size - 1];
	}

	template<typename T>
	template<typename IncrementFunctor>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T& value)
	{
		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data + _size)T(value);

		return Iterator(*this, _size++);
	}

	template<typename T>
	template<typename IncrementFunctor>
	inline typename Vector<T>::Iterator Vector<T>::PushBack(T&& value)
	{
		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data + _size)T(std::forward<T>(value));

		return Iterator(*this, _size++);
	}

	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (!IsEmpty())
		{
			_data[--_size].~T();
		}
	}

	template<typename T>
	inline void Vector<T>::Reserve(size_t capacity)
	{
		if (capacity > _capacity)
		{
			T* data = reinterpret_cast<T*>(realloc(_data, sizeof(T) * capacity));
			assert(data != nullptr);
			_data = data;
			_capacity = capacity;
		}
	}

	template<typename T>
	inline void Vector<T>::Resize(size_t size)
	{
		if (size < _size)
		{
			for (std::size_t i = size; i < _size; ++i)
			{
				_data[i].~T();
			}
		}
		else
		{
			Reserve(size);
			for (std::size_t i = _size; i < size; ++i)
			{
				new (_data + i)T();
			}
		}

		_size = size;
	}

	template<typename T>
	void Vector<T>::Clear()
	{
		for (size_t i = 0_z; i < _size; ++i)
		{
			_data[i].~T();
		}

		_size = 0;
	}

	template<typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (_size == 0_z)
		{
			free(_data);
			_data = nullptr;
		}
		else if (_size < _capacity)
		{
			T* data = reinterpret_cast<T*>(realloc(_data, sizeof(T) * _size));
			assert(data != nullptr);
			_data = data;
		}
		_capacity = _size;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0_z);
	}
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(*this, 0_z);
	}
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(*this, 0_z);
	}
	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(*this, _size);
	}
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(*this, _size);
	}
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(*this, _size);
	}

	template<typename T>
	template <typename EqualityFunctor>
	inline typename Vector<T>::Iterator Vector<T>::Find(const T& value)
	{
		EqualityFunctor eq;

		Iterator it = begin();
		for (; it != end(); it++)
		{
			if (eq(*it, value))
			{
				break;
			}
		}
		return it;
	}

	template<typename T>
	template <typename EqualityFunctor>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
	{
		return const_cast<Vector*>(this)->Find<EqualityFunctor>(value);
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline bool Vector<T>::Remove(const T& value)
	{
		return Remove(Find<EqualityFunctor>(value));
	}

	template<typename T>
	inline bool Vector<T>::Remove(const Iterator& it)
	{
		if (it._owner != this)
		{
			throw std::runtime_error("Unassociated vector.");
		}

		bool found = false;
		if (it != end())
		{
			_data[it._index].~T();
			size_t size = sizeof(T) * (_size - it._index - 1_z);
			if (size > 0)
			{
#ifdef _WINDOWS
				memmove_s(_data + it._index, size, _data + it._index + 1_z, size);
#else
				memmove(_data + it._index, _data + it._index + 1_z, size);
#endif 
			}
			--_size;
			found = true;
		}

		return found;
	}

	template<typename T>
	inline void Vector<T>::Remove(const Iterator& first, const Iterator& last)
	{
		if (first._owner != this || last._owner != this)
		{
			throw std::runtime_error("Unassociated vector.");
		}

		if (first._index > last._index)
		{
			throw std::runtime_error("Iterators are out of order and do not represent a contiguous range of elements.");
		}

		if (first._index == last._index)
		{
			Remove(first);
		}

		size_t elementsToRemove = (last._index - first._index);
		if (elementsToRemove > 0)
		{
			Iterator it = first, end = last;
			for (; it != end; ++it)
			{
				_data[it._index].~T();
			}

			size_t elementsToMove = (_size - last._index);
			if (elementsToMove > 0)
			{
				const auto size = elementsToMove * sizeof(T);
#ifdef _WINDOWS
				memmove_s(_data + first._index, size, _data + last._index, size);
#endif

#ifndef _WINDOWS
				memmove(_data + first._index, _data + last._index, size);
#endif 
			}

		}
		_size -= elementsToRemove;
	}

}