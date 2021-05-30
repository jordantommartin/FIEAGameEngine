#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <chrono>
#include "Bar.h"
#include "JsonTableParseHelper.h"
#include "JsonParseCoordinator.h"
#include "Event.h"
#include "EventQueue.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "WorldState.h"
#include "Entity.h"
#include "ActionIncrement.h"
#include "TypeManager.h"
#include "TestReaction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;
using namespace std::chrono;
using namespace std::chrono_literals;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	wstring ToString<ReactionAttributed>(ReactionAttributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	wstring ToString<ReactionAttributed>(const ReactionAttributed& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<ActionEvent>(const ActionEvent& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<TestReaction>(const TestReaction& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<Datum::DatumTypes>(const Datum::DatumTypes& t)
	{
		return ToString(static_cast<int>(t));
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ReactionTests)
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
			ReactionAttributed a;

			RTTI* rtti = &a;

			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("ReactionAttributed"s));
			Assert::IsTrue(rtti->Is("Reaction"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ReactionAttributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ReactionAttributed::TypeIdClass(), rtti->TypeIdInstance());

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			ReactionAttributed* f = rtti->As<ReactionAttributed>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);

			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(ReactionAttributed::TypeIdClass());
			Assert::IsNotNull(r);

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(Constructor)
		{
			{
				const string name = "Test Reaction"s;
				const string subtype = "Test"s;
				ReactionAttributed reaction(name, subtype);
				Assert::IsTrue(reaction.IsPrescribedAttribute("Name"s));
				Assert::IsTrue(reaction.IsPrescribedAttribute("Actions"s));
				Assert::IsTrue(reaction.IsPrescribedAttribute("Subtype"s));
				Assert::AreEqual(name, reaction.Name());
				Assert::AreEqual(subtype, reaction.GetSubtype());

				const ActionList& constReaction = reaction;
				Assert::IsTrue(&reaction.Actions() == &constReaction.Actions());
			}

			{
				ReactionAttributed reaction;
				Assert::IsTrue(reaction.IsPrescribedAttribute("Name"s));
				Assert::IsTrue(reaction.IsPrescribedAttribute("Actions"s));
				Assert::IsTrue(reaction.Name().empty());
				Assert::IsTrue(reaction.GetSubtype().empty());

				const ActionList& constReaction = reaction;
				Assert::IsTrue(&reaction.Actions() == &constReaction.Actions());
			}

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ReactionAttributedName)
		{
			ReactionAttributed reaction;
			const string name = "Test Name"s;
			Assert::IsTrue(reaction.Name().empty());
			reaction.SetName(name);
			Assert::AreEqual(name, reaction.Name());

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ReactionAttributedSubtype)
		{
			ReactionAttributed reaction;
			const string subtype = "Test Subtype"s;
			Assert::IsTrue(reaction.GetSubtype().empty());
			reaction.SetSubtype(subtype);
			Assert::AreEqual(subtype, reaction.GetSubtype());

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(EventMessageAttributedSubtype)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			WorldState worldState(gameTime, eventQueue);

			const string subtype = "Test Subtype"s;
			EventMessageAttributed e(subtype, worldState);
			Assert::AreEqual(subtype, e.GetSubtype());

			const string changedSubtype = "Changed Subtype"s;
			e.SetSubtype(changedSubtype);
			Assert::AreEqual(changedSubtype, e.GetSubtype());

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ReactionsAddedToActions)
		{
			ActionList action;
			ReactionAttributedFactory reactionAttributedFactory;

			// Test is Update does nothing and does not crash
			Assert::AreEqual(0_z, action.Actions().Size());
			action.CreateAction(reactionAttributedFactory.ClassName(), "First Reaction"s);
			Assert::AreEqual(1_z, action.Actions().Size());

			GameTime gameTime;
			WorldState worldState(gameTime);
			action.Update(worldState);

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ActionEvents)
		{
			EntityFactory entityFactory;
			ActionEventFactory actionEventFactory;

			Assert::AreEqual(0_z, Event<EventMessageAttributed>::Subscribers().Size());
			{
				GameTime gameTime;
				EventQueue eventQueue;
				WorldState worldState(gameTime, eventQueue);
				Entity world;
				Datum& a = world.AppendAuxiliaryAttribute("A"s) = 1;

				const string name = "Test Reaction"s;
				const string subtype = "Test"s;
				ReactionAttributed* reaction = new ReactionAttributed(name, subtype);
				Assert::AreEqual(1_z, Event<EventMessageAttributed>::Subscribers().Size());
				world.Adopt(*reaction, "Reaction"s);

				ActionIncrementFactory incrementFactory;
				Action& increment = reaction->CreateAction(incrementFactory.ClassName(), "Increment Action"s);
				ActionIncrement* incrementAction = increment.As<ActionIncrement>();
				Assert::IsNotNull(incrementAction);
				Assert::IsTrue(incrementAction->Target().empty());
				Assert::AreEqual(1, incrementAction->Step());

				Entity& entity = world.CreateChild(entityFactory.ClassName(), "Test Entity"s);

				Action& action = entity.CreateAction(actionEventFactory.ClassName(), "Test Action"s);
				ActionEvent* actionEvent = action.As<ActionEvent>();
				Assert::IsNotNull(actionEvent);
				actionEvent->SetSubtype(subtype);

				actionEvent->AppendAuxiliaryAttribute("Target"s) = "A"s;
				actionEvent->AppendAuxiliaryAttribute("Step"s) = 2;
				Assert::AreEqual(0, actionEvent->GetDelay());

				world.Update(worldState); // Enqueue the ActionEvent
				Assert::AreEqual(1_z, eventQueue.Size());
				worldState.GetGameTime().SetCurrentTime(high_resolution_clock::time_point(1s));
				world.Update(worldState); // Deliver ActionEvent

				Assert::AreEqual("A"s, incrementAction->Target());
				Assert::AreEqual(2, incrementAction->Step());
				Assert::AreEqual(3, a.GetInteger());
			}
			Assert::AreEqual(0_z, Event<EventMessageAttributed>::Subscribers().Size());

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(Clone)
		{
			{
				Assert::AreEqual(0_z, Event<EventMessageAttributed>::Subscribers().Size());
				const string name = "Test Reaction"s;
				const string subtype = "Test"s;
				ReactionAttributed reaction(name, subtype);
				Assert::AreEqual(1_z, Event<EventMessageAttributed>::Subscribers().Size());

				auto clone = reaction.Clone();
				Assert::IsNotNull(clone);
				Assert::AreEqual(2_z, Event<EventMessageAttributed>::Subscribers().Size());
				Assert::AreNotSame(reaction, *clone);
				Assert::AreEqual(reaction, *clone);

				delete clone;
			}

			{
				ActionEvent actionEvent;

				auto clone = actionEvent.Clone();
				Assert::IsNotNull(clone);
				Assert::AreNotSame(actionEvent, *clone);
				Assert::AreEqual(actionEvent, *clone);

				delete clone;
			}

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(JsonDeserialization)
		{
			ReactionAttributedFactory reactionAttributedFactory;
			ActionIncrementFactory incrementFactory;
			ActionListFactory actionListFactory;

			ReactionAttributed reaction;

			{
				// Deserialization
				JsonTableParseHelper::SharedData sharedData(reaction);
				JsonParseCoordinator parseMaster(sharedData);

				JsonTableParseHelper parseHelper;
				parseMaster.AddHelper(parseHelper);

				const std::string filename = R"(Content\TestReaction.json)";
				parseMaster.ParseFromFile(filename);
			}

			Assert::AreEqual("Test Reaction"s, reaction.Name());

			const std::string subtype = "Test"s;
			Assert::AreEqual(subtype, reaction.GetSubtype());

			auto& actionsDatum = reaction.Actions();
			Assert::AreEqual(3_z, actionsDatum.Size());
			for (size_t i = 0; i < actionsDatum.Size(); ++i)
			{
				Action* action = actionsDatum[i].As<Action>();
				Assert::IsNotNull(action);
				stringstream name;
				name << "Nested Action " << (i + 1);
				Assert::AreEqual(name.str(), action->Name());
			}

			ActionIncrement* actionIncrement = (actionsDatum[1].As<ActionIncrement>());
			Assert::IsNotNull(actionIncrement);
			Assert::AreEqual("A"s, actionIncrement->Target());

			Datum* a = reaction.Find("A"s);
			Assert::IsNotNull(a);
			Assert::AreEqual(Datum::DatumTypes::Integer, a->Type());
			Assert::AreEqual(0, a->GetInteger());

			GameTime gameTime;
			EventQueue eventQueue;
			WorldState worldState(gameTime, eventQueue);
			Entity world;
			
			EventMessageAttributed payload(subtype, worldState);
			shared_ptr<Event<EventMessageAttributed>> actionEvent = make_shared<Event<EventMessageAttributed>>(payload);

			eventQueue.Enqueue(actionEvent, gameTime);
			worldState.GetGameTime().SetCurrentTime(high_resolution_clock::time_point(1s));
			world.Update(worldState); // should trigger ReactionAttributed object deserialized from Json
			Assert::AreEqual(1, a->GetInteger());

			Bar bar(10);
			eventQueue.Enqueue(make_shared<Event<Bar>>(bar), gameTime);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(2s));
			world.Update(worldState); // should not trigger ReactionAttributed object deserialized from Json
			Assert::AreEqual(1, a->GetInteger());

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(DerivedClass)
		{
			const auto signatures = TypeManager::GetSignaturesForType(TestReaction::TypeIdClass());
			Assert::IsFalse(signatures.IsEmpty());

			TestReaction action;
			Assert::IsTrue(action.IsPrescribedAttribute("Name"s));
			Assert::IsTrue(action.IsPrescribedAttribute("Count"s));

			Scope* s = Factory<Scope>::Create(TestReaction::TypeName());
			Assert::IsNull(s);

			TestReactionFactory factory;
			s = Factory<Scope>::Create(TestReaction::TypeName());
			Assert::IsNotNull(s);

			TestReaction* a = s->As<TestReaction>();
			Assert::IsNotNull(a);

			EventQueue eventQueue;
			GameTime gameTime;
			FooSubscriber fooSubscriber;
			shared_ptr<Event<FooSubscriber>> event = make_shared<Event<FooSubscriber>>(fooSubscriber);
			eventQueue.Enqueue(event, gameTime);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			eventQueue.Update(gameTime);

			Assert::AreEqual(action.Count, 1);

			{
				TestReaction testReaction;

				auto clone = testReaction.Clone();
				Assert::IsNotNull(clone);
				Assert::AreNotSame(testReaction, *clone);
				Assert::AreEqual(testReaction, *clone);

				delete clone;
			}

			delete s;
			Event<FooSubscriber>::UnsubscribeAll();
		}

		TEST_METHOD(TestConvenienceFunctions)
		{
			EntityFactory entityFactory;
			ActionEventFactory actionEventFactory;

			Assert::AreEqual(0_z, Event<EventMessageAttributed>::Subscribers().Size());
			{
				GameTime gameTime;
				EventQueue eventQueue;
				WorldState worldState(gameTime, eventQueue);
				Entity world;
				Datum& a = world.AppendAuxiliaryAttribute("A"s) = 1;

				const string name = "Test Reaction"s;
				const string subtype = "Test"s;
				ReactionAttributed* reaction = new ReactionAttributed(name, subtype);
				Assert::AreEqual(1_z, Event<EventMessageAttributed>::Subscribers().Size());
				world.Adopt(*reaction, "Reaction"s);

				ActionIncrementFactory incrementFactory;
				Action& increment = reaction->CreateAction(incrementFactory.ClassName(), "Increment Action"s);
				ActionIncrement* incrementAction = increment.As<ActionIncrement>();
				Assert::IsNotNull(incrementAction);
				Assert::IsTrue(incrementAction->Target().empty());
				Assert::AreEqual(1, incrementAction->Step());

				Entity& entity = world.CreateChild(entityFactory.ClassName(), "Test Entity"s);

				Action& action = entity.CreateAction(actionEventFactory.ClassName(), "Test Action"s);
				ActionEvent* actionEvent = action.As<ActionEvent>();
				Assert::IsNotNull(actionEvent);
				actionEvent->SetSubtype(subtype);
				Assert::AreEqual(actionEvent->GetSubtype(), subtype);
				actionEvent->SetDelay(0);

				actionEvent->AppendAuxiliaryAttribute("Target"s) = "A"s;
				actionEvent->AppendAuxiliaryAttribute("Step"s) = 2;
				Assert::AreEqual(0, actionEvent->GetDelay());

				world.Update(worldState); // Enqueue the ActionEvent
				Assert::AreEqual(1_z, eventQueue.Size());
				worldState.GetGameTime().SetCurrentTime(high_resolution_clock::time_point(1s));
				world.Update(worldState); // Deliver ActionEvent

				Assert::AreEqual("A"s, incrementAction->Target());
				Assert::AreEqual(2, incrementAction->Step());
				Assert::AreEqual(3, a.GetInteger());

				{
					EventMessageAttributed payload(subtype, worldState);
					shared_ptr<Event<EventMessageAttributed>> actionEvent2 = make_shared<Event<EventMessageAttributed>>(payload);
					auto eventMessagedAttributed = actionEvent2->Message();
					eventMessagedAttributed.SetWorldState(worldState);
				}
			}
			Assert::AreEqual(0_z, Event<EventMessageAttributed>::Subscribers().Size());

			Event<EventMessageAttributed>::UnsubscribeAll();
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ReactionTests::sStartMemState;
}