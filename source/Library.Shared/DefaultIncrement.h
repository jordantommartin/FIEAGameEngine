#pragma once
#include "SizeLiteral.h"

namespace FIEAGameEngine
{
	struct DefaultIncrement final
	{
		size_t operator()(size_t /*size*/, size_t capacity) const;
	};

	inline size_t DefaultIncrement::operator()(size_t /*size*/, size_t capacity) const
	{
		return capacity;
	}

}
