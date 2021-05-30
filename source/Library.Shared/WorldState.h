#pragma once
#include "GameTime.h"
#include "Vector.h"
#include "Action.h"
#include "EventQueue.h"

namespace FIEAGameEngine
{
	class Entity;
	class Action;
	/// <summary>
	/// Class that manages the GameTime and the current Entity that is being processed.
	/// </summary>
	class WorldState final
	{
	public:
		/// <summary>
		/// Creates a new instance of WorldState. Uses default constructor.
		/// </summary>
		WorldState() = default;
		/// <summary>
		/// Creates a new instance of WorldState. Sets the game time of this world state.
		/// </summary>
		explicit WorldState(GameTime gameTime);
		/// <summary>
		/// Creates a new instance of WorldState. Sets the game time and the EventQueue of this world state.
		/// </summary>
		WorldState(GameTime gameTime, EventQueue& eventQueue);
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new WorldState and deep copies each member variable from the other WorldState into this WorldState. 
		/// </summary>
		/// <param name="other">The instance of WorldState that is being copied from.</param>
		WorldState(const WorldState&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new WorldState and "steals" the member variables from other WorldState into this WorldState. 
		/// Shallow copies the member variables and sets the other WorldState's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of WorldState that is being moved from.</param>
		WorldState(WorldState&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this WorldState and deep copies each element from other WorldState into this WorldState.
		/// </summary>
		/// <param name="other">The instance of WorldState that is being copied from.</param>
		/// <returns>A WorldState reference to this object.</returns>
		WorldState& operator=(const WorldState&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other WorldState into this WorldState. 
		/// Clears the current WorldState and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other WorldState's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of WorldState that is being moved from.</param>
		/// <returns>A WorldState reference to the this object.</returns>
		WorldState& operator=(WorldState&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this WorldState.
		/// </summary>
		~WorldState() = default;
		/// <summary>
		/// Gets the game time object associated with this WorldState.
		/// </summary>
		/// <returns>The game time of this WorldState.</returns>
		GameTime& GetGameTime();
		/// <summary>
		/// Sets the game time object associated with this WorldState.
		/// </summary>
		/// <param name="gameTime">The game time that you want to be associated with this WorldState.</param>
		void SetGameTime(GameTime& gameTime);

		/// <summary>
		/// The game time object associated with this WorldState.
		/// </summary>
		GameTime _gameTime;
		/// <summary>
		/// The Event Queue associated with this WorldState.
		/// </summary>
		EventQueue* _eventQueue = nullptr;
		/// <summary>
		/// A pointer to the current entity that is being processed in its own Update method.
		/// </summary>
		Entity* _currentEntity = nullptr;
		/// <summary>
		/// A pointer to the current action that is being processed in its own Update method.
		/// </summary>
		Action* _currentAction = nullptr;

		/// <summary>
		/// Struct used to hold all the information required to create a new action.
		/// </summary>
		struct CreateAction
		{
			/// <summary>
			/// Constructor used to create and populate a CreateAction struct.
			/// </summary>
			/// <param name="actionToCreateName">The name that will be given to the newly created action.</param>
			/// <param name="className">The name of the class that the newly created action will be a type of.</param>
			/// <param name="targetName">The name of the entity/action that will be searched for and acted upon inside the action. 
			///	This is required for each action that is created.
			/// </param>
			/// <param name="context">The parent scope of the action that called this constructor.</param>
			CreateAction(string& actionToCreateName, string& className, string& targetName, Scope& context) :
				_actionToCreateName(actionToCreateName), _className(className), _targetName(targetName), _context(context)
			{

			}

			/// <summary>
			/// The name that will be given to the newly created action.
			/// </summary>
			string& _actionToCreateName;
			/// <summary>
			/// The name of the class that the newly created action will be a type of.
			/// </summary>
			string& _className;
			/// <summary>
			/// The name of the entity/action that will be searched for and acted upon inside the action. 
			///	This is required for each action that is created.
			/// </summary>
			string& _targetName;
			/// <summary>
			/// The parent scope of the action that called this constructor.
			/// </summary>
			Scope& _context;
		};

		/// <summary>
		/// Struct used to hold all the information required to delete an action.
		/// </summary>
		struct DestroyAction
		{
			/// <summary>
			/// Constructor used to create and populate a DeleteAction struct.
			/// </summary>
			/// <param name="actionToDeleteName">The name of the action that will be deleted. This must already exist.</param>
			/// <param name="context">The parent scope of the action that called this constructor.</param>
			DestroyAction(string& actionToDeleteName, Scope& context) :
				_actionToDeleteName(actionToDeleteName), _context(context)
			{

			}

			/// <summary>
			/// The name of the action that will be deleted. This must already exist.
			/// </summary>
			string& _actionToDeleteName;
			/// <summary>
			/// The parent scope of the action that called this constructor.
			/// </summary>
			Scope& _context;
		};

		/// <summary>
		/// Calls the corresponding functions to process both the create and destroy queues.
		/// </summary>
		void ProcessQueues();
		/// <summary>
		/// Processes each of the CreateAction structs in the createQueue at this moment. Clears the queue afterwards.
		/// </summary>
		void ProcessCreateQueue();
		/// <summary>
		/// Processes each of the DeleteAction structs in the deleteQueue at this moment. Clears the queue afterwards.
		/// </summary>
		void ProcessDestroyQueue();
		/// <summary>
		/// Pushes a CreateAction struct into the queue.
		/// </summary>
		/// <param name="action">The CreateAction struct that holds info on how the create this action.</param>
		void AddCreateAction(CreateAction action);
		/// <summary>
		/// Pushes a DeleteAction struct into the queue.
		/// </summary>
		/// <param name="action">The DeleteAction struct that holds info on how the delete this action.</param>
		void AddDestroyAction(DestroyAction action);

	private:
		/// <summary>
		/// Queue to hold all the pending CreateAction calls (through the info stored in a CreateAction struct).
		/// </summary>
		Vector<CreateAction> createQueue;
		/// <summary>
		/// Queue to hold all the pending DeleteAction calls (through the info stored in a DeleteAction struct).
		/// </summary>
		Vector<DestroyAction> destroyQueue;
	};
}