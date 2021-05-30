#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <chrono>
#include "Event.h"
#include "EventSubscribers.h"
#include "EventQueue.h"
#include "Entity.h"
#include "Bar.h"

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
	wstring ToString<Event<Foo>>(Event<Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTests)
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
			Foo foo;
			Event<Foo> event(foo);

			RTTI* rtti = &event;

			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("Event"s));
			Assert::IsTrue(rtti->Is("EventPublisher"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(rtti->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Event<Foo>::TypeIdClass(), rtti->TypeIdInstance());

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			Event<Foo>* e = rtti->As<Event<Foo>>();
			Assert::IsNotNull(e);
			Assert::AreEqual(&event, e);

			EventPublisher* ep = rtti->As<EventPublisher>();
			Assert::IsNotNull(ep);
			Assert::AreEqual(&event, static_cast<Event<Foo>*>(ep));
			Assert::IsTrue(rtti->Is(ep->TypeIdInstance()));

			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(Event<Foo>::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsNotNull(r);

			Assert::AreEqual("Event"s, r->ToString());

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(FooEvent)
		{
			GameTime gameTime;
			Foo foo;
			FooSubscriber fooSubscriber;
			EventQueue eventQueue;

			shared_ptr<Event<Foo>> event = make_shared<Event<Foo>>(foo);
			Assert::AreEqual(event->Message(), foo);
			event->Deliver();
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueEnqueue)
		{
			GameTime gameTime;
			Foo foo;
			FooSubscriber fooSubscriber;
			EventQueue eventQueue;

			shared_ptr<Event<Foo>> event = make_shared<Event<Foo>>(foo);
			Assert::AreEqual(event->Message(), foo);

			Assert::AreEqual(0_z, eventQueue.Size());
			eventQueue.Enqueue(event, gameTime);
			Assert::AreEqual(1_z, eventQueue.Size());

			Event<Foo>::Subscribe(fooSubscriber);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			Assert::IsFalse(fooSubscriber.WasNotified);
			eventQueue.Update(gameTime);
			Assert::IsTrue(fooSubscriber.WasNotified);

			// Message removed from queue after delivery
			fooSubscriber.WasNotified = false;
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			Assert::AreEqual(0_z, eventQueue.Size());

			// Test delay
			gameTime.SetCurrentTime(high_resolution_clock::time_point());
			eventQueue.Enqueue(event, gameTime, 1s);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(500ms));
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(2s));
			eventQueue.Update(gameTime);
			Assert::IsTrue(fooSubscriber.WasNotified);

			Assert::IsTrue(eventQueue.IsEmpty());
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueEnqueueManyEvents)
		{
			GameTime gameTime;
			Foo foo;
			FooSubscriber fooSubscriber;
			EventQueue eventQueue;

			shared_ptr<Event<Foo>> event = make_shared<Event<Foo>>(foo);
			Assert::AreEqual(event->Message(), foo);
			Assert::AreEqual(0_z, eventQueue.Size());
			Event<Foo>::Subscribe(fooSubscriber);

			// Test delay
			gameTime.SetCurrentTime(high_resolution_clock::time_point());
			for (size_t i = 0; i < 1000; ++i)
			{
				eventQueue.Enqueue(event, gameTime, 1s);
			}
			gameTime.SetCurrentTime(high_resolution_clock::time_point(500ms));
			eventQueue.Update(gameTime);
			Assert::IsFalse(eventQueue.IsEmpty());
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			eventQueue.Update(gameTime);
			Assert::IsFalse(eventQueue.IsEmpty());
			gameTime.SetCurrentTime(high_resolution_clock::time_point(2s));
			eventQueue.Update(gameTime);
			Assert::IsTrue(eventQueue.IsEmpty());

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueCheckValueChanged)
		{
			GameTime gameTime;
			Foo foo(50);
			FooSubscriber fooSubscriber(10);
			EventQueue eventQueue;

			shared_ptr<Event<Foo>> event = make_shared<Event<Foo>>(foo);
			Assert::AreEqual(event->Message(), foo);

			Assert::AreEqual(0_z, eventQueue.Size());
			eventQueue.Enqueue(event, gameTime);
			Assert::AreEqual(1_z, eventQueue.Size());

			Event<Foo>::Subscribe(fooSubscriber);
			Assert::AreEqual(fooSubscriber._data, 10);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			Assert::AreNotEqual(fooSubscriber._data, 50);
			eventQueue.Update(gameTime);
			Assert::AreEqual(fooSubscriber._data, 50);

			// Message removed from queue after delivery
			fooSubscriber._data = 10;
			fooSubscriber.WasNotified = false;
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			Assert::AreEqual(0_z, eventQueue.Size());

			// Test delay
			gameTime.SetCurrentTime(high_resolution_clock::time_point());
			eventQueue.Enqueue(event, gameTime, 1s);
			Assert::AreEqual(fooSubscriber._data, 10);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(500ms));
			eventQueue.Update(gameTime);
			Assert::AreNotEqual(fooSubscriber._data, 50);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			eventQueue.Update(gameTime);
			Assert::AreNotEqual(fooSubscriber._data, 50);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(2s));
			eventQueue.Update(gameTime);
			Assert::AreEqual(fooSubscriber._data, 50);

			Assert::IsTrue(eventQueue.IsEmpty());
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueMultipleSubscribes)
		{
			GameTime gameTime;
			Foo foo;
			FooSubscriber fooSubscriber;
			EventQueue eventQueue;

			shared_ptr<Event<Foo>> event = make_shared<Event<Foo>>(foo);
			Assert::AreEqual(event->Message(), foo);

			Assert::AreEqual(0_z, eventQueue.Size());
			eventQueue.Enqueue(event, gameTime);
			Assert::AreEqual(1_z, eventQueue.Size());

			Event<Foo>::Subscribe(fooSubscriber);
			Event<Foo>::Subscribe(fooSubscriber);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			Assert::IsFalse(fooSubscriber.WasNotified);
			eventQueue.Update(gameTime);
			Assert::IsTrue(fooSubscriber.WasNotified);

			// Message removed from queue after delivery
			fooSubscriber.WasNotified = false;
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			Assert::AreEqual(0_z, eventQueue.Size());

			Event<Foo>::Unsubscribe(fooSubscriber);
			Event<Foo>::Unsubscribe(fooSubscriber);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueAdditionalFunctions)
		{
			GameTime gameTime;
			Foo foo;
			FooSubscriber fooSubscriber;
			EventQueue eventQueue;

			shared_ptr<Event<Foo>> event = make_shared<Event<Foo>>(foo);

			// Enqueue and clear
			Assert::AreEqual(0_z, eventQueue.Size());
			for (size_t i = 0; i < 100; ++i)
			{
				eventQueue.Enqueue(event, gameTime);
			}
			Assert::AreEqual(100_z, eventQueue.Size());

			eventQueue.Clear();
			Assert::AreEqual(0_z, eventQueue.Size());



			Event<Foo>::UnsubscribeAll();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventTests::sStartMemState;
}