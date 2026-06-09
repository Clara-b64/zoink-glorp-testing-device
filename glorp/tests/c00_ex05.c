/*
** c00_ex05 -- ft_print_comb: void ft_print_comb(void);
** Writes every strictly-ascending 3-digit combination (a<b<c) of the digits
** 0-9, in ascending order, separated by ", ", with no separator after the last
** entry ("789"). 120 combinations -> 598 bytes, no newline.
*/

#include "runner.h"
#include <unistd.h>

extern void	ft_print_comb(void);

/* Builds the reference output into `buf` the way the spec defines it: every
** strictly-ascending triple, ", " before each entry except the first. Returns
** the length. */
static int	buildExpectedComb(char *buf)
{
    int	a;
    int	b;
    int	c;
    int	len;

    len = 0;
    a = '0';
    while (a <= '9')
    {
        b = a + 1;
        while (b <= '9')
        {
            c = b + 1;
            while (c <= '9')
            {
                if (len > 0)
                {
                    buf[len++] = ',';
                    buf[len++] = ' ';
                }
                buf[len++] = a;
                buf[len++] = b;
                buf[len++] = c;
                c++;
            }
            b++;
        }
        a++;
    }
    buf[len] = '\0';
    return (len);
}

/* Runs ft_print_comb() with fd 1 redirected into a pipe, restores stdout, then
** returns the bytes emitted via `out` and their count (-1 on failure). */
static int	capturePrintComb(char *out, int cap)
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
    ft_print_comb();
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

/* The whole output matches the reference list of ascending triples exactly. */
static void	testPrintCombMatchesAllAscendingTriples(void)
{
    char	out[1024];
    char	expected[1024];
    int		n;
    int		len;

    len = buildExpectedComb(expected);
    n = capturePrintComb(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(len, n);
    TEST_ASSERT_EQUAL_STRING(expected, out);
}

/* 120 combinations with ", " between them weigh exactly 598 bytes. */
static void	testPrintCombWritesFiveHundredNinetyEightBytes(void)
{
    char	out[1024];
    int		n;

    n = capturePrintComb(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(598, n);
}

/* The list begins with the smallest triple, "012", followed by a separator. */
static void	testPrintCombStartsAt012(void)
{
    char	out[1024];
    int		n;

    n = capturePrintComb(out, sizeof(out));
    TEST_ASSERT_TRUE(n >= 5);
    TEST_ASSERT_EQUAL_MEMORY("012, ", out, 5);
}

/* The list ends with the largest triple, "789", and no trailing separator. */
static void	testPrintCombEndsAt789WithoutTrailingSeparator(void)
{
    char	out[1024];
    int		n;

    n = capturePrintComb(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(598, n);
    TEST_ASSERT_EQUAL_MEMORY("789", out + n - 3, 3);
    TEST_ASSERT_EQUAL_CHAR('9', out[n - 1]);
}

/* Each entry is three strictly-ascending digits, joined by ", ". */
static void	testPrintCombEveryTripleIsStrictlyAscending(void)
{
    char	out[1024];
    int		n;
    int		i;
    int		base;

    n = capturePrintComb(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(598, n);
    i = 0;
    while (i < 120)
    {
        base = i * 5;
        TEST_ASSERT_TRUE(out[base] >= '0' && out[base + 2] <= '9');
        TEST_ASSERT_TRUE(out[base] < out[base + 1]);
        TEST_ASSERT_TRUE(out[base + 1] < out[base + 2]);
        if (i < 119)
        {
            TEST_ASSERT_EQUAL_CHAR(',', out[base + 3]);
            TEST_ASSERT_EQUAL_CHAR(' ', out[base + 4]);
        }
        i++;
    }
}

/* Exactly 120 entries, i.e. 119 ", " separators. */
static void	testPrintCombHasExactlyOneHundredTwentyEntries(void)
{
    char	out[1024];
    int		n;
    int		i;
    int		separators;

    n = capturePrintComb(out, sizeof(out));
    separators = 0;
    i = 0;
    while (i + 1 < n)
    {
        if (out[i] == ',' && out[i + 1] == ' ')
            separators++;
        i++;
    }
    TEST_ASSERT_EQUAL_INT(119, separators);
}

/* Each entry is numerically greater than the one before it. */
static void	testPrintCombEntriesAreStrictlyIncreasing(void)
{
    char	out[1024];
    int		n;
    int		i;
    int		base;
    int		prev;
    int		cur;

    n = capturePrintComb(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(598, n);
    prev = -1;
    i = 0;
    while (i < 120)
    {
        base = i * 5;
        cur = (out[base] - '0') * 100 + (out[base + 1] - '0') * 10
            + (out[base + 2] - '0');
        TEST_ASSERT_TRUE(cur > prev);
        prev = cur;
        i++;
    }
}

/* Within every entry the three digits are distinct (the spec's "999" rule). */
static void	testPrintCombDigitsWithinTripleAreDistinct(void)
{
    char	out[1024];
    int		n;
    int		i;
    int		base;

    n = capturePrintComb(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(598, n);
    i = 0;
    while (i < 120)
    {
        base = i * 5;
        TEST_ASSERT_TRUE(out[base] != out[base + 1]);
        TEST_ASSERT_TRUE(out[base + 1] != out[base + 2]);
        TEST_ASSERT_TRUE(out[base] != out[base + 2]);
        i++;
    }
}

/* Every strictly-ascending triple a<b<c is present exactly once. */
static void	testPrintCombContainsEveryAscendingTripleExactlyOnce(void)
{
    char	out[1024];
    int		seen[1000];
    int		n;
    int		i;
    int		a;
    int		b;
    int		c;

    n = capturePrintComb(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(598, n);
    i = 0;
    while (i < 1000)
        seen[i++] = 0;
    i = 0;
    while (i < 120)
    {
        seen[(out[i * 5] - '0') * 100 + (out[i * 5 + 1] - '0') * 10
            + (out[i * 5 + 2] - '0')]++;
        i++;
    }
    a = 0;
    while (a <= 9)
    {
        b = a + 1;
        while (b <= 9)
        {
            c = b + 1;
            while (c <= 9)
            {
                TEST_ASSERT_EQUAL_INT(1, seen[a * 100 + b * 10 + c]);
                c++;
            }
            b++;
        }
        a++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testPrintCombMatchesAllAscendingTriples);
    RUN_TEST(testPrintCombWritesFiveHundredNinetyEightBytes);
    RUN_TEST(testPrintCombStartsAt012);
    RUN_TEST(testPrintCombEndsAt789WithoutTrailingSeparator);
    RUN_TEST(testPrintCombEveryTripleIsStrictlyAscending);
    RUN_TEST(testPrintCombHasExactlyOneHundredTwentyEntries);
    RUN_TEST(testPrintCombEntriesAreStrictlyIncreasing);
    RUN_TEST(testPrintCombDigitsWithinTripleAreDistinct);
    RUN_TEST(testPrintCombContainsEveryAscendingTripleExactlyOnce);
}
