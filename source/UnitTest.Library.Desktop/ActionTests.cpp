#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "Attributed.h"
#include "Entity.h"
#include "GameTime.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "WorldState.h"
#include "ActionIncrement.h"
#include "ActionIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	wstring ToString<ActionList>(const ActionList& t) 
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<ActionIf>(const ActionIf& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<ActionIncrement>(const ActionIncrement& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<ActionCreateAction>(const ActionCreateAction& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<ActionDestroyAction>(const ActionDestroyAction& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<Entity>(Entity* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	wstring ToString<Attributed>(Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(RTTITest)
		{
			Entity entity;

			RTTI* rtti = &entity;

			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("Entity"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsTrue(rtti->Is("Entity"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Entity::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Entity::TypeIdClass(), rtti->TypeIdInstance());

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			Entity* e = rtti->As<Entity>();
			Assert::IsNotNull(e);
			Assert::AreEqual(&entity, e);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&entity), fAsAttributed);

			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(Entity::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsNotNull(r);

			Entity otherFoo;
			Assert::IsTrue(rtti->Equals(&otherFoo));

			Bar otherBar(10);
			Assert::IsFalse(rtti->Equals(&otherBar));

			Assert::AreEqual("Entity"s, r->ToString());
		}

		TEST_METHOD(TestActionIncrement)
		{
			ActionIncrement increment;
			Datum& a = increment.AppendAuxiliaryAttribute("A"s);
			a = 0;
			increment.SetTarget("A"s);
			Assert::AreEqual("A"s, increment.Target());
			Assert::AreEqual(1, increment.Step()); 

			GameTime gameTime;
			WorldState worldState(gameTime);
			increment.Update(worldState);
			Assert::AreEqual(1, a.GetInteger());

			increment.Update(worldState);
			Assert::AreEqual(2, a.GetInteger());

			increment.SetStep(2);
			Assert::AreEqual(2, increment.Step());
			increment.Update(worldState);
			Assert::AreEqual(4, a.GetInteger());

			// Target does not exist
			increment.SetTarget("B");
			Assert::ExpectException<exception>([&increment, &worldState] { increment.Update(worldState); });

			// Target exists but cannot be incremented
			Datum& b = increment.AppendAuxiliaryAttribute("B"s);
			b = "This cannot be incremented"s;
			Assert::ExpectException<exception>([&increment, &worldState] { increment.Update(worldState); });
		}

		TEST_METHOD(ActionIfTest) 
		{
			WorldState worldState;
			ActionListFactory actionListFactory; 
			ActionIfFactory actionIfFactory; 
			ActionIncrementFactory actionIncrementFactory;
			ActionCreateActionFactory actionCreateActionFactory;
			ActionDestroyActionFactory actionDestroyActionFactory;
			ActionList actionList;

			{
				// Deserialization 
				JsonTableParseHelper::SharedData sharedData(actionList);
				JsonParseCoordinator parseMaster(sharedData);
				JsonTableParseHelper parseHelper;
				parseMaster.AddHelper(parseHelper);
				const std::string filename = R"(Content\TestAction.json)"s;
				parseMaster.ParseFromFile(filename);
			}

			Assert::AreEqual("Test Action"s, actionList.Name());

			auto& actionsDatum = actionList.Actions();
			Assert::AreEqual(2_z, actionsDatum.Size());
			for (size_t i = 0; i < actionsDatum.Size(); ++i)
			{
				Action* action = actionsDatum[i].As<Action>();
				Assert::IsNotNull(action);
				stringstream name;
				name << "Nested Action " << (i + 1);
				Assert::AreEqual(name.str(), action->Name());
			}

			ActionList& nestedActionList = *(actionsDatum[1].As<ActionList>());
			auto& nestedActionListActionsDatum = nestedActionList.Actions();
			Assert::AreEqual(3_z, nestedActionListActionsDatum.Size());
			for (size_t i = 0; i < nestedActionListActionsDatum.Size(); ++i)
			{
				Action* action = nestedActionListActionsDatum[i].As<Action>();
				Assert::IsNotNull(action);
				stringstream name;
				name << "Nested Nested Action " << (i + 1);
				Assert::AreEqual(name.str(), action->Name());
			}

			{
				auto testActionList = nestedActionListActionsDatum[1].As<ActionList>();
				Assert::IsNotNull(testActionList);
				Assert::IsTrue(testActionList->IsAuxiliaryAttribute("TestAttribute"));
				Datum* d = testActionList->Find("TestAttribute");
				Assert::AreEqual(100, d->GetInteger());
			}

			{
				auto action = nestedActionListActionsDatum[2].As<ActionIf>();
				Assert::IsNotNull(action);
				ActionIf& actionIf = *action;
				Assert::AreEqual(false, actionIf.Condition());

				actionIf.Update(worldState);
				Assert::AreEqual(2, actionList.At("Y").GetInteger());
			}
		}

		TEST_METHOD(ActionCreateDeleteTest)
		{
			WorldState worldState;
			ActionListFactory actionListFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionCreateActionFactory actionCreateActionFactory;
			ActionDestroyActionFactory actionDestroyActionFactory;
			Entity world;

			{
				// Deserialization 
				JsonTableParseHelper::SharedData sharedData(world);
				JsonParseCoordinator parseMaster(sharedData);
				JsonTableParseHelper parseHelper;
				parseMaster.AddHelper(parseHelper);
				const std::string filename = R"(Content\TestActionCreateDelete.json)"s;
				parseMaster.ParseFromFile(filename);
			}

			Assert::AreEqual("Test Action Create Delete"s, world.Name());

			auto& XDatum = world.At("X");
			Assert::AreEqual(0, XDatum.GetInteger());
			auto& YDatum = world.At("Y");
			Assert::AreEqual(0, YDatum.GetInteger());

			auto& actionsDatum = world.Actions();
			Assert::AreEqual(4_z, actionsDatum.Size());

			auto actionDestroyAction = actionsDatum[0].As<ActionDestroyAction>();
			Assert::IsNotNull(actionDestroyAction);
			Assert::AreEqual("DestroyIncrementX"s, actionDestroyAction->Name());

			auto actionCreateAction = actionsDatum[1].As<ActionCreateAction>();
			Assert::IsNotNull(actionCreateAction);
			Assert::AreEqual("CreateIncrementY"s, actionCreateAction->Name());

			auto actionIncrement = actionsDatum[2].As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
			Assert::AreEqual("ActionIncrementToBeDeleted"s, actionIncrement->Name());

			auto actionList = actionsDatum[3].As<ActionList>();
			Assert::IsNotNull(actionList);
			Assert::AreEqual("Nested ActionList"s, actionList->Name());

			auto nestedDestroyAction = actionList->Actions()[0].As<ActionDestroyAction>();
			Assert::IsNotNull(nestedDestroyAction);
			Assert::AreEqual("DestroyNestedIncrementX"s, nestedDestroyAction->Name());

			// First Update, Add to X twice and Create/Destroy is added to the Queue
			world.Update(worldState);
			Assert::AreEqual(4_z, actionsDatum.Size());
			auto oldActionIncrement = actionsDatum[2].As<ActionIncrement>();
			Assert::AreEqual("ActionIncrementToBeDeleted"s, oldActionIncrement->Name());
			Assert::AreEqual(2, XDatum.GetInteger());
			Assert::AreEqual(0, YDatum.GetInteger());

			// Second Update, Create/Destroy is executed, Add to Y as a result
			world.Update(worldState);
			Assert::AreEqual(4_z, actionsDatum.Size());
			auto newActionIncrement = actionsDatum[3].As<ActionIncrement>();
			Assert::AreEqual("NewIncrementY"s, newActionIncrement->Name());

			Assert::AreEqual(2, XDatum.GetInteger());
			Assert::AreEqual(1, YDatum.GetInteger());

			nestedDestroyAction->Update(worldState);
			actionDestroyAction->Update(worldState);

		}

		TEST_METHOD(EntityActionTest)
		{
			ActionIfFactory actionIfFactory;
			Entity entity; 

			string className = "ActionIf"s;
			string actionName = "NewIfAction"s;
			Action& action = entity.CreateAction(className, actionName);
			Assert::AreEqual(action.Target(),""s);

			const Entity constEntity = entity;
			Assert::AreEqual(1_z, constEntity.Actions().Size());

			ActionIf& actionIf = *action.As<ActionIf>();
			actionIf.SetCondition(true);
		}

		TEST_METHOD(ActionListTest)
		{
			ActionIfFactory actionIfFactory;
			ActionList actionList;

			string className = "ActionIf"s;
			string actionName = "NewIfAction"s;
			actionList.CreateAction(className, actionName);

			const ActionList constActionList = actionList;
			Assert::AreEqual(1_z, constActionList.Actions().Size());
		}

		TEST_METHOD(Clone) 
		{
			{
				string testName = "Test Action"s;
				ActionList action(testName);

				auto clone = action.Clone();
				Assert::AreNotSame(action, *clone);
				Assert::AreEqual(action, *clone);

				delete clone;
			}

			{
				ActionIncrement action;

				auto clone = action.Clone();
				Assert::AreNotSame(action, *clone);
				Assert::AreEqual(action, *clone);

				delete clone;
			}

			{
				ActionIf action;

				auto clone = action.Clone();
				Assert::AreNotSame(action, *clone);
				Assert::AreEqual(action, *clone);

				delete clone;
			}

			{
				ActionCreateAction action;

				auto clone = action.Clone();
				Assert::AreNotSame(action, *clone);
				Assert::AreEqual(action, *clone);

				delete clone;
			}

			{
				ActionDestroyAction action;

				auto clone = action.Clone();
				Assert::AreNotSame(action, *clone);
				Assert::AreEqual(action, *clone);

				delete clone;
			}
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionTests::sStartMemState;
}