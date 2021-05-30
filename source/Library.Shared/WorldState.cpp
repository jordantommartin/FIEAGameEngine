#include "pch.h"
#include "WorldState.h"

namespace FIEAGameEngine
{
#pragma region WorldState

	WorldState::WorldState(GameTime gameTime) :
		_gameTime(gameTime)
	{

	}

	WorldState::WorldState(GameTime gameTime, EventQueue& eventQueue) :
		_gameTime(gameTime), _eventQueue(&eventQueue)
	{
	}

	GameTime& WorldState::GetGameTime()
	{
		return _gameTime;
	}

	void WorldState::SetGameTime(GameTime& gameTime)
	{
		_gameTime = gameTime;
	}
	void WorldState::ProcessQueues()
	{
		ProcessCreateQueue();
		ProcessDestroyQueue();
	}

	void WorldState::ProcessCreateQueue()
	{
		for (CreateAction createAction : createQueue)
		{
			Scope* scope = Factory<Scope>::Create(createAction._className);
			assert(scope != nullptr);
			assert(scope->Is(Action::TypeIdClass()));

			Action& action = static_cast<Action&>(*scope);
			action.SetName(createAction._actionToCreateName);
			action.SetTarget(createAction._targetName);
			createAction._context.Adopt(action, "Actions");
		}
		createQueue.Clear();
	}

	void WorldState::ProcessDestroyQueue()
	{
		for (DestroyAction destroyAction : destroyQueue)
		{
			Datum* actionsDatum = destroyAction._context.Find("Actions");
			assert(actionsDatum != nullptr);

			for (size_t i = 0_z; i < actionsDatum->Size(); ++i)
			{
				Scope* scope = &actionsDatum->GetScope(i);
				assert(scope->Is(Action::TypeIdClass()));
				Action* action = static_cast<Action*>(scope);

				if (action->Name() == destroyAction._actionToDeleteName)
				{
					actionsDatum->RemoveAt(i);
					delete action;
					break;
				}
			}
		}
		destroyQueue.Clear();
	}

	void WorldState::AddCreateAction(CreateAction action)
	{
		createQueue.PushBack(action);
	}
	void WorldState::AddDestroyAction(DestroyAction action)
	{
		for (auto it = destroyQueue.begin(); it != destroyQueue.end(); ++it)
		{
			DestroyAction destroyAction = *it;
			if (destroyAction._context.IsDescendantOf(action._context))
			{
				destroyQueue.Remove(it);
			}
		}
		
		destroyQueue.PushBack(action);
	}
#pragma endregion
}