#include "pch.h"
#include "Bar.h"
#include <stdexcept>


namespace UnitTests
{
	RTTI_DEFINITIONS(Bar);

	Bar::Bar(int value) : 
		_data(new int (value))
	{

	}

	Bar::Bar(const Bar& other) :
		_data(new int(*other._data))
	{
	}

	Bar::Bar(Bar&& other) noexcept :
		_data(other._data)
	{
		other._data = nullptr;
	}

	Bar& Bar::operator=(const Bar& other)
	{
		if (this != &other)
		{
			*_data = *other._data;
		}

		return *this;
	}
	
	Bar& Bar::operator=(Bar&& other) noexcept
	{
		if (this != &other)
		{
			delete _data;
			_data = other._data;
			other._data = nullptr;
		}

		return *this;
	}

	Bar::~Bar()
	{
		
		delete _data;
	}

	int& Bar::Data()
	{
		if (_data == nullptr)
		{
			throw std::runtime_error("_data should not be null. Did you move this instance?");
		}

		return *_data;
	}

	int Bar::Data() const
	{
		if (_data == nullptr)
		{
			throw std::runtime_error("_data should not be null. Did you move this instance?");
		}

		return *_data;
	}
}