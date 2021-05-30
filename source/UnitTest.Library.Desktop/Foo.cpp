#include "pch.h"
#include "Foo.h"
#include <stdexcept>


namespace UnitTests
{
	RTTI_DEFINITIONS(Foo)

	Foo::Foo(int value) : 
		_data(new int (value))
	{
		
	}

	Foo::Foo(const Foo& other) :
		_data(new int(*other._data))
	{

	}

	Foo::Foo(Foo&& other) noexcept :
		_data(other._data)
	{
		other._data = nullptr;
	}

	Foo& Foo::operator=(const Foo& other)
	{
		if (this != &other)
		{
			*_data = *other._data;
		}

		return *this;
	}
	
	Foo& Foo::operator=(Foo&& other) noexcept
	{
		if (this != &other)
		{
			delete _data;
			_data = other._data;
			other._data = nullptr;
		}

		return *this;
	}

	bool Foo::operator==(const Foo& other) const
	{
		if (_data == nullptr || other._data == nullptr)
		{
			throw std::runtime_error("_data should not be null. Did you move this instance?");
		}

		return *_data == *other._data;
	}

	bool Foo::operator!=(const Foo& other) const
	{
		return !(operator==(other));
	}

	Foo::~Foo()
	{
		delete _data;
	}

	int& Foo::Data()
	{
		if (_data == nullptr)
		{
			throw std::runtime_error("_data should not be null. Did you move this instance?");
		}

		return *_data;
	}

	int Foo::Data() const
	{
		if (_data == nullptr)
		{
			throw std::runtime_error("_data should not be null. Did you move this instance?");
		}

		return *_data;
	}

	bool Foo::Equals(const RTTI* rhs) const
	{
		const Foo* other = rhs->As<Foo>();
		return other != nullptr ? *this == *other : false;
	}

	std::string Foo::ToString() const
	{
		if (_data == nullptr)
		{
			throw std::runtime_error("_data should not be nullptr. Did you std::move() this instance?");
		}

		return std::to_string(*_data);
	}
}