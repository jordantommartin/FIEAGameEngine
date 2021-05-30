#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "ToStringSpecialization.h"
#include "TypeManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace glm;
using namespace std::string_literals;
using namespace FIEAGameEngine;

namespace UnitTestLibraryDesktop
{
	class Monster : public RTTI
	{
		RTTI_DECLARATIONS(Monster, RTTI);

	public:
		std::string Name;
		int HitPoints;
		float Dps;

		static const FIEAGameEngine::Vector<FIEAGameEngine::Signature> Signatures()
		{
			return Vector<Signature>
			{
				{"Name"s, Datum::DatumTypes::String, 1, offsetof(Monster, Name) },
				{ "HitPoints"s, Datum::DatumTypes::Integer, 1, offsetof(Monster, HitPoints) },
				{ "Dps"s, Datum::DatumTypes::Float, 1, offsetof(Monster, Name) }
			};
		}
	};

	RTTI_DEFINITIONS(Monster)

	class MeanMonster : public Monster
	{
		RTTI_DECLARATIONS(MeanMonster, Monster);

	public:
		int Grumpiness;

		static const FIEAGameEngine::Vector<FIEAGameEngine::Signature> Signatures()
		{
			return Vector<Signature>
			{
				{ "Name"s, Datum::DatumTypes::String, 1, offsetof(Monster, Name) },
				{ "HitPoints"s, Datum::DatumTypes::Integer, 1, offsetof(Monster, HitPoints) },
				{ "Dps"s, Datum::DatumTypes::Float, 1, offsetof(Monster, Name) },
				{ "Grumpiness"s, Datum::DatumTypes::Integer, 1, offsetof(MeanMonster, Grumpiness) }
			};
		}
	};

	RTTI_DEFINITIONS(MeanMonster)

	TEST_CLASS(TypeManagerTests)
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

		TEST_METHOD(NoInheritance)
		{
			const size_t preRegisteredTypeCount = TypeManager::Types().Size();
			{
				Assert::IsFalse(TypeManager::ContainsType(Monster::TypeIdClass()));
				TypeManager::AddType(Monster::TypeIdClass(), Monster::Signatures());
				Assert::AreEqual(preRegisteredTypeCount + 1, TypeManager::Types().Size());
				Assert::IsTrue(TypeManager::ContainsType(Monster::TypeIdClass()));
				Assert::ExpectException<exception>([] { TypeManager::AddType(Monster::TypeIdClass(), Monster::Signatures()); }, L"Expected an exception, but none was thrown");

				auto monsterSignatures = TypeManager::GetSignaturesForType(Monster::TypeIdClass());
				Assert::AreEqual(3_z, monsterSignatures.Size());

				TypeManager::RemoveType(Monster::TypeIdClass());
				Assert::AreEqual(preRegisteredTypeCount, TypeManager::Types().Size());

				Assert::ExpectException<exception>([] { TypeManager::GetSignaturesForType(Monster::TypeIdClass()); }, L"Expected an exception, but none was thrown");
			}

			{
				Vector<Signature> signatures
				{
					{"Name"s, Datum::DatumTypes::String, 1, offsetof(Monster, Name) },
					{"HitPoints"s, Datum::DatumTypes::Integer, 1, offsetof(Monster, HitPoints) },
					{"Name"s, Datum::DatumTypes::Float, 1, offsetof(Monster, Dps) },
				};

				Assert::IsFalse(TypeManager::ContainsType(Monster::TypeIdClass()));
				TypeManager::AddType(Monster::TypeIdClass(), std::move(signatures));
				Assert::AreEqual(preRegisteredTypeCount + 1, TypeManager::Types().Size());
				Assert::IsTrue(TypeManager::ContainsType(Monster::TypeIdClass()));

				const auto monsterSignatures = TypeManager::GetSignaturesForType(Monster::TypeIdClass());
				Assert::AreEqual(3_z, monsterSignatures.Size());

				TypeManager::RemoveType(Monster::TypeIdClass());
				Assert::AreEqual(preRegisteredTypeCount, TypeManager::Types().Size());
			}
		}

		TEST_METHOD(Inheritance)
		{
			const size_t preRegisteredTypeCount = TypeManager::Types().Size();

			{
				Assert::IsFalse(TypeManager::ContainsType(Monster::TypeIdClass()));
				TypeManager::AddType(Monster::TypeIdClass(), Monster::Signatures());
				Assert::AreEqual(preRegisteredTypeCount + 1, TypeManager::Types().Size());
				Assert::IsTrue(TypeManager::ContainsType(Monster::TypeIdClass()));

				Assert::IsFalse(TypeManager::ContainsType(MeanMonster::TypeIdClass()));
				TypeManager::AddType(MeanMonster::TypeIdClass(), MeanMonster::Signatures());
				Assert::AreEqual(preRegisteredTypeCount + 2, TypeManager::Types().Size());
				Assert::IsTrue(TypeManager::ContainsType(MeanMonster::TypeIdClass()));

				auto monsterSignatures = TypeManager::GetSignaturesForType(MeanMonster::TypeIdClass());
				Assert::AreEqual(4_z, monsterSignatures.Size());

				TypeManager::RemoveType(MeanMonster::TypeIdClass());
				TypeManager::RemoveType(Monster::TypeIdClass());
				Assert::AreEqual(preRegisteredTypeCount, TypeManager::Types().Size());

				Assert::ExpectException<exception>([&monsterSignatures] {TypeManager::GetSignaturesForType(MeanMonster::TypeIdClass()); }, L"Expected an exception but none was thrown");
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState TypeManagerTests::sStartMemState;
}