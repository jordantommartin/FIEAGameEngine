#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "Power.h"
#include "Entity.h"
#include "Avatar.h"
#include "ActionIncrement.h"
#include "ActionIf.h"
#include "ActionDestroyAction.h"
#include "ActionCreateAction.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "TestReaction.h"
#include "ActionExpression.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace FIEAGameEngine;

namespace UnitTestsLibraryDesktop
{
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
		TypeManager::AddType(Power::TypeIdClass(), Power::Signatures());
		TypeManager::AddType(Entity::TypeIdClass(), Entity::Signatures());
		TypeManager::AddType(Avatar::TypeIdClass(), Avatar::Signatures());
		TypeManager::AddType(Action::TypeIdClass(), Action::Signatures());
		TypeManager::AddType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
		TypeManager::AddType(ActionList::TypeIdClass(), ActionList::Signatures());
		TypeManager::AddType(ActionIf::TypeIdClass(), ActionIf::Signatures());
		TypeManager::AddType(ActionDestroyAction::TypeIdClass(), ActionDestroyAction::Signatures());
		TypeManager::AddType(ActionCreateAction::TypeIdClass(), ActionCreateAction::Signatures());
		TypeManager::AddType(ReactionAttributed::TypeIdClass(), ReactionAttributed::Signatures());
		TypeManager::AddType(EventMessageAttributed::TypeIdClass(), EventMessageAttributed::Signatures());
		TypeManager::AddType(ActionEvent::TypeIdClass(), ActionEvent::Signatures());
		TypeManager::AddType(TestReaction::TypeIdClass(), TestReaction::Signatures());
		TypeManager::AddType(ActionExpression::TypeIdClass(), ActionExpression::Signatures());
	}

	TEST_MODULE_CLEANUP(TestModuleCleanup)
	{
		TypeManager::Clear();
	}
}