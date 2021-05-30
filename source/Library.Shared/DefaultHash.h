#pragma once
#include "SizeLiteral.h"

namespace FIEAGameEngine
{
	template <typename TKey>
	struct DefaultHash final
	{
		size_t operator()(const TKey& key) const;
	};
}

#include "DefaultHash.inl"