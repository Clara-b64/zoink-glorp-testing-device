/*
** c00_ex08 -- ft_print_combn: void ft_print_combn(int n);
** Writes every strictly-ascending combination of n digits (0-9), in ascending
** order, joined by ", ", with no separator after the last combination. This is
** the generalization of ex05 (which is the n == 3 case). The spec guarantees
** 0 < n < 10, so only n in 1..9 is exercised.
**
** For a size-n combination there are C(10, n) entries; each entry is n bytes
** and each separator is 2 bytes, so the per-entry stride is n + 2.
**
** The reference output is generated here independently, so it is never
** hand-computed.
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>

#define COMBN_BUFSIZE 4096

extern void	ft_print_combn(int n);

/* Number of size-n combinations of 10 digits, i.e. C(10, n). */
static int	binom10(int n)
{
    int	result;
    int	i;

    result = 1;
    i = 0;
    while (i < n)
    {
        result = result * (10 - i) / (i + 1);
        i++;
    }
    return (result);
}

/* Builds the reference output for ft_print_combn(n) into `buf`: every
** strictly-ascending n-digit combination in ascending order, ", " before each
** entry except the first. Returns the length. */
static int	buildExpectedCombn(int n, char *buf)
{
    int	digit[10];
    int	len;
    int	i;
    int	j;

    i = 0;
    while (i < n)
    {
        digit[i] = i;
        i++;
    }
    len = 0;
    while (1)
    {
        if (len > 0)
        {
            buf[len++] = ',';
            buf[len++] = ' ';
        }
        i = 0;
        while (i < n)
            buf[len++] = '0' + digit[i++];
        i = n - 1;
        while (i >= 0 && digit[i] == 10 - n + i)
            i--;
        if (i < 0)
            break ;
        digit[i]++;
        j = i + 1;
        while (j < n)
        {
            digit[j] = digit[j - 1] + 1;
            j++;
        }
    }
    buf[len] = '\0';
    return (len);
}

/* Runs ft_print_combn(n) with fd 1 redirected into a pipe, restores stdout,
** then returns the bytes emitted via `out` and their count (-1 on failure). */
static int	captureCombn(int n, char *out, int cap)
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
    ft_print_combn(n);
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

/* For every valid n the whole output matches the independently-built list. */
static void	testPrintCombnMatchesReferenceForEachN(void)
{
    char	out[COMBN_BUFSIZE];
    char	expected[COMBN_BUFSIZE];
    int		n;
    int		len;
    int		got;

    n = 1;
    while (n <= 9)
    {
        len = buildExpectedCombn(n, expected);
        got = captureCombn(n, out, sizeof(out));
        TEST_ASSERT_EQUAL_INT(len, got);
        TEST_ASSERT_EQUAL_STRING(expected, out);
        n++;
    }
}

/* For every valid n the byte count is C(10,n)*n + (C(10,n)-1)*2. */
static void	testPrintCombnByteCountForEachN(void)
{
    char	out[COMBN_BUFSIZE];
    int		n;
    int		count;
    int		got;

    n = 1;
    while (n <= 9)
    {
        count = binom10(n);
        got = captureCombn(n, out, sizeof(out));
        TEST_ASSERT_EQUAL_INT(count * n + (count - 1) * 2, got);
        n++;
    }
}

