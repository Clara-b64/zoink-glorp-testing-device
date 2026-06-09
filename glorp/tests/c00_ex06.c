/*
** c00_ex06 -- ft_print_comb2: void ft_print_comb2(void);
** Writes every pair (a, b) with a < b and 0 <= a < b <= 99, each formatted as
** two zero-padded numbers "aa bb", in ascending order, joined by ", ", with no
** separator after the last entry ("98 99"). 4950 pairs -> 34648 bytes, no
** newline. Each entry is 5 bytes, each separator 2 bytes, so the stride from
** one entry to the next is 7 bytes.
*/

#include "runner.h"
#include <unistd.h>

#define COMB2_BYTES 34648
#define COMB2_ENTRIES 4950
#define COMB2_STRIDE 7
#define COMB2_BUFSIZE 40000

extern void	ft_print_comb2(void);

/* Builds the reference output into `buf`: every ascending pair, ", " before
** each entry except the first. Returns the length. */
static int	buildExpectedComb2(char *buf)
{
    int	a;
    int	b;
    int	len;

    len = 0;
    a = 0;
    while (a <= 98)
    {
        b = a + 1;
        while (b <= 99)
        {
            if (len > 0)
            {
                buf[len++] = ',';
                buf[len++] = ' ';
            }
            buf[len++] = a / 10 + '0';
            buf[len++] = a % 10 + '0';
            buf[len++] = ' ';
            buf[len++] = b / 10 + '0';
            buf[len++] = b % 10 + '0';
            b++;
        }
        a++;
    }
    buf[len] = '\0';
    return (len);
}

/* Runs ft_print_comb2() with fd 1 redirected into a pipe, restores stdout, then
** returns the bytes emitted via `out` and their count (-1 on failure). */
static int	capturePrintComb2(char *out, int cap)
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
    ft_print_comb2();
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

/* The whole output matches the reference list of ascending pairs exactly. */
static void	testPrintComb2MatchesAllAscendingPairs(void)
{
    char	out[COMB2_BUFSIZE];
    char	expected[COMB2_BUFSIZE];
    int		n;
    int		len;

    len = buildExpectedComb2(expected);
    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(len, n);
    TEST_ASSERT_EQUAL_STRING(expected, out);
}

/* 4950 entries plus 4949 ", " separators weigh exactly 34648 bytes. */
static void	testPrintComb2WritesExpectedByteCount(void)
{
    char	out[COMB2_BUFSIZE];
    int		n;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(COMB2_BYTES, n);
}

/* The list begins with the smallest pair, "00 01", followed by a separator. */
static void	testPrintComb2StartsAt0001(void)
{
    char	out[COMB2_BUFSIZE];
    int		n;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_TRUE(n >= 7);
    TEST_ASSERT_EQUAL_MEMORY("00 01, ", out, 7);
}

/* The list ends with the largest pair, "98 99", and no trailing separator. */
static void	testPrintComb2EndsAt9899WithoutTrailingSeparator(void)
{
    char	out[COMB2_BUFSIZE];
    int		n;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(COMB2_BYTES, n);
    TEST_ASSERT_EQUAL_MEMORY("98 99", out + n - 5, 5);
    TEST_ASSERT_EQUAL_CHAR('9', out[n - 1]);
}

/* Each entry is "aa bb": two zero-padded numbers, a space, joined by ", ". */
static void	testPrintComb2EveryEntryIsWellFormed(void)
{
    char	out[COMB2_BUFSIZE];
    int		n;
    int		i;
    int		base;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(COMB2_BYTES, n);
    i = 0;
    while (i < COMB2_ENTRIES)
    {
        base = i * COMB2_STRIDE;
        TEST_ASSERT_TRUE(out[base] >= '0' && out[base] <= '9');
        TEST_ASSERT_TRUE(out[base + 1] >= '0' && out[base + 1] <= '9');
        TEST_ASSERT_EQUAL_CHAR(' ', out[base + 2]);
        TEST_ASSERT_TRUE(out[base + 3] >= '0' && out[base + 3] <= '9');
        TEST_ASSERT_TRUE(out[base + 4] >= '0' && out[base + 4] <= '9');
        if (i < COMB2_ENTRIES - 1)
        {
            TEST_ASSERT_EQUAL_CHAR(',', out[base + 5]);
            TEST_ASSERT_EQUAL_CHAR(' ', out[base + 6]);
        }
        i++;
    }
}

/* Within every entry the first number is strictly less than the second. */
static void	testPrintComb2FirstNumberLessThanSecond(void)
{
    char	out[COMB2_BUFSIZE];
    int		n;
    int		i;
    int		base;
    int		a;
    int		b;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(COMB2_BYTES, n);
    i = 0;
    while (i < COMB2_ENTRIES)
    {
        base = i * COMB2_STRIDE;
        a = (out[base] - '0') * 10 + (out[base + 1] - '0');
        b = (out[base + 3] - '0') * 10 + (out[base + 4] - '0');
        TEST_ASSERT_TRUE(a < b);
        i++;
    }
}

/* Exactly 4950 entries, i.e. 4949 ", " separators. */
static void	testPrintComb2HasExactlyExpectedEntryCount(void)
{
    char	out[COMB2_BUFSIZE];
    int		n;
    int		i;
    int		separators;

    n = capturePrintComb2(out, sizeof(out));
    separators = 0;
    i = 0;
    while (i + 1 < n)
    {
        if (out[i] == ',' && out[i + 1] == ' ')
            separators++;
        i++;
    }
    TEST_ASSERT_EQUAL_INT(COMB2_ENTRIES - 1, separators);
}

