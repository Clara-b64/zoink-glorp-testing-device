/*
** c06_ex00 -- ft_print_program_name (program): prints argv[0] followed by a
** newline on stdout, whatever else is on the command line. The piscine sources
** are compiled with -Dmain=studentMain, so the test invokes the program's real
** entry point with a fabricated argv.
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

/* With no arguments the program name is printed with a trailing newline. */
static void	testProgramNamePlain(void)
{
    char	*argv[] = {"ft_print_program_name", NULL};
    char	out[128];
    int		n;

    n = captureProgram(1, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT((int)strlen("ft_print_program_name\n"), n);
    TEST_ASSERT_EQUAL_STRING("ft_print_program_name\n", out);
}

/* argv[0] is echoed as given, path prefix included. */
static void	testProgramNameWithPath(void)
{
    char	*argv[] = {"./a.out", NULL};
    char	out[128];

    captureProgram(1, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("./a.out\n", out);
}

/* Extra arguments change nothing: only the program name is printed. */
static void	testProgramNameIgnoresArguments(void)
{
    char	*argv[] = {"prog", "one", "two", NULL};
    char	out[128];

    captureProgram(3, argv, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("prog\n", out);
}

void	glorpRun(void)
{
    RUN_TEST(testProgramNamePlain);
    RUN_TEST(testProgramNameWithPath);
    RUN_TEST(testProgramNameIgnoresArguments);
}
