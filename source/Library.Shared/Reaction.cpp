#include "pch.h"
#include "Reaction.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Reaction)

	Reaction::Reaction(RTTI::IdType type) :
		ActionList(type)
	{

	}
}