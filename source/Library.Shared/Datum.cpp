#include "pch.h"
#include "Datum.h"
#include <stdexcept>
#include <cassert>

using namespace glm;
using namespace std;

namespace FIEAGameEngine
{
	Datum::Datum(DatumTypes type) :
		_type(type)
	{

	}

	Datum::Datum(const Datum& other) :
		_type(other._type), _size(other._size)
	{
		if (other._size > 0_z)
		{
			if (other._isExternal)
			{
				_data = other._data;
				_capacity = other._capacity;
				_isExternal = other._isExternal;
			}
			else
			{
				Reserve(other._size);

				CopyFunction func = _copyFunctions[static_cast<size_t>(_type)];
				assert(func != nullptr);
				(this->*func)(other);
			}
		}
	}



	Datum::Datum(Datum&& other) noexcept :
		_capacity(other._capacity), _size(other._size), _data(other._data), _type(other._type), _isExternal(other._isExternal)
	{
		other._capacity = 0_z;
		other._size = 0_z;
		other._data.vp = nullptr;
		other._type = DatumTypes::Unknown;
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			_type = other._type;
			Clear();
			

			if (other._isExternal)
			{
				_data = other._data;
				_capacity = other._capacity;
				_isExternal = other._isExternal;
			}
			else
			{
				Reserve(other._capacity);

				CopyFunction func = _copyFunctions[static_cast<size_t>(_type)];
				assert(func != nullptr);
				(this->*func)(other);
			}
			_size = other._size;
		}

		return *this;
	}

	Datum& Datum::operator=(Datum&& other) noexcept
	{
		if (this != &other)
		{
			_type = other._type;

			Clear();
			if (_capacity > 0_z)
			{
				free(_data.vp);
			}

			_capacity = other._capacity;
			_size = other._size;
			_data = other._data;
			_isExternal = other._isExternal;


			other._capacity = 0_z;
			other._size = 0_z;
			other._data.vp = nullptr;
			other._type = DatumTypes::Unknown;
		}

		return *this;
	}

	Datum::~Datum()
	{
		if (!_isExternal)
		{
			Clear();
			free(_data.vp);
		}
	}

	bool Datum::operator==(const Datum& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const Datum& other) const
	{
		if (_type == DatumTypes::Unknown || other._type == DatumTypes::Unknown)
		{
			return !(_type == DatumTypes::Unknown && other._type == DatumTypes::Unknown);
		} 

		if (other._type != _type || _size != other._size)
		{
			return true;
		}

		bool isEqual = false;
		CompareFunction func = _compareFunctions[static_cast<size_t>(_type)];
		assert(func != nullptr);
		isEqual = (this->*func)(other);

		return !(isEqual);
	}

	Scope& Datum::operator[](size_t index) 
	{ 
		return GetScope(index); 
	}

#pragma region CopyConstructorOverloads
	Datum::Datum(const int& value) :
		_type(Datum::DatumTypes::Integer)
	{
		Resize(1_z);
		*_data.i = value;
	}

	Datum::Datum(const float& value) :
		_type(Datum::DatumTypes::Float)
	{
		Resize(1_z);
		*_data.f = value;
	}

	Datum::Datum(const std::string& value) :
		_type(Datum::DatumTypes::String)
	{
		Resize(1_z);
		*_data.s = value;
	}

	Datum::Datum(const glm::vec4& value) :
		_type(Datum::DatumTypes::Vector)
	{
		Resize(1_z);
		*_data.v = value;
	}

	Datum::Datum(const glm::mat4& value) :
		_type(Datum::DatumTypes::Matrix)
	{
		Resize(1_z);
		*_data.m = value;
	}

	Datum::Datum(RTTI*& value) :
		_type(Datum::DatumTypes::Pointer)
	{
		Resize(1_z);
		*_data.p = value;
	}
#pragma endregion

