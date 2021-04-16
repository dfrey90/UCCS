"""
Daniel Frey
CS 3160-001
Extra Credit Project
12/7/18
"""

print("Welcome to Postfix Calculator!")
print("This calculator will take a postfix expression and evaluate it.")
print("Acceptable operators are: + - * / ^ % ")
print("Use a space to separate tokens.")
print("Type '?' for help.")
print("Type 'quit' to exit the program.\n")

runCalc = True


# Function to determine if token x is a number
def isDigit(x):
    try:
        float(x)
        return True
    except ValueError:
        return False


# Function to return error msg
def err(expr):
    return "Error - invalid expression: " + expr


# Function to display help
def hlp():
    return "Acceptable operators are: + - * / ^ % " + "\nType 'quit' to exit the program"


# Function to evaluate postfix expression
def evalPostfix():
    # For use of regular expressions
    import re

    # Get access to global variable run
    global runCalc

    # Get user's postfix expression and remove leading/trailing whitespace
    in_expr = input("\nEnter a postfix expression to evaluate:")

    # If there are multiple spaces in the expression then trim down to single space
    in_expr = ' '.join(in_expr.split())

    # If user enters 'quit' then stop the program, otherwise keep going and evaluate expression
    if in_expr == 'quit':
        runCalc = False
        return "Thank you for using the Postfix Calculator!"
    elif in_expr == '?':
        return hlp()
    else:
        # Split the user's expression into tokens separated by a single space
        expr_token = re.split(' ', in_expr)

        # Use list as stack to evaluate expression
        eval_stack = []

        # Patterns to match when looping through expression
        operator_pattern = re.compile("[+-/*^%]")

        # Loop through the expression scanning for operands and operators
        for token in expr_token:
            # If operand then push onto stack, if operator then perform action, else error
            if isDigit(token):
                eval_stack.append(token)
            elif operator_pattern.match(token):
                # If 2 operands are able to be applied to an operator, else error in expression
                if len(eval_stack) > 1:
                    val1 = float(eval_stack.pop())
                    val2 = float(eval_stack.pop())
                else:
                    return err(in_expr)

                # Match operator and perform action
                if token == '+':
                    # Push result onto stack
                    eval_stack.append(val2 + val1)
                elif token == '-':
                    # Push result onto stack
                    eval_stack.append(val2 - val1)
                elif token == '*':
                    # Push result onto stack
                    eval_stack.append(val2 * val1)
                elif token == '/':
                    # Push result onto stack
                    eval_stack.append(val2 / val1)
                elif token == '^':
                    # Push result onto stack
                    eval_stack.append(pow(val2, val1))
                elif token == '%':
                    # Push result onto stack
                    eval_stack.append(val2 % val1)
            else:
                return err(in_expr)

        # If more than one thing left in the stack then must have been invalid expression
        if len(eval_stack) == 1:
            return in_expr + " = " + str(eval_stack.pop())
        else:
            return err(in_expr)


while runCalc:
    result = evalPostfix()
    print(result)
