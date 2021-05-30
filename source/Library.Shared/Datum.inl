#pragma once
#include "Datum.h"

namespace FIEAGameEngine
{
#pragma region Maps
	inline const size_t	Datum::_sizeMap[] =
	{
		sizeof(int), // DatumTypes::Integer
		sizeof(float), // DatumTypes::Float
		sizeof(std::string), // DatumTypes::std::string
		sizeof(glm::vec4), // DatumTypes::Vector
		sizeof(glm::mat4), // DatumTypes::Matrix
		sizeof(Scope*), // DatumTypes::Pointer
		sizeof(RTTI*) // DatumTypes::Pointer
	};

	inline const HashMap<const std::string, Datum::DatumTypes> Datum::_typeMap
	{
			std::pair("integer"s, Datum::DatumTypes::Integer),
			std::pair("float"s, Datum::DatumTypes::Float),
			std::pair("string"s, Datum::DatumTypes::String),
			std::pair("vector"s, Datum::DatumTypes::Vector),
			std::pair("matrix"s, Datum::DatumTypes::Matrix),
			std::pair("table"s, Datum::DatumTypes::Table)
	};
#pragma endregion

#pragma region CreateFunctions
	inline const Datum::CreateFunction Datum::_createFunctions[] =
	{
		&Datum::CreateIntegers,
		&Datum::CreateFloats,
		&Datum::CreateStrings,
		&Datum::CreateVectors,
		&Datum::CreateMatrices,
		&Datum::CreatePointers,
		&Datum::CreatePointers,
		nullptr
	};

	inline void Datum::CreateIntegers(size_t startIndex, size_t size) const
	{
		for (std::size_t i = startIndex; i < size; ++i)
		{
			new (_data.i + i)int();
		}
	}

	inline void Datum::CreateFloats(size_t startIndex, size_t size) const
	{
		for (std::size_t i = startIndex; i < size; ++i)
		{
			new (_data.f + i)float();
		}
	}

	inline void Datum::CreateStrings(size_t startIndex, size_t size) const
	{
		for (std::size_t i = startIndex; i < size; ++i)
		{
			new (_data.s + i)std::string();
		}
	}

	inline void Datum::CreateVectors(size_t startIndex, size_t size) const
	{
		for (std::size_t i = startIndex; i < size; ++i)
		{
			new (_data.v + i)glm::vec4();
		}
	}

	inline void Datum::CreateMatrices(size_t startIndex, size_t size) const
	{
		for (std::size_t i = startIndex; i < size; ++i)
		{
			new (_data.m + i)glm::mat4();
		}
	}

	inline void Datum::CreatePointers(size_t startIndex, size_t size) const
	{
		for (std::size_t i = startIndex; i < size; ++i)
		{
			new (_data.p + i)RTTI*();
		}
	}
#pragma endregion

#pragma region CopyFunctions
	inline const Datum::CopyFunction Datum::_copyFunctions[] =
	{
		&Datum::CopyIntegers,
		&Datum::CopyFloats,
		&Datum::CopyStrings,
		&Datum::CopyVectors,
		&Datum::CopyMatrices,
		&Datum::CopyPointers,
		&Datum::CopyPointers,
		nullptr
	};
	// Changes paramaters
	inline void Datum::CopyIntegers(const Datum& other) const
	{
		for (std::size_t i = 0_z; i < other._size; ++i)
		{
			new (_data.i + i)int(*(other._data.i + i));
		}
	}

	inline void Datum::CopyFloats(const Datum& other) const
	{
		for (std::size_t i = 0_z; i < other._size; ++i)
		{
			new (_data.f + i)float(*(other._data.f + i));
		}
	}

	inline void Datum::CopyStrings(const Datum& other) const
	{
		for (std::size_t i = 0_z; i < other._size; ++i)
		{
			new (_data.s + i)std::string(*(other._data.s + i));
		}
	}

	inline void Datum::CopyVectors(const Datum& other) const
	{
		for (std::size_t i = 0_z; i < other._size; ++i)
		{
			new (_data.v + i)glm::vec4(*(other._data.v + i));
		}
	}

	inline void Datum::CopyMatrices(const Datum& other) const
	{
		for (std::size_t i = 0_z; i < other._size; ++i)
		{
			new (_data.m + i)glm::mat4(*(other._data.m + i));
		}
	}

	inline void Datum::CopyPointers(const Datum& other) const
	{
		for (std::size_t i = 0_z; i < other._size; ++i)
		{
			new (_data.p + i)RTTI*(*(other._data.p + i));
		}
	}
#pragma endregion

#pragma region ToStringFunctions
	inline Datum::ToStringFunction Datum::_toStringFunctions[] =
	{
		&Datum::ToStringIntegers,
		&Datum::ToStringFloats,
		&Datum::ToStringStrings,
		&Datum::ToStringVectors,
		&Datum::ToStringMatrices,
		&Datum::ToStringPointers,
		&Datum::ToStringPointers,
		nullptr
	};