#pragma region InitializerListCopyConstructorOverloads

	Datum::Datum(std::initializer_list<int> list) :
		Datum(Datum::DatumTypes::Integer)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.i + _size++)int(value);
		}
	}

	Datum::Datum(std::initializer_list<float> list) :
		Datum(Datum::DatumTypes::Float)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.f + _size++)float(value);
		}
	}

	Datum::Datum(std::initializer_list<string> list) :
		Datum(Datum::DatumTypes::String)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.s + _size++)string(value);
		}
	}

	Datum::Datum(std::initializer_list<glm::vec4> list) :
		Datum(Datum::DatumTypes::Vector)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.v + _size++)glm::vec4(value);
		}
	}

	Datum::Datum(std::initializer_list<glm::mat4> list) :
		Datum(Datum::DatumTypes::Matrix)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.m + _size++)glm::mat4(value);
		}
	}

	Datum::Datum(std::initializer_list<RTTI*> list) :
		Datum(Datum::DatumTypes::Pointer)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			new(_data.p + _size++)RTTI* (value);
		}
	}

#pragma endregion

#pragma region CopyAssignmentOverloads
	Datum& Datum::operator=(const int& value)
	{
		SetType(Datum::DatumTypes::Integer);
		if (_size != 1_z)
		{
			Resize(1_z);
		}
		*_data.i = value;
		return *this;
	}

	Datum& Datum::operator=(const float& value)
	{
		SetType(Datum::DatumTypes::Float);
		if (_size != 1_z)
		{
			Resize(1_z);
		}
		*_data.f = value;
		return *this;
	}

	Datum& Datum::operator=(const std::string& value)
	{
		SetType(Datum::DatumTypes::String);
		if (_size != 1_z)
		{
			Resize(1_z);
		}
		*_data.s = value;
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& value)
	{
		SetType(Datum::DatumTypes::Vector);
		if (_size != 1_z)
		{
			Resize(1_z);
		}
		*_data.v = value;
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& value)
	{
		SetType(Datum::DatumTypes::Matrix);
		if (_size != 1_z)
		{
			Resize(1_z);
		}
		*_data.m = value;
		return *this;
	}

	Datum& Datum::operator=(Scope& value)
	{
		SetType(Datum::DatumTypes::Table);
		if (_size != 1_z)
		{
			Resize(1_z);
		}
		*_data.t = &(value);
		return *this;
	}

	Datum& Datum::operator=(RTTI* value)
	{
		SetType(Datum::DatumTypes::Pointer);
		if (_size != 1_z)
		{
			Resize(1_z);
		}
		*_data.p = value;
		return *this;
	}

#pragma endregion

#pragma region ComparisonOperatorOverloads

	bool Datum::operator==(const int& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const int& other) const
	{
		if (_type == DatumTypes::Unknown)
		{
			return false;
		}

		return !(_type == Datum::DatumTypes::Integer && _size == 1_z && _data.i[0_z] == other);
	}

	bool Datum::operator==(const float& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const float& other) const
	{
		if (_type == DatumTypes::Unknown)
		{
			return false;
		}

		return !(_type == Datum::DatumTypes::Float && _size == 1_z && _data.f[0_z] == other);
	}

	bool Datum::operator==(const std::string& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const std::string& other) const
	{
		if (_type == DatumTypes::Unknown)
		{
			return false;
		}

		return !(_type == Datum::DatumTypes::String && _size == 1_z && !_data.s[0_z].compare(other));
	}

	bool Datum::operator==(const glm::vec4& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const glm::vec4& other) const
	{
		if (_type == DatumTypes::Unknown)
		{
			return false;
		}

		return !(_type == Datum::DatumTypes::Vector && _size == 1_z && _data.v[0_z] == other);
	}

	bool Datum::operator==(const glm::mat4& other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(const glm::mat4& other) const
	{
		if (_type == DatumTypes::Unknown)
		{
			return false;
		}

		return !(_type == Datum::DatumTypes::Matrix && _size == 1_z && _data.m[0_z] == other);
	}

	bool Datum::operator==(RTTI* const & other) const
	{
		return !(operator!=(other));
	}

	bool Datum::operator!=(RTTI* const & other) const
	{
		if (_type == DatumTypes::Unknown)
		{
			return false;
		}
		return !(_type == Datum::DatumTypes::Pointer && _size == 1_z && _data.p[0_z]->Equals(other));
	}

#pragma endregion

	Datum::DatumTypes Datum::Type() const
	{
		return _type;
	}

	void Datum::SetType(DatumTypes type)
	{
		if (_type != DatumTypes::Unknown && type != _type)
		{
			throw std::runtime_error("You cannot change a datum's type once set.");
		}

		_type = type;
	}

	size_t Datum::Size() const
	{
		return _size;
	}

	size_t Datum::Capacity() const
	{
		return _capacity;
	}

	void Datum::Reserve(size_t capacity)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot reserve with external memory.");
		}

		if (_type == Datum::DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot reserve with an unknown type.");
		}

		if (capacity > _capacity)
		{
			size_t size = _sizeMap[static_cast<size_t>(_type)];

			void* data = realloc(_data.vp, capacity * size);
			assert(data != nullptr);
			_data.vp = data;
			_capacity = capacity;
		}
	}

