#include "SList.h"

namespace FIEAGameEngine
{
#pragma region Node
	template<typename T>
	inline SList<T>::Node::Node(const T& data, Node* next) :
		_data(data), _next(next)
	{

	}

	template<typename T>
	inline SList<T>::Node::Node(T&& data, Node* next) :
		_data(std::forward<T>(data)), _next(next)
	{

	}
#pragma endregion Node

#pragma region Iterator
	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		_owner(&owner), _node(node)
	{

	}
	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}
	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return _owner != other._owner || _node != other._node;
	}

	template<typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated list.");
		}
		if (_node != nullptr)
		{
			_node = _node->_next;
		}

		return *this;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp(*this);
		operator++();
		return temp;
	}

	template<typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Null reference. Does this iterator == end()?");
		}
		return _node->_data;
	}

	template<typename T>
	inline T* SList<T>::Iterator::operator->() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Null reference. Does this iterator == end()?");
		}
		return &(_node->_data);
	}


#pragma endregion Iterator

#pragma region ConstIterator

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const SList& owner, Node* node) :
		_owner(&owner), _node(node)
	{

	}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _node(other._node)
	{

	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return _owner != other._owner || _node != other._node;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Unassociated list.");
		}
		if (_node != nullptr)
		{
			_node = _node->_next;
		}

		return *this;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp(*this);
		operator++();
		return temp;
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Null reference. Does this iterator == end()?");
		}
		return _node->_data;
	}

	template<typename T>
	inline const T* SList<T>::ConstIterator::operator->() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Null reference. Does this iterator == end()?");
		}
		return &(_node->_data);
	}


#pragma endregion ConstIterator

#pragma region SList
	template<typename T>
	inline SList<T>::SList(const SList& other)
	{
		for (const T& value : other)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline SList<T>::SList(SList&& other) noexcept :
		_front(other._front), _back(other._back), _size(other._size)
	{
		other._front = nullptr;
		other._back = nullptr;
		other._size = 0_z;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			Clear();

			for (const T& value : other)
			{
				PushBack(value);
			}
		}

		return *this;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& other) noexcept
	{
		if (this != &other)
		{
			Clear();

			_front = other._front;
			_back = other._back;
			_size = other._size;


			other._front = nullptr;
			other._back = nullptr;
			other._size = 0_z;
		}

		return *this;
	}

	template<typename T>
	inline SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushFront(const T& value)
	{
		_front = new Node(value, _front);
		if (_size == 0_z)
		{
			_back = _front;
		}
		++_size;
		return Iterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushFront(T&& value)
	{
		_front = new Node(std::forward<T>(value), _front);
		if (_size == 0_z)
		{
			_back = _front;
		}
		++_size;
		return Iterator(*this, _front);
	}

	template<typename T> 
	inline typename SList<T>::Iterator SList<T>::PushBack(const T& value)
	{
		Node* node = new Node(value);

		if (_size == 0)
		{
			_front = node;
		}
		else
		{
			_back->_next = node;
		}

		_back = node;
		++_size;
		return Iterator(*this, _back);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushBack(T&& value)
	{
		Node* node = new Node(std::forward<T>(value));

		if (_size == 0_z)
		{
			_front = node;
		}
		else
		{
			_back->_next = node;
		}

		_back = node;
		++_size;
		return Iterator(*this, _back);
	}

	template<typename T>
	inline void SList<T>::PopFront()
	{
		if (_size > 0_z)
		{
			Node* temp = _front;
			_front = _front->_next;
			delete temp;
			--_size;
		}
	}

	template<typename T>
	inline void SList<T>::PopBack()
	{
		if (_size == 1_z)
		{
			PopFront();
		}
		else if(_size > 1_z)
		{
			Node* currentNode = _front;
			while (currentNode->_next->_next != nullptr)
			{
				currentNode = currentNode->_next;
			}
			Node* nodeToDelete = currentNode->_next;
			currentNode->_next = nullptr;
			_back = currentNode;
			delete nodeToDelete;
			--_size;
		}
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return _size == 0_z;
	}

	template<typename T>
	inline T& SList<T>::Front()
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Empty list.");
		}
		return _front->_data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Empty list.");
		}
		return _front->_data;  
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Empty list.");
		}
		return _back->_data;
	}

	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (_size == 0_z)
		{
			throw std::runtime_error("Empty list.");
		}
		return _back->_data; 
	}

	template<typename T>
	inline size_t SList<T>::Size() const
	{
		return _size;
	}

	template<typename T>
	inline void SList<T>::Clear()
	{
		Node* currentNode = _front;
		while (currentNode != nullptr)
		{
			Node* nodeToDelete = currentNode;
			currentNode = currentNode->_next;
			delete nodeToDelete;
		}

		_size = 0_z;
		_front = _back = nullptr;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, _front);
	}
	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator(*this, _front);
	}
	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(*this, _front);
	}
	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this, nullptr);
	}
	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator(*this, nullptr);
	}
	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(*this, nullptr);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const T& value, const Iterator& it)
	{
		if (this != it._owner)
		{
			throw std::runtime_error("Iterator owners are not the same. Passed in iterator may be nullptr.");
		}

		if (it._node == nullptr || it._node == _back)
		{
			return PushBack(value);
		}

		Node* newNode = new Node(value, it._node->_next);
		it._node->_next = newNode;
		++_size;
		return Iterator(*this, newNode);
	}

	template<typename T>
	template <typename EqualityFunctor>
	inline typename SList<T>::Iterator SList<T>::Find(const T& value)
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
	inline typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
	{
		return const_cast<SList*>(this)->Find<EqualityFunctor>(value);
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline bool SList<T>::Remove(const T& value)
	{
		return Remove(Find<EqualityFunctor>(value));
	}

	template<typename T>
	inline bool SList<T>::Remove(const Iterator& it)
	{
		if (this != it._owner)
		{
			throw std::runtime_error("Iterator owners are not the same. Passed in iterator may be nullptr.");
		}

		bool found = false;
		if (it != end())
		{
			if (it._node == _back)
			{
				PopBack();
			}
			else
			{
				Node* next = it._node->_next;
				
				// Cannot use operator= when using SList within a HashMap because it stores a std::pair with a const key.
				it._node->_data.~T();
				new(&it._node->_data)T(std::move(next->_data));
				it._node->_next = next->_next;
				delete next;

				if (it._node->_next == nullptr)
				{
					_back = it._node;
				}
				--_size;
			}
			found = true;
		}
		return found;
	}
#pragma endregion SList
}