/* Each pair is numerically greater than the one before it. */
static void	testPrintComb2PairsAreStrictlyIncreasing(void)
{
    char	out[COMB2_BUFSIZE];
    int		n;
    int		i;
    int		base;
    int		prev;
    int		cur;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(COMB2_BYTES, n);
    prev = -1;
    i = 0;
    while (i < COMB2_ENTRIES)
    {
        base = i * COMB2_STRIDE;
        cur = ((out[base] - '0') * 10 + (out[base + 1] - '0')) * 100
            + (out[base + 3] - '0') * 10 + (out[base + 4] - '0');
        TEST_ASSERT_TRUE(cur > prev);
        prev = cur;
        i++;
    }
}

/* Every ascending pair a<b (0..99) is present exactly once. */
static void	testPrintComb2ContainsEveryAscendingPairExactlyOnce(void)
{
    static int	seen[10000];
    char		out[COMB2_BUFSIZE];
    int			n;
    int			i;
    int			base;
    int			a;
    int			b;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(COMB2_BYTES, n);
    i = 0;
    while (i < 10000)
        seen[i++] = 0;
    i = 0;
    while (i < COMB2_ENTRIES)
    {
        base = i * COMB2_STRIDE;
        a = (out[base] - '0') * 10 + (out[base + 1] - '0');
        b = (out[base + 3] - '0') * 10 + (out[base + 4] - '0');
        seen[a * 100 + b]++;
        i++;
    }
    a = 0;
    while (a <= 98)
    {
        b = a + 1;
        while (b <= 99)
        {
            TEST_ASSERT_EQUAL_INT(1, seen[a * 100 + b]);
            b++;
        }
        a++;
    }
}

/* The enumeration is correct pairwise: inside a row b grows by 1, and after
** "aa 99" it rolls over to "(aa+1) (aa+2)". This targets the row-reset logic. */
static void	testPrintComb2RowRolloverIsCorrect(void)
{
    char	out[COMB2_BUFSIZE];
    int		n;
    int		i;
    int		base;
    int		a;
    int		b;
    int		na;
    int		nb;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(COMB2_BYTES, n);
    i = 0;
    while (i < COMB2_ENTRIES - 1)
    {
        base = i * COMB2_STRIDE;
        a = (out[base] - '0') * 10 + (out[base + 1] - '0');
        b = (out[base + 3] - '0') * 10 + (out[base + 4] - '0');
        na = (out[base + 7] - '0') * 10 + (out[base + 8] - '0');
        nb = (out[base + 10] - '0') * 10 + (out[base + 11] - '0');
        if (b == 99)
        {
            TEST_ASSERT_EQUAL_INT(a + 1, na);
            TEST_ASSERT_EQUAL_INT(a + 2, nb);
        }
        else
        {
            TEST_ASSERT_EQUAL_INT(a, na);
            TEST_ASSERT_EQUAL_INT(b + 1, nb);
        }
        i++;
    }
}

/* Each first number a appears in exactly (99 - a) entries (its whole row). */
static void	testPrintComb2RowSizesAreCorrect(void)
{
    static int	rowcount[100];
    char		out[COMB2_BUFSIZE];
    int			n;
    int			i;
    int			a;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(COMB2_BYTES, n);
    i = 0;
    while (i < 100)
        rowcount[i++] = 0;
    i = 0;
    while (i < COMB2_ENTRIES)
    {
        a = (out[i * COMB2_STRIDE] - '0') * 10 + (out[i * COMB2_STRIDE + 1] - '0');
        rowcount[a]++;
        i++;
    }
    a = 0;
    while (a <= 98)
    {
        TEST_ASSERT_EQUAL_INT(99 - a, rowcount[a]);
        a++;
    }
}

/* The output contains only digits, spaces and commas -- no stray bytes. */
static void	testPrintComb2ContainsOnlyExpectedCharacters(void)
{
    char	out[COMB2_BUFSIZE];
    int		n;
    int		i;
    char	c;

    n = capturePrintComb2(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(COMB2_BYTES, n);
    i = 0;
    while (i < n)
    {
        c = out[i];
        TEST_ASSERT_TRUE((c >= '0' && c <= '9') || c == ' ' || c == ',');
        i++;
    }
}

/* Calling the function twice yields byte-for-byte identical output. */
static void	testPrintComb2IsDeterministic(void)
{
    static char	first[COMB2_BUFSIZE];
    static char	second[COMB2_BUFSIZE];
    int			n1;
    int			n2;

    n1 = capturePrintComb2(first, sizeof(first));
    n2 = capturePrintComb2(second, sizeof(second));
    TEST_ASSERT_EQUAL_INT(n1, n2);
    TEST_ASSERT_EQUAL_MEMORY(first, second, n1);
}

void	glorpRun(void)
{
    RUN_TEST(testPrintComb2MatchesAllAscendingPairs);
    RUN_TEST(testPrintComb2WritesExpectedByteCount);
    RUN_TEST(testPrintComb2StartsAt0001);
    RUN_TEST(testPrintComb2EndsAt9899WithoutTrailingSeparator);
    RUN_TEST(testPrintComb2EveryEntryIsWellFormed);
    RUN_TEST(testPrintComb2FirstNumberLessThanSecond);
    RUN_TEST(testPrintComb2HasExactlyExpectedEntryCount);
    RUN_TEST(testPrintComb2PairsAreStrictlyIncreasing);
    RUN_TEST(testPrintComb2ContainsEveryAscendingPairExactlyOnce);
    RUN_TEST(testPrintComb2RowRolloverIsCorrect);
    RUN_TEST(testPrintComb2RowSizesAreCorrect);
    RUN_TEST(testPrintComb2ContainsOnlyExpectedCharacters);
    RUN_TEST(testPrintComb2IsDeterministic);
}
