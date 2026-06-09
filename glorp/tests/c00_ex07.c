/*
** c00_ex07 -- ft_putnbr: void ft_putnbr(int nb);
** Writes the decimal representation of nb to stdout (fd 1): an optional '-'
** then digits, no leading zeros (except "0" itself), no sign for positives,
** no extra characters. Must handle the whole int range, INT_MIN included.
**
** The expected output is produced by snprintf("%d", nb), so it is never
** hand-computed.
*/

#include "runner.h"
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

extern void	ft_putnbr(int nb);

/* Runs ft_putnbr(nb) with fd 1 redirected into a pipe, restores stdout, then
** returns the bytes emitted via `out` and their count (-1 on failure). */
static int	capturePutnbr(int nb, char *out, int cap)
{
    int	pfd[2];
    int	saved;
    int	n;

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
    ft_putnbr(nb);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    return (n);
}

/* Captures ft_putnbr(nb) and asserts it equals snprintf("%d", nb) exactly,
** both in content and in length (so no stray newline/sign/padding slips by). */
static void	assertPutnbr(int nb)
{
    char	out[16];
    char	expected[16];
    int		n;
    int		len;

    n = capturePutnbr(nb, out, sizeof(out));
    len = snprintf(expected, sizeof(expected), "%d", nb);
    TEST_ASSERT_EQUAL_INT(len, n);
    TEST_ASSERT_EQUAL_STRING(expected, out);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Zero is the single character "0". */
static void	testPutnbrPrintsZero(void)
{
    char	out[16];
    int		n;

    n = capturePutnbr(0, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_STRING("0", out);
}

/* A one-digit positive number. */
static void	testPutnbrPrintsPositiveSingleDigit(void)
{
    assertPutnbr(7);
}

/* A multi-digit positive number. */
static void	testPutnbrPrintsPositiveMultipleDigits(void)
{
    assertPutnbr(12345);
}

/* A one-digit negative number keeps its sign. */
static void	testPutnbrPrintsNegativeSingleDigit(void)
{
    assertPutnbr(-7);
}

/* A multi-digit negative number. */
static void	testPutnbrPrintsNegativeMultipleDigits(void)
{
    assertPutnbr(-12345);
}

/* No leading zeros on round numbers. */
static void	testPutnbrPrintsRoundNumbersWithoutLeadingZeros(void)
{
    assertPutnbr(10);
    assertPutnbr(100);
    assertPutnbr(1000000000);
    assertPutnbr(-10);
    assertPutnbr(-100);
}

/* INT_MAX prints all ten digits. */
static void	testPutnbrPrintsIntMax(void)
{
    char	out[16];
    int		n;

    n = capturePutnbr(INT_MAX, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(10, n);
    TEST_ASSERT_EQUAL_STRING("2147483647", out);
}

/* INT_MIN is the critical case: cannot be negated inside an int. */
static void	testPutnbrPrintsIntMin(void)
{
    char	out[16];
    int		n;

    n = capturePutnbr(INT_MIN, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(11, n);
    TEST_ASSERT_EQUAL_STRING("-2147483648", out);
}

/* Output is '-' (optional) followed by digits only -- nothing else. */
static void	testPutnbrEmitsOnlySignAndDigits(void)
{
    char	out[16];
    int		n;
    int		i;

    n = capturePutnbr(-1234567890, out, sizeof(out));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_EQUAL_CHAR('-', out[0]);
    i = 1;
    while (i < n)
    {
        TEST_ASSERT_TRUE(out[i] >= '0' && out[i] <= '9');
        i++;
    }
}

/* Every value in a dense small range matches printf. */
static void	testPutnbrMatchesPrintfOverSmallRange(void)
{
    int	nb;

    nb = -1000;
    while (nb <= 1000)
    {
        assertPutnbr(nb);
        nb++;
    }
}

/* All the interesting boundaries across the int range match printf. */
static void	testPutnbrMatchesPrintfAtBoundaries(void)
{
    int	edges[23];
    int	i;

    edges[0] = INT_MIN;
    edges[1] = INT_MIN + 1;
    edges[2] = -2000000000;
    edges[3] = -1000000000;
    edges[4] = -999999999;
    edges[5] = -100000;
    edges[6] = -1000;
    edges[7] = -100;
    edges[8] = -10;
    edges[9] = -9;
    edges[10] = -1;
    edges[11] = 0;
    edges[12] = 1;
    edges[13] = 9;
    edges[14] = 10;
    edges[15] = 100;
    edges[16] = 1000;
    edges[17] = 100000;
    edges[18] = 999999999;
    edges[19] = 1000000000;
    edges[20] = 2000000000;
    edges[21] = INT_MAX - 1;
    edges[22] = INT_MAX;
    i = 0;
    while (i < 23)
    {
        assertPutnbr(edges[i]);
        i++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testPutnbrPrintsZero);
    RUN_TEST(testPutnbrPrintsPositiveSingleDigit);
    RUN_TEST(testPutnbrPrintsPositiveMultipleDigits);
    RUN_TEST(testPutnbrPrintsNegativeSingleDigit);
    RUN_TEST(testPutnbrPrintsNegativeMultipleDigits);
    RUN_TEST(testPutnbrPrintsRoundNumbersWithoutLeadingZeros);
    RUN_TEST(testPutnbrPrintsIntMax);
    RUN_TEST(testPutnbrPrintsIntMin);
    RUN_TEST(testPutnbrEmitsOnlySignAndDigits);
    RUN_TEST(testPutnbrMatchesPrintfOverSmallRange);
    RUN_TEST(testPutnbrMatchesPrintfAtBoundaries);
}
