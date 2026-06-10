/*
** c03_ex04 -- ft_strstr: char *ft_strstr(char *str, char *to_find);
** Must behave exactly like libc strstr: return a pointer to the first
** occurrence of to_find inside str, or NULL if it does not occur. An empty
** to_find returns str.
**
** Both ft_strstr and strstr are called on the SAME haystack buffer, so their
** returned pointers index into the same memory and can be compared exactly.
** (Comparing results from two separate string literals would be meaningless.)
*/

#include "runner.h"
#include <string.h>

extern char	*ft_strstr(char *str, char *to_find);

/* Asserts ft_strstr returns exactly the same pointer as libc strstr for the
** same haystack/needle. */
static void	assertStrstr(char *haystack, char *needle)
{
    TEST_ASSERT_EQUAL_PTR(strstr(haystack, needle), ft_strstr(haystack, needle));
}

/* Fills buf with `len` chars over the 2-letter alphabet {a,b} encoded by mask. */
static void	genBinString(char *buf, int len, unsigned int mask)
{
    int	i;

    i = 0;
    while (i < len)
    {
        buf[i] = (char)('a' + ((mask >> i) & 1u));
        i++;
    }
    buf[len] = '\0';
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Found at the start, middle and end. */
static void	testStrstrFoundPositions(void)
{
    assertStrstr("hello world", "hello");
    assertStrstr("hello world", "lo wo");
    assertStrstr("hello world", "world");
}

/* Not present at all. */
static void	testStrstrNotFound(void)
{
    assertStrstr("hello world", "xyz");
    assertStrstr("abc", "abcd");
    assertStrstr("abc", "d");
}

/* An empty needle returns the haystack pointer. */
static void	testStrstrEmptyNeedle(void)
{
    assertStrstr("hello", "");
    assertStrstr("", "");
}

/* An empty haystack with a non-empty needle is not found. */
static void	testStrstrEmptyHaystack(void)
{
    assertStrstr("", "a");
}

/* The needle equal to the whole haystack matches at offset 0. */
static void	testStrstrWholeMatch(void)
{
    assertStrstr("abc", "abc");
}

/* The FIRST occurrence is returned when there are several. */
static void	testStrstrFirstOccurrence(void)
{
    assertStrstr("ababab", "ab");
    assertStrstr("aaa", "aa");
    assertStrstr("xxabcxxabc", "abc");
}

/* Cases that require the search to restart after a partial match. */
static void	testStrstrBacktracking(void)
{
    assertStrstr("aaab", "aab");
    assertStrstr("abababc", "ababc");
    assertStrstr("mississippi", "issip");
    assertStrstr("aaaaaab", "aaab");
}

/* Single-character needle. */
static void	testStrstrSingleChar(void)
{
    assertStrstr("abcabc", "c");
    assertStrstr("abcabc", "a");
    assertStrstr("abcabc", "z");
}

/* Matching is case sensitive. */
static void	testStrstrCaseSensitive(void)
{
    assertStrstr("Hello", "hello");
    assertStrstr("Hello", "Hello");
}

/* High bytes are matched verbatim (byte search, no signedness involved). */
static void	testStrstrHighBytes(void)
{
    char	hay[8];
    char	needle[4];

    hay[0] = 'a';
    hay[1] = (char)0x80;
    hay[2] = (char)0xff;
    hay[3] = 'b';
    hay[4] = '\0';
    needle[0] = (char)0x80;
    needle[1] = (char)0xff;
    needle[2] = '\0';
    assertStrstr(hay, needle);
    needle[0] = (char)0xfe;
    assertStrstr(hay, needle);
}

/* Exhaustive over the alphabet {a,b}: every haystack of length 0..7 against
** every needle of length 0..4. This covers all overlap/backtrack patterns. */
static void	testStrstrExhaustiveBinary(void)
{
    char			hay[16];
    char			needle[8];
    int				hl;
    int				nl;
    unsigned int	hm;
    unsigned int	nm;

    hl = 0;
    while (hl <= 7)
    {
        hm = 0;
        while (hm < (1u << hl))
        {
            genBinString(hay, hl, hm);
            nl = 0;
            while (nl <= 4)
            {
                nm = 0;
                while (nm < (1u << nl))
                {
                    genBinString(needle, nl, nm);
                    TEST_ASSERT_EQUAL_PTR(strstr(hay, needle),
                        ft_strstr(hay, needle));
                    nm++;
                }
                nl++;
            }
            hm++;
        }
        hl++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testStrstrFoundPositions);
    RUN_TEST(testStrstrNotFound);
    RUN_TEST(testStrstrEmptyNeedle);
    RUN_TEST(testStrstrEmptyHaystack);
    RUN_TEST(testStrstrWholeMatch);
    RUN_TEST(testStrstrFirstOccurrence);
    RUN_TEST(testStrstrBacktracking);
    RUN_TEST(testStrstrSingleChar);
    RUN_TEST(testStrstrCaseSensitive);
    RUN_TEST(testStrstrHighBytes);
    RUN_TEST(testStrstrExhaustiveBinary);
}
