#pragma once
#include "RTTI.h"
#include "Action.h"
#include "TypeManager.h"
#include "WorldState.h"
#include "Stack.h"
#include "Vector.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// An action that can be used to find a Infix Expression, evaluate it, and assign it to a target.
	/// </summary>
	class ActionExpression final : public Action
	{
	public:
		RTTI_DECLARATIONS(ActionExpression, Action)

	public:
		/// <summary>
		/// Creates a new instance of ActionExpression. 
		/// </summary>
		ActionExpression();
		/// <summary>
		/// Invokes the copy constructor. 
		/// Creates a new ActionExpression and deep copies each member variable from the other ActionExpression into this ActionExpression. 
		/// </summary>
		/// <param name="other">The instance of ActionExpression that is being copied from.</param>
		ActionExpression(const ActionExpression&) = default;
		/// <summary>
		/// Invokes the move constructor. Creates a new ActionExpression and "steals" the member variables from other ActionExpression into this ActionExpression. 
		/// Shallow copies the member variables and sets the other ActionExpression's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionExpression that is being moved from.</param>
		ActionExpression(ActionExpression&&) = default;
		/// <summary>
		/// Invokes the copy assignment. Destructs all elements in this ActionExpression and deep copies each element from other ActionExpression into this ActionExpression.
		/// </summary>
		/// <param name="other">The instance of ActionExpression that is being copied from.</param>
		/// <returns>A ActionExpression reference to this object.</returns>
		ActionExpression& operator=(const ActionExpression&) = default;
		/// <summary>
		/// Invokes the move assignment. "Steals" the member variables from other ActionExpression into this ActionExpression. 
		/// Clears the current ActionExpression and frees the memory asscoaited with it. 
		/// Then, shallow copies the member variables (including the data) and sets the other ActionExpression's member variables to their uninitialized defaults.
		/// </summary>
		/// <param name="other">The instance of ActionExpression that is being moved from.</param>
		/// <returns>A ActionExpression reference to the this object.</returns>
		ActionExpression& operator=(ActionExpression&&) = default;
		/// <summary>
		/// Destructor that clears the memory associated with this ActionList.
		/// </summary>
		~ActionExpression() = default;

		/// <summary>
		/// Returns the name of the target to find.
		/// </summary>
		/// <returns>The name of the target to find.</returns>
		const std::string& Target() const;
		/// <summary>
		/// Sets the name of the target to find.
		/// </summary>
		/// <param name="target">The new name of the target to find.</param>
		void SetTarget(const std::string& target);

		/// <summary>
		/// Returns the name of the InfixExpression to find.
		/// </summary>
		/// <returns>The name of the target to find.</returns>
		const std::string& InfixExpressionName() const;
		/// <summary>
		/// Sets the name of the InfixExpression to find. 
		/// </summary>
		/// <param name="target">The new name of the InfixExpression to find.</param>
		void SetInfixExpressionName(const std::string& infixExpression);

		/// <summary>
		/// An override on the clone function which provides a heap allocated object of an ActionExpression.
		/// </summary>
		/// <returns>An ActionExpression pointer to the heap allocated memory.</returns>
		gsl::owner<ActionExpression*> Clone() const override;

		/// <summary>
		/// Finds the infix expression, Converts and evaluates the expression. Assigns this to the target.
		/// </summary>
		/// <param name="worldState">The WorldState that is managed during each update call.</param>
		void Update(WorldState& worldState) override;

		/// <summary>
		/// Takes in a string representing an infix expression and converts it to a postfix expression using the Shunting Yard Algorithm.
		/// </summary>
		/// <param name="infixExpression">String representing the infix expression</param>
		/// <returns></returns>
		string ConvertInfixToPostfix(const string& infixExpression);

		/// <summary>
		/// Takes in a string representing a postfix expression and evaluates it to an integer.
		/// </summary>
		/// <param name="postfixExpression">The string representing the postfix expression</param>
		/// <returns>and integer representing the final value</returns>
		int Evaluate(const string& postfixExpression);

		/// <summary>
		/// Checks to see if the character is an operator (that we support)
		/// </summary>
		/// <param name="character">The char being checked</param>
		/// <returns>Whether the character was an operator</returns>
		bool IsOperator(char character);

		/// <summary>
		/// Provides the Signatures Vector for an ActionExpression with all the prescribed Attributes.
		/// </summary>
		/// <returns>The Signatures Vector for an ActionExpression.</returns>
		static const Vector<Signature> Signatures();

	private:
		/// <summary>
		/// The name of the attributed where the infix expression will be found
		/// </summary>
		std::string _infixExpressionName = "";
		/// <summary>
		/// String that will be used to get the infix expression name.
		/// </summary>
		inline static std::string InfixExpressionNameString = "InfixExpressionName"s;

		/// <summary>
		/// Mapping of precendences to operator characters
		/// </summary>
		HashMap<char, size_t> _precendenceMap = { 
			std::make_pair('(', 3), 
			std::make_pair(')', 3),
			std::make_pair('^', 2), 
			std::make_pair('*', 1), 
			std::make_pair('/', 1), 
			std::make_pair('+', 0), 
			std::make_pair('-', 0)
		};
	};

	ConcreteFactory(ActionExpression, Scope)
}