/* n == 1: the ten single digits, "0, 1, ..., 9". */
static void	testPrintCombnSingleDigitCase(void)
{
    char	out[COMBN_BUFSIZE];
    int		got;

    got = captureCombn(1, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(28, got);
    TEST_ASSERT_EQUAL_STRING("0, 1, 2, 3, 4, 5, 6, 7, 8, 9", out);
}

/* n == 2: begins "01, ", ends "89", no trailing separator (the user example). */
static void	testPrintCombnN2MatchesExample(void)
{
    char	out[COMBN_BUFSIZE];
    int		got;

    got = captureCombn(2, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(178, got);
    TEST_ASSERT_EQUAL_MEMORY("01, ", out, 4);
    TEST_ASSERT_EQUAL_MEMORY("89", out + got - 2, 2);
    TEST_ASSERT_EQUAL_CHAR('9', out[got - 1]);
}

/* n == 9: the ten 9-digit combinations, "012345678" .. "123456789". */
static void	testPrintCombnLargestNCase(void)
{
    char	out[COMBN_BUFSIZE];
    int		got;

    got = captureCombn(9, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(108, got);
    TEST_ASSERT_EQUAL_MEMORY("012345678, ", out, 11);
    TEST_ASSERT_EQUAL_MEMORY("123456789", out + got - 9, 9);
}

/* For every n the list begins with the smallest combination "012..(n-1)". */
static void	testPrintCombnStartsAtSmallestCombination(void)
{
    char	out[COMBN_BUFSIZE];
    char	first[16];
    int		n;
    int		i;
    int		got;

    n = 1;
    while (n <= 9)
    {
        i = 0;
        while (i < n)
        {
            first[i] = '0' + i;
            i++;
        }
        got = captureCombn(n, out, sizeof(out));
        TEST_ASSERT_TRUE(got >= n);
        TEST_ASSERT_EQUAL_MEMORY(first, out, n);
        n++;
    }
}

/* For every n the list ends with the largest combination and no trailing
** separator. The largest size-n combination is the digits {10-n .. 9}. */
static void	testPrintCombnEndsAtLargestCombination(void)
{
    char	out[COMBN_BUFSIZE];
    char	last[16];
    int		n;
    int		i;
    int		got;

    n = 1;
    while (n <= 9)
    {
        i = 0;
        while (i < n)
        {
            last[i] = '0' + (10 - n + i);
            i++;
        }
        got = captureCombn(n, out, sizeof(out));
        TEST_ASSERT_EQUAL_MEMORY(last, out + got - n, n);
        TEST_ASSERT_EQUAL_CHAR('9', out[got - 1]);
        n++;
    }
}

/* For every n each entry is n strictly-ascending digits joined by ", ". */
static void	testPrintCombnEveryCombinationIsStrictlyAscending(void)
{
    char	out[COMBN_BUFSIZE];
    int		n;
    int		count;
    int		k;
    int		base;
    int		j;
    int		got;

    n = 1;
    while (n <= 9)
    {
        count = binom10(n);
        got = captureCombn(n, out, sizeof(out));
        TEST_ASSERT_EQUAL_INT(count * n + (count - 1) * 2, got);
        k = 0;
        while (k < count)
        {
            base = k * (n + 2);
            j = 0;
            while (j < n)
            {
                TEST_ASSERT_TRUE(out[base + j] >= '0' && out[base + j] <= '9');
                if (j > 0)
                    TEST_ASSERT_TRUE(out[base + j - 1] < out[base + j]);
                j++;
            }
            if (k < count - 1)
            {
                TEST_ASSERT_EQUAL_CHAR(',', out[base + n]);
                TEST_ASSERT_EQUAL_CHAR(' ', out[base + n + 1]);
            }
            k++;
        }
        n++;
    }
}

/* For every n the combinations themselves are in strictly ascending order
** (same length, so a lexicographic compare is the numeric order). */
static void	testPrintCombnEntriesAreStrictlyIncreasing(void)
{
    char	out[COMBN_BUFSIZE];
    int		n;
    int		count;
    int		k;
    int		stride;
    int		got;

    n = 1;
    while (n <= 9)
    {
        count = binom10(n);
        stride = n + 2;
        got = captureCombn(n, out, sizeof(out));
        TEST_ASSERT_EQUAL_INT(count * n + (count - 1) * 2, got);
        k = 1;
        while (k < count)
        {
            TEST_ASSERT_TRUE(memcmp(out + k * stride,
                    out + (k - 1) * stride, n) > 0);
            k++;
        }
        n++;
    }
}

/* Calling the function twice (here n == 5, the widest output) is identical. */
static void	testPrintCombnIsDeterministic(void)
{
    static char	first[COMBN_BUFSIZE];
    static char	second[COMBN_BUFSIZE];
    int			n1;
    int			n2;

    n1 = captureCombn(5, first, sizeof(first));
    n2 = captureCombn(5, second, sizeof(second));
    TEST_ASSERT_EQUAL_INT(n1, n2);
    TEST_ASSERT_EQUAL_MEMORY(first, second, n1);
}

void	glorpRun(void)
{
    RUN_TEST(testPrintCombnMatchesReferenceForEachN);
    RUN_TEST(testPrintCombnByteCountForEachN);
    RUN_TEST(testPrintCombnSingleDigitCase);
    RUN_TEST(testPrintCombnN2MatchesExample);
    RUN_TEST(testPrintCombnLargestNCase);
    RUN_TEST(testPrintCombnStartsAtSmallestCombination);
    RUN_TEST(testPrintCombnEndsAtLargestCombination);
    RUN_TEST(testPrintCombnEveryCombinationIsStrictlyAscending);
    RUN_TEST(testPrintCombnEntriesAreStrictlyIncreasing);
    RUN_TEST(testPrintCombnIsDeterministic);
}