#pragma region SetOverloads
	void Datum::Set(const int& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Datum type must be an integer is order to set with an integer.");
		}

		_data.i[index] = value;
	}

	void Datum::Set(const float& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Datum type must be an float is order to set with an float.");
		}

		_data.f[index] = value;
	}

	void Datum::Set(const std::string& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Datum type must be an String is order to set with an String.");
		}

		_data.s[index] = value;
	}

	void Datum::Set(const glm::vec4& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		if (_type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Datum type must be an Vector is order to set with an Vector.");
		}

		_data.v[index] = value;
	}

	void Datum::Set(const glm::mat4& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		if (_type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Datum type must be an Matrix is order to set with an Matrix.");
		}

		_data.m[index] = value;
	}

	void Datum::Set(Scope& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		if (_type != Datum::DatumTypes::Table)
		{
			throw std::runtime_error("Datum type must be an Pointer is order to set with an Pointer.");
		}

		_data.t[index] = &value;
	}

	void Datum::Set(RTTI*& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		if (_type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Datum type must be an Pointer is order to set with an Pointer.");
		}

		_data.p[index] = value;
	}
#pragma endregion

#pragma region GetFunctions
	int& Datum::GetInteger(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Can only use GetInteger when the Datum's type is Integer.");
		}

		return _data.i[index];
	}
	float& Datum::GetFloat(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Can only use GetFloat when the Datum's type is Float.");
		}

		return _data.f[index];
	}
	std::string& Datum::GetString(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Can only use GetString when the Datum's type is String.");
		}

		return _data.s[index];
	}
	glm::vec4& Datum::GetVector(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Can only use GetVector when the Datum's type is Vector.");
		}

		return _data.v[index];
	}
	glm::mat4& Datum::GetMatrix(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Can only use GetMatrix when the Datum's type is Matrix.");
		}

		return _data.m[index];
	}

	Scope& Datum::GetScope(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Table)
		{
			throw std::runtime_error("Can only use GetPointer when the Datum's type is Pointer.");
		}

		return *_data.t[index];
	}

	RTTI*& Datum::GetPointer(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Can only use GetPointer when the Datum's type is Pointer.");
		}

		return _data.p[index];
	}

	int& Datum::GetInteger(size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Can only use GetInteger when the Datum's type is Integer.");
		}

		return _data.i[index];
	}
	float& Datum::GetFloat(size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Can only use GetFloat when the Datum's type is Float.");
		}

		return _data.f[index];
	}
	std::string& Datum::GetString(size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Can only use GetString when the Datum's type is String.");
		}

		return _data.s[index];
	}
	glm::vec4& Datum::GetVector(size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Can only use GetVector when the Datum's type is Vector.");
		}

		return _data.v[index];
	}
	glm::mat4& Datum::GetMatrix(size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Can only use GetMatrix when the Datum's type is Matrix.");
		}

		return _data.m[index];
	}

	Scope& Datum::GetScope(size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Table)
		{
			throw std::runtime_error("Can only use GetPointer when the Datum's type is Pointer.");
		}

		return *_data.t[index];
	}

	RTTI*& Datum::GetPointer(size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Index out of bounds");
		}

		if (_type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Can only use GetPointer when the Datum's type is Pointer.");
		}

		return _data.p[index];
	}
