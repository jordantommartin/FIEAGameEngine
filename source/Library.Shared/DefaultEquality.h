#pragma once

namespace FIEAGameEngine
{
	template <typename T>
	struct DefaultEquality final
	{
		bool operator()(const T& lhs, const T& rhs) const;
	};

}

#include "DefaultEquality.inl"

