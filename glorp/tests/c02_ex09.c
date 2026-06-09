/*
** c02_ex09 -- ft_strcapitalize: char *ft_strcapitalize(char *str);
** Capitalizes the first letter of each word and lowercases the rest, in place,
** and returns str. A "word" is a maximal run of ALPHANUMERIC bytes; any other
** byte is a separator that starts a new word.
**
** Reference rule (matches the picine_1 implementation and the canonical 42
** example): walking left to right with a `space` flag (1 at start and after any
** separator):
**   - separator byte      -> space = 1
**   - first byte of a word -> if it is a lowercase letter, uppercase it; space=0
**   - later byte of a word -> if it is an uppercase letter, lowercase it
** Because digits are alphanumeric, a word that begins with a digit (e.g.
** "42Mots") has already cleared `space` by the time its first letter is seen,
** so that letter is LOWERCASED: "42Mots" -> "42mots".
**
** Since behaviour at any position depends only on (previous-was-separator,
** current byte), the (prev, curr) sweep over all byte pairs is exhaustive.
*/

#include "runner.h"
#include <string.h>

#define CAP_BUFSIZE 1024

extern char	*ft_strcapitalize(char *str);

/* 1 if c is NOT alphanumeric (a separator), else 0 -- same as the reference. */
static int	refIsSep(char c)
{
    if (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z')
        && !(c >= '0' && c <= '9'))
        return (1);
    return (0);
}

/* Reference capitalize, replicating the reference state machine exactly. */
static void	refCapitalize(char *s)
{
    int	i;
    int	space;

    space = 1;
    i = 0;
    while (s[i] != '\0')
    {
        if (refIsSep(s[i]))
            space = 1;
        else
        {
            if (space == 1 && s[i] >= 'a' && s[i] <= 'z')
                s[i] = (char)(s[i] - 32);
            if (space == 0 && s[i] >= 'A' && s[i] <= 'Z')
                s[i] = (char)(s[i] + 32);
            space = 0;
        }
        i++;
    }
}

/* Runs ft_strcapitalize on a writable sentinel-filled copy of `input`, runs the
** reference on a parallel copy, and asserts full-buffer equality plus that
** ft_strcapitalize returns its argument. */
static void	assertCapitalize(char *input)
{
    char	mine[CAP_BUFSIZE];
    char	ref[CAP_BUFSIZE];
    char	*ret;
    int		len;

    len = (int)strlen(input);
    memset(mine, 'X', sizeof(mine));
    memset(ref, 'X', sizeof(ref));
    memcpy(mine, input, len + 1);
    memcpy(ref, input, len + 1);
    ret = ft_strcapitalize(mine);
    refCapitalize(ref);
    TEST_ASSERT_EQUAL_PTR(mine, ret);
    TEST_ASSERT_EQUAL_MEMORY(ref, mine, sizeof(mine));
}

