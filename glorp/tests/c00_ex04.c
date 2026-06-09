/*
** c00_ex04 -- ft_is_negative: void ft_is_negative(int n);
** Writes 'N' to stdout (fd 1) when n is negative, 'P' otherwise (zero is 'P').
*/

#include "runner.h"
#include <unistd.h>
#include <limits.h>

extern void	ft_is_negative(int n);

/* Runs ft_is_negative(n) with fd 1 redirected into a pipe, restores stdout,
** then returns the bytes emitted via `out` and their count (-1 on failure). */
static int	captureIsNegative(int n, char *out, int cap)
{
    int	pfd[2];
    int	saved;
    int	count;

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
    ft_is_negative(n);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    count = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (count < 0)
        count = 0;
    out[count] = '\0';
    return (count);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A negative number prints 'N'. */
static void	testIsNegativeNegativePrintsN(void)
{
    char	out[8];
    int		count;

    count = captureIsNegative(-1, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_CHAR('N', out[0]);
}

/* A positive number prints 'P'. */
static void	testIsNegativePositivePrintsP(void)
{
    char	out[8];
    int		count;

    count = captureIsNegative(1, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_CHAR('P', out[0]);
}

/* Zero counts as non-negative, so it prints 'P'. */
static void	testIsNegativeZeroPrintsP(void)
{
    char	out[8];
    int		count;

    count = captureIsNegative(0, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_CHAR('P', out[0]);
}

/* INT_MIN is negative -> 'N'. */
static void	testIsNegativeIntMinPrintsN(void)
{
    char	out[8];
    int		count;

    count = captureIsNegative(INT_MIN, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_CHAR('N', out[0]);
}

/* INT_MAX is positive -> 'P'. */
static void	testIsNegativeIntMaxPrintsP(void)
{
    char	out[8];
    int		count;

    count = captureIsNegative(INT_MAX, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_CHAR('P', out[0]);
}

/* Exactly one byte: no trailing newline and no extra characters. */
static void	testIsNegativeWritesExactlyOneByte(void)
{
    char	out[8];
    int		count;

    count = captureIsNegative(42, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, count);
}

void	glorpRun(void)
{
    RUN_TEST(testIsNegativeNegativePrintsN);
    RUN_TEST(testIsNegativePositivePrintsP);
    RUN_TEST(testIsNegativeZeroPrintsP);
    RUN_TEST(testIsNegativeIntMinPrintsN);
    RUN_TEST(testIsNegativeIntMaxPrintsP);
    RUN_TEST(testIsNegativeWritesExactlyOneByte);
}
