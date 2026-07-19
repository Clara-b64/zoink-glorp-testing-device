/*
** c11_ex05 -- do_op (program): ./do_op value1 op value2 prints the result of
** the operation followed by a newline. op is one of + - * / %; any other op
** prints "0". Division by zero prints "Stop : division by zero", modulo by
** zero "Stop : modulo by zero" (each with a newline). Values are read like
** atoi. The wrong-argument-count case is left untested (subject wording
** varies). Compiled with -Dmain=studentMain; the sources live in srcs/ and
** are built with -Iincludes by the SRCS_EXERCISES makefile path.
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>

extern int	studentMain(int argc, char **argv);

/* Runs do_op's entry point on the three operands with stdout captured. */
static void	captureDoOp(char *a, char *op, char *b, char *out, int cap)
{
    int		pfd[2];
    int		saved;
    int		n;
    char	*argv[5];

    argv[0] = "do_op";
    argv[1] = a;
    argv[2] = op;
    argv[3] = b;
    argv[4] = NULL;
    saved = dup(STDOUT_FILENO);
    pipe(pfd);
    dup2(pfd[1], STDOUT_FILENO);
    close(pfd[1]);
    studentMain(4, argv);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    close(pfd[0]);
}

static void	assertDoOp(char *a, char *op, char *b, char *expected)
{
    char	out[128];

    captureDoOp(a, op, b, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING(expected, out);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The subject's own example. */
static void	testDoOpSubjectExample(void)
{
    assertDoOp("123", "*", "456", "56088\n");
}

/* Each operator on plain values. */
static void	testDoOpAllOperators(void)
{
    assertDoOp("40", "+", "2", "42\n");
    assertDoOp("40", "-", "2", "38\n");
    assertDoOp("6", "*", "7", "42\n");
    assertDoOp("84", "/", "2", "42\n");
    assertDoOp("10", "%", "3", "1\n");
}

/* Negative operands follow C semantics. */
static void	testDoOpNegatives(void)
{
    assertDoOp("-42", "+", "-8", "-50\n");
    assertDoOp("-7", "*", "6", "-42\n");
    assertDoOp("7", "-", "49", "-42\n");
}

/* Integer division truncates. */
static void	testDoOpDivisionTruncates(void)
{
    assertDoOp("7", "/", "2", "3\n");
    assertDoOp("1", "/", "2", "0\n");
}

/* Division and modulo by zero print the stop messages. */
static void	testDoOpByZero(void)
{
    assertDoOp("42", "/", "0", "Stop : division by zero\n");
    assertDoOp("42", "%", "0", "Stop : modulo by zero\n");
}

/* An unknown operator prints 0. */
static void	testDoOpUnknownOperator(void)
{
    assertDoOp("42", "p", "21", "0\n");
    assertDoOp("1", "add", "2", "0\n");
}

void	glorpRun(void)
{
    RUN_TEST(testDoOpSubjectExample);
    RUN_TEST(testDoOpAllOperators);
    RUN_TEST(testDoOpNegatives);
    RUN_TEST(testDoOpDivisionTruncates);
    RUN_TEST(testDoOpByZero);
    RUN_TEST(testDoOpUnknownOperator);
}
