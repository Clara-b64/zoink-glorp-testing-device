/*
** c06_ex01 -- ft_print_params (program): prints each command-line argument
** except argv[0], in order, one per line, on stdout. Compiled with
** -Dmain=studentMain so the test drives the real entry point.
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>

extern int	studentMain(int argc, char **argv);

/* Runs studentMain(argc, argv) with stdout captured into out. */
static int	captureProgram(int argc, char **argv, char *out, int cap)
{
    int	pfd[2];
    int	saved;
    int	n;

    out[0] = '\0';
    saved = dup(STDOUT_FILENO);
    if (saved == -1)
        return (-1);
    if (pipe(pfd) == -1)
    {
        close(saved);
        return (-1);
    }
    dup2(pfd[1], STDOUT_FILENO);
    close(pfd[1]);
    studentMain(argc, argv);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    return (n);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Several arguments are printed in order, one per line. */
static void	testPrintParamsInOrder(void)
{
    char	*argv[] = {"prog", "first", "second", "third", NULL};
    char	out[256];

    captureProgram(4, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("first\nsecond\nthird\n", out);
}

/* A single argument prints one line. */
static void	testPrintParamsSingle(void)
{
    char	*argv[] = {"prog", "only", NULL};
    char	out[128];

    captureProgram(2, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("only\n", out);
}

/* Arguments are printed verbatim, spaces and symbols included. */
static void	testPrintParamsVerbatim(void)
{
    char	*argv[] = {"prog", "hello world", "42*", "", NULL};
    char	out[256];

    captureProgram(4, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("hello world\n42*\n\n", out);
}

/* No arguments: nothing is printed. */
static void	testPrintParamsNone(void)
{
    char	*argv[] = {"prog", NULL};
    char	out[64];
    int		n;

    n = captureProgram(1, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(0, n);
}

void	glorpRun(void)
{
    RUN_TEST(testPrintParamsInOrder);
    RUN_TEST(testPrintParamsSingle);
    RUN_TEST(testPrintParamsVerbatim);
    RUN_TEST(testPrintParamsNone);
}