	inline std::string Datum::ToStringIntegers(size_t index) const
	{
		return std::to_string(GetInteger(index));
	}

	inline std::string Datum::ToStringFloats(size_t index) const
	{
		return std::to_string(GetFloat(index));
	}

	inline std::string Datum::ToStringStrings(size_t index) const
	{
		return GetString(index);
	}

	inline std::string Datum::ToStringVectors(size_t index) const
	{
		return glm::to_string(GetVector(index));
	}

	inline std::string Datum::ToStringMatrices(size_t index) const
	{
		return glm::to_string(GetMatrix(index));
	}

	inline std::string Datum::ToStringPointers(size_t index) const
	{
		RTTI* ptr = GetPointer(index);
		assert(ptr != nullptr);
		return ptr->ToString();
	}
#pragma endregion

#pragma region FromStringFunctions

	inline int32_t Datum::FromStringInteger(const std::string& value) const
	{
		return stoi(value);
	}

	inline float Datum::FromStringFloat(const std::string& value) const
	{
		return stof(value);
	}

	inline glm::vec4 Datum::FromStringVector(const std::string& value) const
	{
		glm::vec4 data;
		sscanf_s(value.c_str(), "vec4(%f, %f, %f, %f)", &data[0], &data[1], &data[2], &data[3]);
		return data;
	}

	inline glm::mat4 Datum::FromStringMatrix(const std::string& value) const
	{
		glm::mat4 data;
		sscanf_s(value.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
			&data[0][0], &data[0][1], &data[0][2], &data[0][3],
			&data[1][0], &data[1][1], &data[1][2], &data[1][3],
			&data[2][0], &data[2][1], &data[2][2], &data[2][3],
			&data[3][0], &data[3][1], &data[3][2], &data[3][3]
		);
		return data;
	}

#pragma endregion

#pragma region SetFromStringFunctions
	inline Datum::SetFromStringFunction Datum::_setFromStringFunctions[] =
	{
		&Datum::SetFromStringIntegers,
		&Datum::SetFromStringFloats,
		&Datum::SetFromStringStrings,
		&Datum::SetFromStringVectors,
		&Datum::SetFromStringMatrices,
		nullptr,
		nullptr,
		nullptr
	};

	inline void Datum::SetFromStringIntegers(const std::string& str, size_t index)
	{
		Set(FromStringInteger(str), index);
	}

	inline void Datum::SetFromStringFloats(const std::string& str, size_t index)
	{
		Set(FromStringFloat(str), index);
	}

	inline void Datum::SetFromStringStrings(const std::string& str, size_t index)
	{
		Set(str, index);
	}

	inline void Datum::SetFromStringVectors(const std::string& str, size_t index)
	{
		Set(FromStringVector(str), index);
	}

	inline void Datum::SetFromStringMatrices(const std::string& str, size_t index)
	{
		Set(FromStringMatrix(str), index);
	}
#pragma endregion

#pragma region PushBackFromStringFunctions
	inline Datum::PushBackFromStringFunction Datum::_pushBackFromStringFunctions[] =
	{
		&Datum::PushBackFromStringIntegers,
		&Datum::PushBackFromStringFloats,
		&Datum::PushBackFromStringStrings,
		&Datum::PushBackFromStringVectors,
		&Datum::PushBackFromStringMatrices,
		nullptr,
		nullptr,
		nullptr
	};

	inline void Datum::PushBackFromStringIntegers(const std::string& str)
	{
		PushBack(FromStringInteger(str));
	}

	inline void Datum::PushBackFromStringFloats(const std::string& str)
	{
		PushBack(FromStringFloat(str));
	}

	inline void Datum::PushBackFromStringStrings(const std::string& str)
	{
		PushBack(str);
	}

	inline void Datum::PushBackFromStringVectors(const std::string& str)
	{
		PushBack(FromStringVector(str));
	}

	inline void Datum::PushBackFromStringMatrices(const std::string& str)
	{
		PushBack(FromStringMatrix(str));
	}
#pragma endregion

#pragma region CompareFunctions
	inline const Datum::CompareFunction Datum::_compareFunctions[] =
	{
		&Datum::ComparePODs,
		&Datum::ComparePODs,
		&Datum::CompareStrings,
		&Datum::ComparePODs,
		&Datum::ComparePODs,
		&Datum::ComparePointers,
		&Datum::ComparePointers,
		nullptr
	};

