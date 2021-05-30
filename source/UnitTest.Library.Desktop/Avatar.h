#pragma once

#include "Entity.h"
#include "Vector.h"
#include "TypeManager.h"

namespace UnitTests
{
	class Avatar final : public FIEAGameEngine::Entity
	{
		RTTI_DECLARATIONS(Avatar, FIEAGameEngine::Entity)

	public:
		Avatar();

		gsl::owner<Avatar*> Clone() const override;
		int HitPoints = 100;

		static const FIEAGameEngine::Vector<FIEAGameEngine::Signature> Signatures();
	};

	ConcreteFactory(Avatar, FIEAGameEngine::Scope)
}