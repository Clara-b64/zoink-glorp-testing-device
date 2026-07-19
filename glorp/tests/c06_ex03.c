/*
** c06_ex03 -- ft_sort_params (program): prints the command-line arguments
** except argv[0] sorted by ASCII order (strcmp), one per line, on stdout.
** Compiled with -Dmain=studentMain so the test drives the real entry point.
** Each test builds a fresh argv because the program may sort it in place.
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

/* Shuffled words come out in ascending ASCII order. */
static void	testSortParamsBasic(void)
{
    char	*argv[] = {"prog", "banana", "apple", "cherry", NULL};
    char	out[256];

    captureProgram(4, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("apple\nbanana\ncherry\n", out);
}

/* ASCII order: digits before uppercase before lowercase. */
static void	testSortParamsAsciiOrder(void)
{
    char	*argv[] = {"prog", "apple", "42", "Apple", NULL};
    char	out[256];

    captureProgram(4, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("42\nApple\napple\n", out);
}

/* Duplicates are all kept. */
static void	testSortParamsDuplicates(void)
{
    char	*argv[] = {"prog", "b", "a", "b", "a", NULL};
    char	out[128];

    captureProgram(5, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("a\na\nb\nb\n", out);
}

/* A prefix sorts before its extension. */
static void	testSortParamsPrefix(void)
{
    char	*argv[] = {"prog", "apple", "app", NULL};
    char	out[128];

    captureProgram(3, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("app\napple\n", out);
}

/* An already sorted list is unchanged. */
static void	testSortParamsAlreadySorted(void)
{
    char	*argv[] = {"prog", "a", "b", "c", NULL};
    char	out[128];

    captureProgram(4, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("a\nb\nc\n", out);
}

/* No arguments: nothing is printed. */
static void	testSortParamsNone(void)
{
    char	*argv[] = {"prog", NULL};
    char	out[64];
    int		n;

    n = captureProgram(1, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(0, n);
}

void	glorpRun(void)
{
    RUN_TEST(testSortParamsBasic);
    RUN_TEST(testSortParamsAsciiOrder);
    RUN_TEST(testSortParamsDuplicates);
    RUN_TEST(testSortParamsPrefix);
    RUN_TEST(testSortParamsAlreadySorted);
    RUN_TEST(testSortParamsNone);
}
