/*
 * @author Siadun Anton
 * antonsiadun@yandex.by
 *
 * @copyright MIT License (c) 2024 Siadun Anton

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

int evaluate_expression(char *expr, int *position);
int calculate(char *expression);

/**
 * Glue a sign and digits into a natural number.
 * If contains a parenthesis, recursively calculate it like a expression.
 *
 * @param expression: A string representing the arithmetic expression.
 * @param position: The current position in a parsing expression.
 * @return: The natural number with sign.
 */
int get_natural(char *expr, int *position)
{
    int natural = 0;
    int sign = 1;

    if (expr[*position] == '-')
    {
        sign *= -1;
        (*position)++;
    }
    if (expr[*position] == '(')
    {
        (*position)++;
        natural = evaluate_expression(expr, position);
        (*position)++;
    }
    else
    {
        while (isdigit(expr[*position]))
        {
            natural = natural * 10 + expr[*position] - '0';
            (*position)++;

            if (*position >= strlen(expr))
                break;
        }
    }
    return natural * sign;
}

/**
 * Calculates the result of a term - a piece of expression,
 * representing multiply/divide operation.
 *
 * @param expression: A string representing the arithmetic expression.
 * @param position: The current position in a parsing expression.
 * @return: The result of the arithmetic expression.
 */
int evaluate_term(char *expr, int *position)
{
    int result = get_natural(expr, position);

    while (expr[*position] == '*' || expr[*position] == '/')
    {
        char operator= expr[*position];
        (*position)++;
        if (operator == '*')
        {
            result *= get_natural(expr, position);
        }
        else if (operator == '/')
        {
            int operand = get_natural(expr, position);
            if (operand == 0)
                exit(EXIT_FAILURE);
            result /= operand;
        }
    }
    return result;
}

/**
 * Calculates the part result of an arithmetic expression.
 * Main operations - add and substract.
 *
 * @param expression: A string representing the arithmetic expression.
 * @param position: The current position in a parsing expression.
 * @return: The result of the arithmetic expression.
 */
int evaluate_expression(char *expr, int *position)
{
    int result = evaluate_term(expr, position);

    while (expr[*position] == '+' || expr[*position] == '-')
    {
        char operator= expr[*position];
        (*position)++;
        if (operator == '+')
        {
            result += evaluate_term(expr, position);
        }
        else if (operator == '-')
        {
            result -= evaluate_term(expr, position);
        }
    }
    return result;
}

int is_right_char(char c);

/**
 * Calculates the result of an arithmetic expression.
 *
 * @param expression: A string representing the arithmetic expression.
 * @return: The result of the arithmetic expression.
 */
int calculate(char *expression)
{
    // Check for right characters in expression
    for (unsigned int i = 0; i < strlen(expression) - 1; i++)
    {
        if (!is_right_char(expression[i]))
        {
            printf("\nWrong character in the expression.\n");
            exit(EXIT_FAILURE);
        }
    }
    int position = 0;
    return evaluate_expression(expression, &position);
}

int is_right_char(char c)
{
    return c == '(' || c == ')' || c == '+' || c == '-' ||
           c == '*' || c == '/' || isdigit(c);
}


#pragma region TEST_GET_NATURAL
void test_get_natural()
{
    int position = 0;
    assert(get_natural("-1", &position) == -1);
    position = 0;
    assert(get_natural("8.0", &position) == 8);
    position = 0;
    assert(get_natural("2f", &position) == 2);
    position = 0;
    assert(get_natural("xf", &position) == 0);
    position = 0;
    assert(get_natural("(1)", &position) == 1);
    position = 0;
    assert(get_natural("(1+1)", &position) == 2);
    position = 0;
    assert(get_natural("((-1)+1)", &position) == 0);
}
#pragma endregion

#pragma region TEST_EVALUATE_TERM
void test_evaluate_term()
{
    int position = 0;
    assert(evaluate_term("1*2", &position) == 2);
    position = 0;
    assert(evaluate_term("-3*4", &position) == -12);
    position = 0;
    assert(evaluate_term("-1*-4", &position) == 4);
    position = 0;
    assert(evaluate_term("0*-4", &position) == 0);
}
#pragma endregion

#pragma region TEST_EVALUATE_EXPRESSION
void test_evaluate_expression()
{
    int position = 0;
    assert(evaluate_expression("1+2", &position) == 3);
    position = 0;
    assert(evaluate_expression("3+2*1", &position) == 5);
    position = 0;
    assert(evaluate_expression("1*4+2", &position) == 6);
    position = 0;
    assert(evaluate_expression("4*4-3*2", &position) == 10);
    position = 0;
    assert(evaluate_expression("0*4-3*2+1", &position) == -5);
}
#pragma endregion

#pragma region TEST_CALCULATE
void test_calculate()
{
    assert(calculate("(1+3*(-4))/2") == -5);
    assert(calculate("(1*(-2))*(-2)-1*(2+4*2)/3+1") == 2);
    assert(calculate("(1*(-1+2*1)/3") == 0);
    assert(calculate("2/2") == 1);
    assert(calculate("1+2/(1*3)-2") == -1);
}
#pragma endregion

// Include all tests
void test_overall()
{
    test_get_natural();
    test_evaluate_term();
    test_evaluate_expression();
    test_calculate();
}

int main()
{
    test_overall();

    char expression[100];
    printf("Enter an arithmetic expression: ");
    fgets(expression, sizeof(expression), stdin);
    int result = calculate(expression);
    printf("Result: %d\n", result);
    return 0;
}