/* Runs ft_strcapitalize on a writable copy of `input` and checks it equals the
** hand-written `expected` (independent documentation of the behaviour). */
static void	expectCapitalize(char *input, char *expected)
{
    char	buf[CAP_BUFSIZE];
    char	*ret;

    memcpy(buf, input, strlen(input) + 1);
    ret = ft_strcapitalize(buf);
    TEST_ASSERT_EQUAL_PTR(buf, ret);
    TEST_ASSERT_EQUAL_STRING(expected, buf);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Simple lowercase words get their first letter capitalized. */
static void	testCapitalizeBasic(void)
{
    expectCapitalize("hello world", "Hello World");
}

/* All-uppercase input is normalized: first letter stays, rest lowercased. */
static void	testCapitalizeAllUppercase(void)
{
    expectCapitalize("HELLO WORLD", "Hello World");
}

/* Ragged casing is normalized per word. */
static void	testCapitalizeRaggedCase(void)
{
    expectCapitalize("hELLO wORLD", "Hello World");
}

/* The canonical 42 example. */
static void	testCapitalizeCanonicalExample(void)
{
    expectCapitalize(
        "salut, comment tu vas ? 42mots quarante-deux cinquante+et+un",
        "Salut, Comment Tu Vas ? 42mots Quarante-Deux Cinquante+Et+Un");
}

/* A word that begins with a digit: the first letter after it is lowercased. */
static void	testCapitalizeWordStartingWithDigit(void)
{
    expectCapitalize("42mots", "42mots");
    expectCapitalize("42Mots", "42mots");
    expectCapitalize("42ABC", "42abc");
}

/* Digits inside a word are kept; surrounding letters follow word position. */
static void	testCapitalizeDigitsInsideWord(void)
{
    expectCapitalize("ab12cd", "Ab12cd");
    expectCapitalize("fOO123bAr", "Foo123bar");
}

/* Single characters. */
static void	testCapitalizeSingleChar(void)
{
    expectCapitalize("a", "A");
    expectCapitalize("A", "A");
    expectCapitalize("z", "Z");
    expectCapitalize("5", "5");
    expectCapitalize("!", "!");
}

/* The empty string stays empty and the pointer is returned. */
static void	testCapitalizeEmpty(void)
{
    expectCapitalize("", "");
}

/* Leading and trailing separators are preserved. */
static void	testCapitalizeLeadingAndTrailingSeparators(void)
{
    expectCapitalize("  hello  ", "  Hello  ");
    expectCapitalize("a,,b", "A,,B");
}

/* Various separators (space, tab, newline, punctuation) all start words. */
static void	testCapitalizeVariousSeparators(void)
{
    expectCapitalize("hello\nworld\tfoo.bar", "Hello\nWorld\tFoo.Bar");
}

/* Every single byte as a one-char string matches the reference. */
static void	testCapitalizeSingleByte(void)
{
    char	mine[2];
    char	ref[2];
    int		c;

    c = 1;
    while (c <= 255)
    {
        mine[0] = (char)c;
        mine[1] = '\0';
        ref[0] = (char)c;
        ref[1] = '\0';
        ft_strcapitalize(mine);
        refCapitalize(ref);
        TEST_ASSERT_EQUAL_MEMORY(ref, mine, 2);
        c++;
    }
}

/* Exhaustive: every (prev, curr) byte pair. Because the function's behaviour at
** a position depends only on whether the previous byte was a separator and on
** the current byte, this fully characterizes ft_strcapitalize. */
static void	testCapitalizeAllBytePairs(void)
{
    char	mine[3];
    char	ref[3];
    int		a;
    int		b;

    a = 1;
    while (a <= 255)
    {
        b = 1;
        while (b <= 255)
        {
            mine[0] = (char)a;
            mine[1] = (char)b;
            mine[2] = '\0';
            ref[0] = (char)a;
            ref[1] = (char)b;
            ref[2] = '\0';
            ft_strcapitalize(mine);
            refCapitalize(ref);
            TEST_ASSERT_EQUAL_MEMORY(ref, mine, 3);
            b++;
        }
        a++;
    }
}

/* Representative triples over letters/digits/separators/boundary chars, to
** confirm the separator state propagates correctly across word boundaries. */
static void	testCapitalizeRepresentativeTriples(void)
{
    char	alpha[] = "aAzZ09mM .-\n@[`{/:";
    char	mine[4];
    char	ref[4];
    int		n;
    int		i;
    int		j;
    int		k;

    n = (int)strlen(alpha);
    i = 0;
    while (i < n)
    {
        j = 0;
        while (j < n)
        {
            k = 0;
            while (k < n)
            {
                mine[0] = alpha[i];
                mine[1] = alpha[j];
                mine[2] = alpha[k];
                mine[3] = '\0';
                memcpy(ref, mine, 4);
                ft_strcapitalize(mine);
                refCapitalize(ref);
                TEST_ASSERT_EQUAL_MEMORY(ref, mine, 4);
                k++;
            }
            j++;
        }
        i++;
    }
}

/* A long mixed string is transformed identically to the reference. */
static void	testCapitalizeLongMixed(void)
{
    char	src[600];
    int		i;

    i = 0;
    while (i < 599)
    {
        if (i % 7 == 0)
            src[i] = ' ';
        else if (i % 5 == 0)
            src[i] = (char)('0' + (i % 10));
        else if (i % 2 == 0)
            src[i] = (char)('A' + (i % 26));
        else
            src[i] = (char)('a' + (i % 26));
        i++;
    }
    src[599] = '\0';
    assertCapitalize(src);
}

void	glorpRun(void)
{
    RUN_TEST(testCapitalizeBasic);
    RUN_TEST(testCapitalizeAllUppercase);
    RUN_TEST(testCapitalizeRaggedCase);
    RUN_TEST(testCapitalizeCanonicalExample);
    RUN_TEST(testCapitalizeWordStartingWithDigit);
    RUN_TEST(testCapitalizeDigitsInsideWord);
    RUN_TEST(testCapitalizeSingleChar);
    RUN_TEST(testCapitalizeEmpty);
    RUN_TEST(testCapitalizeLeadingAndTrailingSeparators);
    RUN_TEST(testCapitalizeVariousSeparators);
    RUN_TEST(testCapitalizeSingleByte);
    RUN_TEST(testCapitalizeAllBytePairs);
    RUN_TEST(testCapitalizeRepresentativeTriples);
    RUN_TEST(testCapitalizeLongMixed);
}
