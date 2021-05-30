#pragma once
#include "DefaultEquality.h"
#include "RTTI.h"
#include <utility>
namespace FIEAGameEngine
{
	template<typename T>
	inline bool DefaultEquality<T>::operator()(const T& lhs, const T& rhs) const
	{
		return lhs == rhs;
	}

	template<>
	struct DefaultEquality<RTTI*>
	{
		bool operator()(const RTTI* lhs, const RTTI* rhs) const
		{
			if (lhs == rhs)
			{
				return true;
			}

			if (lhs == nullptr)
			{
				return false;
			}

			return lhs->Equals(rhs);
		}
	};


	template<>
	struct DefaultEquality<char*>
	{
		bool operator()(const char* lhs, const char* rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

	template<>
	struct DefaultEquality<const char*>
	{
		bool operator()(const char* lhs, const char* rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

	template<>
	struct DefaultEquality<char* const>
	{
		bool operator()(const char* const lhs, const char* const rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};

	template<>
	struct DefaultEquality<const char* const>
	{
		bool operator()(const char* const lhs, const char* const rhs) const
		{
			return strcmp(lhs, rhs) == 0;
		}
	};
}