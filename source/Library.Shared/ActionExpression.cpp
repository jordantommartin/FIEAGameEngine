#include "pch.h"
#include "ActionExpression.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionExpression)

	ActionExpression::ActionExpression() :
		Action(ActionExpression::TypeIdClass())
	{

	}

	const std::string& ActionExpression::Target() const
	{
		return _target;
	}
	void ActionExpression::SetTarget(const std::string& target)
	{
		_target = target;
	}

	const std::string& ActionExpression::InfixExpressionName() const
	{
		return _infixExpressionName;
	}
	void ActionExpression::SetInfixExpressionName(const std::string& infixExpression)
	{
		_infixExpressionName = infixExpression;
	}

	gsl::owner<ActionExpression*> ActionExpression::Clone() const
	{
		return new ActionExpression(*this);
	}
	void ActionExpression::Update(WorldState& worldState)
	{
		UNREFERENCED_LOCAL(worldState);

		worldState._currentAction = this;

		Datum* foundInfixExpression = Search(_infixExpressionName);
		if (foundInfixExpression == nullptr)
		{
			throw std::runtime_error("Target was not found.");
		}

		Datum* foundTarget = Search(_target);
		if (foundTarget == nullptr)
		{
			throw std::runtime_error("Target was not found.");
		}

		foundTarget->Set(Evaluate(ConvertInfixToPostfix(foundInfixExpression->GetString())));
	}


	string ActionExpression::ConvertInfixToPostfix(const string& infixExpression)
	{
		Stack<char> stack;
		Vector<char> queue;
		std::string postfixExpression;

		for (size_t i = 0; i < infixExpression.size(); ++i)
		{
			char character = infixExpression[i];

			if (character == ')')
			{
				if (stack.IsEmpty())
				{
					throw::runtime_error("Righthand paran was found, but no lefthand paren to accompany.");
				}

				while (stack.Top() != '(')
				{
					queue.PushBack(stack.Top());
					stack.Pop();

					if (stack.IsEmpty())
					{
						throw::runtime_error("Righthand paran was found, but no lefthand paren to accompany.");
					}
				}
				stack.Pop();
			}
			else if (IsOperator(character))
			{
				if (stack.IsEmpty() || (_precendenceMap.At(character) > _precendenceMap.At(stack.Top())) || character == '(' || stack.Top() == '(')
				{
					stack.Push(character);
				}
				else
				{
					while (!stack.IsEmpty() && stack.Top() != '(' && _precendenceMap.At(stack.Top()) >= _precendenceMap.At(character))
					{
						queue.PushBack(stack.Top());
						stack.Pop();
					}
					stack.Push(character);
				}
			}
			else
			{
				queue.PushBack(character);
			}
		}

		while(!stack.IsEmpty())
		{
			queue.PushBack(stack.Top());
			stack.Pop();
		}

		for (size_t i = 0; i < queue.Size(); ++i)
		{
			postfixExpression += queue.At(i);
		}

		return postfixExpression;
	}

	int ActionExpression::Evaluate(const string& postfixExpression)
	{
		Stack<int> stack;

		for (size_t i = 0; i < postfixExpression.size(); ++i)
		{
			char character = postfixExpression[i];
			if (isdigit(character))
			{
				stack.Push(character - '0');
			}
			else if (IsOperator(character))
			{
				int second = stack.Top();
				stack.Pop();
				int first = stack.Top();
				stack.Pop();

				switch (character)
				{
				case '+':
					stack.Push(first + second);
					break;
				case '-':
					stack.Push(first - second);
					break;
				case '*':
					stack.Push(first * second);
					break;
				case '/':
					stack.Push(first / second);
					break;
				case '^':
					stack.Push(static_cast<int>(pow(first, second)));
					break;
				case '%':
					stack.Push(first % second);
					break;
				default:
					break;
				}
			}
		}

		return stack.Top();
	}

	bool ActionExpression::IsOperator(char character)
	{
		return (character == '+' || character == '-' || character == '*' || character == '/' || character == '^' || character == '(' || character == '%');
	}

	const Vector<Signature> ActionExpression::Signatures()
	{
		return Vector< FIEAGameEngine::Signature>
		{
			{ NameStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionExpression, _name) },
			{ TargetStringLiteral, Datum::DatumTypes::String, 1, offsetof(ActionExpression, _target) },
			{ InfixExpressionNameString, Datum::DatumTypes::String, 1, offsetof(ActionExpression, _infixExpressionName) },
		};
	}
}