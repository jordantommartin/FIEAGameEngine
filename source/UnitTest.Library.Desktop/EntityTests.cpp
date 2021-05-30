#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "Attributed.h"
#include "Entity.h"
#include "Avatar.h"
#include "GameTime.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "WorldState.h"
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	wstring ToString<Avatar>(const Avatar& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	wstring ToString<Avatar>(Avatar* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	wstring ToString<Entity>(const Entity& t)
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

	template<>
	std::wstring ToString<Datum::DatumTypes>(const Datum::DatumTypes& t)
	{
		RETURN_WIDE_STRING(to_wstring(static_cast<int>(t)));
	}

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EntityTests)
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

		TEST_METHOD(EntityJsonDeserialization)
		{
			Entity entity;

			{
				JsonTableParseHelper::SharedData sharedData(entity);
				JsonParseCoordinator parseCoordinator(sharedData);

				JsonTableParseHelper parseHelper;
				parseCoordinator.AddHelper(parseHelper);

				const string filename = R"(Content\TestEntity.json)";
				parseCoordinator.ParseFromFile(filename);
			}

			Assert::AreEqual("Test Entity"s, entity.Name());

			Datum* a = entity.Find("A"s);
			Assert::IsNotNull(a);
			Assert::AreEqual(Datum::DatumTypes::Integer, a->Type());
			Assert::AreEqual(0, a->GetInteger());
		}

		TEST_METHOD(WorldJsonDeserialization)
		{
			GameTime gameTime;
			FIEAGameEngine::WorldState worldState;
			Entity world;

			{
				ScopeFactory scopeFactory;
				EntityFactory entityFactory;
				AvatarFactory avatarFactory;

				JsonTableParseHelper::SharedData sharedData(world);
				JsonParseCoordinator parseCoordinator(sharedData);

				JsonTableParseHelper parseHelper;
				parseCoordinator.AddHelper(parseHelper);

				const string filename = R"(Content\TestWorld.json)";
				parseCoordinator.ParseFromFile(filename);
			}

			Assert::AreEqual("Test World"s, world.Name());

			auto& sectorsDatum = world.GetChildren();
			Assert::AreEqual(3_z, sectorsDatum.Size());
			for (size_t i = 0; i < sectorsDatum.Size(); ++i)
			{
				Assert::IsTrue(sectorsDatum[i].Is("Entity"));
				Entity* sector = sectorsDatum[i].As<Entity>();
				Assert::IsNotNull(sector);
				stringstream name;
				name << "Sector " << (i + 1);
				Assert::AreEqual(name.str(), sector->Name());
				Assert::AreEqual(4_z, sector->Size());
			}

			auto& sector2Children = sectorsDatum[1].As<Entity>()->GetChildren();
			Assert::AreEqual(sector2Children.Size(), 2_z);
			Avatar* avatar = sector2Children[0].As<Avatar>();
			Assert::AreEqual("Crazy Cool Avatar Man"s, avatar->Name());
			Assert::AreEqual(250, avatar->HitPoints);

			Assert::AreEqual(sector2Children.Size(), 2_z);
			Entity* entity = sector2Children[1].As<Entity>();
			Assert::AreEqual("Special Entity #2"s, entity->Name());

			world.Update(worldState);

			// const test
			const Entity constEntity;
			const auto& constChildren = constEntity.GetChildren();
			Assert::AreEqual(constChildren.Size(), 0_z);
		}

		TEST_METHOD(CreateChildren)
		{
			GameTime gameTime;
			FIEAGameEngine::WorldState worldState(gameTime);
			Entity world;

			ScopeFactory scopeFactory;
			EntityFactory entityFactory;
			AvatarFactory avatarFactory;

			{
				JsonTableParseHelper::SharedData sharedData(world);
				JsonParseCoordinator parseCoordinator(sharedData);

				JsonTableParseHelper parseHelper;
				parseCoordinator.AddHelper(parseHelper);

				const string filename = R"(Content\TestWorld.json)";
				parseCoordinator.ParseFromFile(filename);
			}

			Entity& firstSector = *world.GetChildren()[0].As<Entity>();
			string avatarString = "Avatar"s;
			string instanceString = "MegaMan"s;
			firstSector.CreateChild(avatarString, instanceString);
			Avatar megaMan = *firstSector.GetChildren()[0].As<Avatar>();
			Assert::AreEqual("MegaMan"s, megaMan.Name());
			Assert::AreEqual(100, megaMan.HitPoints);
		}

		TEST_METHOD(WorldStateTests)
		{
			GameTime gameTime1, gameTime2;
			WorldState worldState(gameTime1);
			Entity world;
			

			GameTime newGameTime = worldState.GetGameTime();
			worldState.SetGameTime(gameTime2);
		}

		TEST_METHOD(Clone)
		{
			{
				Entity ent;
				auto clone = ent.Clone();
				auto anotherEntity = clone->As<Entity>();
				Assert::IsNotNull(anotherEntity);

				Assert::IsTrue(anotherEntity->IsPrescribedAttribute("this"));
				Assert::IsTrue(anotherEntity->IsPrescribedAttribute("Name"));
				Assert::IsTrue(anotherEntity->IsPrescribedAttribute("Children"));

				Assert::AreEqual(anotherEntity, (*anotherEntity)["this"].GetPointer()->As<Entity>());

				// Test that external storage has been updated 
				string& entData = ent.At("Name").GetString();
				string& anotherentData = (*anotherEntity).At("Name"s).GetString();
				Assert::AreNotSame(anotherentData, entData);
				Assert::AreEqual(ent, *anotherEntity);

				delete clone;
			}


			{
				Avatar ava;
				auto clone = ava.Clone();
				auto anotherAvatar = clone->As<Avatar>();
				Assert::IsNotNull(anotherAvatar);

				Assert::IsTrue(anotherAvatar->IsPrescribedAttribute("this"));
				Assert::IsTrue(anotherAvatar->IsPrescribedAttribute("Name"));
				Assert::IsTrue(anotherAvatar->IsPrescribedAttribute("Children"));

				Assert::AreEqual(anotherAvatar, (*anotherAvatar)["this"].GetPointer()->As<Avatar>());

				// Test that external storage has been updated 
				string& avaData = ava.At("Name").GetString();
				string& anotheravaData = (*anotherAvatar).At("Name"s).GetString();
				Assert::AreNotSame(anotheravaData, avaData);
				Assert::AreEqual(ava, *anotherAvatar);

				delete clone;
			}

		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EntityTests::sStartMemState;
}