	inline bool Datum::ComparePODs(const Datum& rhs) const
	{
		return memcmp(_data.vp, rhs._data.vp, _sizeMap[static_cast<int>(_type)] * _size) == 0;
	}

	inline bool Datum::CompareStrings(const Datum& rhs) const
	{
		bool isEqual = true;
		for (size_t i = 0_z; i < _size; i++)
		{
			if (_data.s[i].compare(rhs._data.s[i]) != 0)
			{
				isEqual = false;
			}
		}

		return isEqual;
	}

	inline bool Datum::ComparePointers(const Datum& rhs) const
	{
		bool isEqual = true;
		for (size_t i = 0_z; i < _size; ++i)
		{
			RTTI* lhsRtti = _data.p[i];
			RTTI* rhsRtti = rhs._data.p[i];
			if ((lhsRtti != rhsRtti) && (lhsRtti != nullptr) && !(lhsRtti->Equals(rhsRtti)))
			{
				isEqual = false;
			}
		}

		return isEqual;
	}
#pragma endregion

#pragma region PushBack Overloads
	template<typename IncrementFunctor>
	inline void Datum::PushBack(const int& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}

		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Can only PushBack an Integer if the Datum's type is Integer.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.i + _size++)int(value);

	}
	template<typename IncrementFunctor>
	inline void Datum::PushBack(int&& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}

		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Can only PushBack an Integer if the Datum's type is Integer.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.i + _size++)int(std::forward<int>(value));
	}


	template<typename IncrementFunctor>
	inline void Datum::PushBack(const float& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}

		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Can only PushBack an Float if the Datum's type is Float.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.f + _size++)float(value);
	}
	template<typename IncrementFunctor>
	inline void Datum::PushBack(float&& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}

		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Can only PushBack an Float if the Datum's type is Float.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.f + _size++)float(std::forward<float>(value));
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(const std::string& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}

		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Can only PushBack an String if the Datum's type is String.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.s + _size++)std::string(value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(std::string&& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}

		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Can only PushBack an String if the Datum's type is String.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.s + _size++)std::string(std::forward<std::string>(value));
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(const glm::vec4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}

		if (_type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Can only PushBack an Vector if the Datum's type is Vector.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.v + _size++)glm::vec4(value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(const glm::mat4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}
		
		if (_type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Can only PushBack an Matrix if the Datum's type is Matrix.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.m + _size++)glm::mat4(value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(Scope& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}

		if (_type != Datum::DatumTypes::Table)
		{
			throw std::runtime_error("Can only PushBack an Table if the Datum's type is Table.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.t + _size++)Scope*(&value);
	}

	template<typename IncrementFunctor>
	inline void Datum::PushBack(RTTI*& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot PushBack with external memory.");
		}

		if (_type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Can only PushBack an Pointer if the Datum's type is Pointer.");
		}

		IncrementFunctor incrementFunc;

		if (_size == _capacity)
		{
			size_t capacity = _capacity + std::max(1_z, incrementFunc(_size, _capacity));
			Reserve(capacity);
		}

		new(_data.p + _size++)RTTI* (value);
	}
#pragma endregion

#pragma region IndexOf
	inline size_t Datum::IndexOf(const int& value)
	{
		size_t index = 0_z;
		for (size_t i = 0_z; i < _size; i++)
		{
			if (_data.i[i] == value)
			{
				index = i;
				break;
			}
		}

		return index;
	}

	inline size_t Datum::IndexOf(const int& value) const
	{
		return const_cast<Datum*>(this)->IndexOf(value);
	}

	inline size_t Datum::IndexOf(const float& value)
	{
		size_t index = 0_z;
		for (size_t i = 0_z; i < _size; i++)
		{
			if (_data.f[i] == value)
			{
				index = i;
				break;
			}
		}

		return index;
	}

	inline size_t Datum::IndexOf(const float& value) const
	{
		return const_cast<Datum*>(this)->IndexOf(value);
	}

	inline size_t Datum::IndexOf(const std::string& value)
	{
		size_t index = 0_z;
		for (size_t i = 0_z; i < _size; i++)
		{
			if (_data.s[i] == value)
			{
				index = i;
				break;
			}
		}

		return index;
	}

	inline size_t Datum::IndexOf(const std::string& value) const
	{
		return const_cast<Datum*>(this)->IndexOf(value);
	}

	inline size_t Datum::IndexOf(const glm::vec4& value)
	{
		size_t index = 0_z;
		for (size_t i = 0_z; i < _size; i++)
		{
			if (_data.v[i] == value)
			{
				index = i;
				break;
			}
		}

		return index;
	}

	inline size_t Datum::IndexOf(const glm::vec4& value) const
	{
		return const_cast<Datum*>(this)->IndexOf(value);
	}

	inline size_t Datum::IndexOf(const glm::mat4& value)
	{
		size_t index = 0_z;
		for (size_t i = 0_z; i < _size; i++)
		{
			if (_data.m[i] == value)
			{
				index = i;
				break;
			}
		}

		return index;
	}

	inline size_t Datum::IndexOf(const glm::mat4& value) const
	{
		return const_cast<Datum*>(this)->IndexOf(value);
	}

	inline size_t Datum::IndexOf(const RTTI*& value)
	{
		size_t index = 0_z;
		for (size_t i = 0_z; i < _size; i++)
		{
			if (value->Equals(_data.p[i]))
			{
				index = i;
				break;
			}
		}

		return index;
	}

	inline size_t Datum::IndexOf(const RTTI*& value) const
	{
		return const_cast<Datum*>(this)->IndexOf(value);
	}
	
#pragma endregion

#pragma region Remove
	inline bool Datum::Remove(const int& value)
	{
		if (_type != Datum::DatumTypes::Integer)
		{
			throw std::runtime_error("Can only Remove an integer if the Datum's type is Integer.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const float& value)
	{
		if (_type != Datum::DatumTypes::Float)
		{
			throw std::runtime_error("Can only Remove an float if the Datum's type is Float.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const std::string& value)
	{
		if (_type != Datum::DatumTypes::String)
		{
			throw std::runtime_error("Can only Remove an string if the Datum's type is String.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const glm::vec4& value)
	{
		if (_type != Datum::DatumTypes::Vector)
		{
			throw std::runtime_error("Can only Remove an vector if the Datum's type is Vector.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const glm::mat4& value)
	{
		if (_type != Datum::DatumTypes::Matrix)
		{
			throw std::runtime_error("Can only Remove an matrix if the Datum's type is Matrix.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline bool Datum::Remove(const RTTI*& value)
	{
		if (_type != Datum::DatumTypes::Pointer)
		{
			throw std::runtime_error("Can only Remove an pointer if the Datum's type is Pointer.");
		}

		return RemoveAt(IndexOf(value));
	}

	inline Datum::RemoveFunction Datum::_removeFunctions[] =
	{
		&Datum::RemoveIntegers,
		&Datum::RemoveFloats,
		&Datum::RemoveStrings,
		&Datum::RemoveVectors,
		&Datum::RemoveMatrices,
		&Datum::RemovePointers,
		&Datum::RemovePointers,
		nullptr
	};

	inline void Datum::RemoveIntegers(size_t index)
	{
		size_t size = sizeof(int) * (_size - index - 1_z);
		if (size > 0)
		{
#ifdef _WINDOWS
			memmove_s(_data.i + index, size, _data.i + index + 1_z, size);
#else
			memmove(_data.i + index, _data.i + index + 1_z, size);
#endif 
		}
	}

	inline void Datum::RemoveFloats(size_t index)
	{
		size_t size = sizeof(float) * (_size - index - 1_z);
		if (size > 0)
		{
#ifdef _WINDOWS
			memmove_s(_data.f + index, size, _data.f + index + 1_z, size);
#else
			memmove(_data.f + index, _data.f + index + 1_z, size);
#endif 
		}
	}

	inline void Datum::RemoveStrings(size_t index)
	{
		size_t size = sizeof(std::string) * (_size - index - 1_z);
		if (size > 0)
		{
#ifdef _WINDOWS
			memmove_s(_data.s + index, size, _data.s + index + 1_z, size);
#else
			memmove(_data.s + index, _data.s + index + 1_z, size);
#endif 
		}
	}

	inline void Datum::RemoveVectors(size_t index)
	{
		size_t size = sizeof(glm::vec4) * (_size - index - 1_z);
		if (size > 0)
		{
#ifdef _WINDOWS
			memmove_s(_data.v + index, size, _data.v + index + 1_z, size);
#else
			memmove(_data.v + index, _data.v + index + 1_z, size);
#endif 
		}
	}

	inline void Datum::RemoveMatrices(size_t index)
	{
		size_t size = sizeof(glm::mat4) * (_size - index - 1_z);
		if (size > 0)
		{
#ifdef _WINDOWS
			memmove_s(_data.m + index, size, _data.m + index + 1_z, size);
#else
			memmove(_data.m + index, _data.m + index + 1_z, size);
#endif 
		}
	}

	inline void Datum::RemovePointers(size_t index)
	{
		size_t size = sizeof(RTTI*) * (_size - index - 1_z);
		if (size > 0)
		{
#ifdef _WINDOWS
			memmove_s(_data.p + index, size, _data.p + index + 1_z, size);
#else
			memmove(_data.p + index, _data.p + index + 1_z, size);
#endif 
		}
	}
#pragma endregion
}