#pragma endregion

	bool Datum::isEmpty() const
	{
		return _size == 0_z;
	}

	bool Datum::IsExternal() const
	{
		return _isExternal;
	}

	void Datum::Resize(size_t size)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot resize with external memory.");
		}

		if (_type == Datum::DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot resize with an unknown type.");
		}

		if (size < _size)
		{
			if (_type == Datum::DatumTypes::String)
			{
				for (std::size_t i = size; i < _size; ++i)
				{
					_data.s[i].~string();
				}
			}
			_capacity = _size;
		}
		else
		{
			Reserve(size);
			CreateFunction func = _createFunctions[static_cast<int>(_type)];
			assert(func != nullptr);
			(this->*func)(_size, size);
		}

		_size = size;
	}

	void Datum::Clear()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot clear with external memory.");
		}

		if (_type == Datum::DatumTypes::String)
		{
			for (std::size_t i = 0_z; i < _size; ++i)
			{
				_data.s[i].~string();
			}
		}

		_size = 0;
	}

	void Datum::SetStorage(int* array, size_t size)
	{
		if (size == 0_z)
		{
			throw std::runtime_error("Cannot set storage with a size of 0.");
		}

		if (_type != Datum::DatumTypes::Unknown && _type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Cannot set storage to integer type if the current type is not Unknown or Integer.");
		}

		if (_size != 0_z && !_isExternal)
		{
			throw std::runtime_error("Cannot set storage to external if internal storage already exists.");
		}
		SetType(Datum::DatumTypes::Integer);
		_data.i = array;
		_size = size;
		_capacity = size;
		_isExternal = true;
	}

	void Datum::SetStorage(float* array, size_t size)
	{
		if (size == 0_z)
		{
			throw std::runtime_error("Cannot set storage with a size of 0.");
		}

		if (_type != Datum::DatumTypes::Unknown && _type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Cannot set storage to float type if the current type is not Unknown or Float.");
		}

		if (_size != 0_z && !_isExternal)
		{
			throw std::runtime_error("Cannot set storage to external if internal storage already exists.");
		}

		SetType(Datum::DatumTypes::Float);
		_data.f = array;
		_size = size;
		_capacity = size;
		_isExternal = true;
	}

	void Datum::SetStorage(std::string* array, size_t size)
	{
		if (size == 0_z)
		{
			throw std::runtime_error("Cannot set storage with a size of 0.");
		}

		if (_type != Datum::DatumTypes::Unknown && _type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Cannot set storage to String type if the current type is not Unknown or String.");
		}

		if (_size != 0_z && !_isExternal)
		{
			throw std::runtime_error("Cannot set storage to external if internal storage already exists.");
		}
		SetType(Datum::DatumTypes::String);
		_data.s = array;
		_size = size;
		_capacity = size;
		_isExternal = true;
	}

	void Datum::SetStorage(glm::vec4* array, size_t size)
	{
		if (size == 0_z)
		{
			throw std::runtime_error("Cannot set storage with a size of 0.");
		}

		if (_type != Datum::DatumTypes::Unknown && _type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Cannot set storage to Vector type if the current type is not Unknown or Vector.");
		}

		if (_size != 0_z && !_isExternal)
		{
			throw std::runtime_error("Cannot set storage to external if internal storage already exists.");
		}
		SetType(Datum::DatumTypes::Vector);
		_data.v = array;
		_size = size;
		_capacity = size;
		_isExternal = true;
	}

	void Datum::SetStorage(glm::mat4* array, size_t size)
	{
		if (size == 0_z)
		{
			throw std::runtime_error("Cannot set storage with a size of 0.");
		}

		if (_type != Datum::DatumTypes::Unknown && _type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Cannot set storage to Matrix type if the current type is not Unknown or Matrix.");
		}

		if (_size != 0_z && !_isExternal)
		{
			throw std::runtime_error("Cannot set storage to external if internal storage already exists.");
		}
		SetType(Datum::DatumTypes::Matrix);
		_data.m = array;
		_size = size;
		_capacity = size;
		_isExternal = true;
	}

	void Datum::SetStorage(RTTI** array, size_t size)
	{
		if (size == 0_z)
		{
			throw std::runtime_error("Cannot set storage with a size of 0.");
		}

		if (_type != Datum::DatumTypes::Unknown && _type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Cannot set storage to pointer type if the current type is not Unknown or Pointer.");
		}

		if (_size != 0_z && !_isExternal)
		{
			throw std::runtime_error("Cannot set storage to external if internal storage already exists.");
		}
		SetType(Datum::DatumTypes::Pointer);
		_data.p = array;
		_size = size;
		_capacity = size;
		_isExternal = true;
	}

	void Datum::SetStorage(DatumTypes type, void* array, size_t size)
	{
		if (size == 0_z)
		{
			throw std::runtime_error("Cannot set storage with a size of 0.");
		}

		if (_type == Datum::DatumTypes::Unknown)
		{
			SetType(type);
		}
		
		else if (_type != type)
		{
			throw runtime_error("Invalid operation. Type has already been set and is different than passed in type.");
		}

		if (_capacity > 0_z && !_isExternal)
		{
			throw std::runtime_error("Cannot set storage to external if internal storage already exists.");
		}

		_data.vp = array;
		_size = size;
		_capacity = size;
		_isExternal = true;
	}

	std::string Datum::ToString(size_t index) const
	{
		ToStringFunction func = _toStringFunctions[static_cast<size_t>(_type)];
		assert(func != nullptr);
		return (this->*func)(index);
	}


	void Datum::SetFromString(const std::string& str, size_t index)
	{
		if (_type == Datum::DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot set from string if type is Unknown.");
		}

		if (_type == Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Cannot set from string if type is Pointer");
		}

		SetFromStringFunction func = _setFromStringFunctions[static_cast<size_t>(_type)];
		assert(func != nullptr);
		(this->*func)(str, index);
	}

	void Datum::PushBackFromString(const string& value)
	{
		PushBackFromStringFunction func = _pushBackFromStringFunctions[static_cast<size_t>(_type)];
		assert(func != nullptr);
		(this->*func)(value);
	}

