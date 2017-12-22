start - begins the program
Assignment operator: NAME: = EXPRESSION
NAME is the name of a variable, consisting of one Latin letter,
all language variables have integer type and take values ??from -1000 to 1000,
variables are not explicitly described and have default values ??of zero. EXPRESSION is an integer expression consisting of operands and operation signs,
operands can be constants, variable names, and pointers to built-in functions.
Language constants are ordinary integer decimal constants from -1000 to 1000.
The language contains the built-in abs function.
The language includes 4 operations: "+" -set, "-" - subtraction, "*" - multiplication and ":" - integer division.
The priorities of the "*" and ":" operations are higher than the priorities of the "+" and "-" operations, the operations of the same priority are performed in a sequence from left to right, the parentheses in the expressions are not allowed.
Jump statement: go MARK
Here MARK is a label of any program statement representing an integer from 0 to 1000.
MARK can be specified at the beginning of any statement and separated from the operator by the "#" symbol.
Conditional operator: if (RELATION) OPERATOR
RELATION is a design consisting of two expressions separated by a relationship sign,
the relationship signs are "=", "! =", ">", "> =", "<", "<=".
The OPERATOR is any operator except the conditional one.
The input operator: get(NAME)
NAME is the name of a variable.
The output operator: put (EXPRESSION / LITER)
here EXPRESSION is the correct expression of the language A,
LITER - construction $ sequence of characters.
finish - stops the program.