#pragma region Front
	int& Datum::FrontInteger()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Cannot get the front as an integer is the Datum is not an integer type.");
		}

		return _data.i[0_z];
	}

	const int& Datum::FrontInteger() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Cannot get the front as an integer is the Datum is not an integer type.");
		}

		return _data.i[0_z];
	}

	float& Datum::FrontFloat()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Cannot get the front as an Float is the Datum is not an Float type.");
		}

		return _data.f[0_z];
	}

	const float& Datum::FrontFloat() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Cannot get the front as an Float is the Datum is not an Float type.");
		}

		return _data.f[0_z];
	}

	std::string& Datum::FrontString()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Cannot get the front as an String is the Datum is not an String type.");
		}

		return _data.s[0_z];
	}

	const std::string& Datum::FrontString() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Cannot get the front as an String is the Datum is not an String type.");
		}

		return _data.s[0_z];
	}

	glm::vec4& Datum::FrontVector()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Cannot get the front as an Vector is the Datum is not an Vector type.");
		}

		return _data.v[0_z];
	}

	const glm::vec4& Datum::FrontVector() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Cannot get the front as an Vector is the Datum is not an Vector type.");
		}

		return _data.v[0_z];
	}

	glm::mat4& Datum::FrontMatrix()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Cannot get the front as an Matrix is the Datum is not an Matrix type.");
		}

		return _data.m[0_z];
	}

	const glm::mat4& Datum::FrontMatrix() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Cannot get the front as an Matrix is the Datum is not an Matrix type.");
		}

		return _data.m[0_z];
	}

	RTTI*& Datum::FrontPointer()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Cannot get the front as an Pointer is the Datum is not an Pointer type.");
		}

		return _data.p[0_z];
	}

	RTTI* const & Datum::FrontPointer() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get front if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Cannot get the front as an Pointer is the Datum is not an Pointer type.");
		}

		return _data.p[0_z];
	}
#pragma endregion

#pragma region Back
	int& Datum::BackInteger()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Cannot get the back as an integer is the Datum is not an integer type.");
		}

		return _data.i[_size - 1_z];
	}

	const int& Datum::BackInteger() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Cannot get the back as an integer is the Datum is not an integer type.");
		}

		return _data.i[_size - 1_z];
	}

	float& Datum::BackFloat()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Cannot get the back as an Float is the Datum is not an Float type.");
		}

		return _data.f[_size - 1_z];
	}

	const float& Datum::BackFloat() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Cannot get the back as an Float is the Datum is not an Float type.");
		}

		return _data.f[_size - 1_z];
	}

	std::string& Datum::BackString()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Cannot get the back as an String is the Datum is not an String type.");
		}

		return _data.s[_size - 1_z];
	}

	const std::string& Datum::BackString() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Cannot get the back as an String is the Datum is not an String type.");
		}

		return _data.s[_size - 1_z];
	}

	glm::vec4& Datum::BackVector()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Cannot get the back as an Vector is the Datum is not an Vector type.");
		}

		return _data.v[_size - 1_z];
	}

	const glm::vec4& Datum::BackVector() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Cannot get the back as an Vector is the Datum is not an Vector type.");
		}

		return _data.v[_size - 1_z];
	}

	glm::mat4& Datum::BackMatrix()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Cannot get the back as an Matrix is the Datum is not an Matrix type.");
		}

		return _data.m[_size - 1_z];
	}

	const glm::mat4& Datum::BackMatrix() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Cannot get the back as an Matrix is the Datum is not an Matrix type.");
		}

		return _data.m[_size - 1_z];
	}

	RTTI*& Datum::BackPointer()
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Cannot get the back as an Pointer is the Datum is not an Pointer type.");
		}

		return _data.p[_size - 1_z];
	}

	RTTI* const & Datum::BackPointer() const
	{
		if (isEmpty())
		{
			throw std::runtime_error("Cannot get back if Datum is empty.");
		}

		if (_type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Cannot get the back as an Pointer is the Datum is not an Pointer type.");
		}

		return _data.p[_size - 1_z];
	}
#pragma endregion

	void Datum::PopBack()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PopBack with external memory.");
		}

		if (_type == Datum::DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot pop back if type is Unknown.");
		}

		if (!isEmpty())
		{
			--_size;
			if (_type == Datum::DatumTypes::String)
			{
				_data.s[_size].~string();
			}
		}
	}

	bool Datum::RemoveAt(size_t index)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot Remove with external memory.");
		}

		if (_type == Datum::DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot remove if a Datum's type is Unknown.");
		}

		bool found = false;
		if (index < _size)
		{
			if (_type == Datum::DatumTypes::String)
			{
				_data.s[index].~string();
			}

			RemoveFunction func = _removeFunctions[static_cast<size_t>(_type)];
			assert(func != nullptr);
			(this->*func)(index);

			--_size;
			found = true;
		}

		return found;
	}
}
