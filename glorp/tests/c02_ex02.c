/*
** c02_ex02 -- ft_str_is_alpha: int ft_str_is_alpha(char *str);
** Returns 1 if str is empty or contains only letters (a-z, A-Z), 0 if it
** contains any non-letter byte. "Letter" is the strict ASCII a-z/A-Z range,
** not the locale-dependent isalpha().
*/

#include "runner.h"

extern int	ft_str_is_alpha(char *str);

/* Reference: 1 if every char is an ASCII letter (empty -> 1), else 0. */
static int	refIsAlpha(char *s)
{
    int		i;
    char	c;

    i = 0;
    while (s[i] != '\0')
    {
        c = s[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
            return (0);
        i++;
    }
    return (1);
}

/* Asserts ft_str_is_alpha matches the reference (which only ever returns 0/1,
** so this also pins the result to exactly 0 or 1). */
static void	assertIsAlpha(char *s)
{
    TEST_ASSERT_EQUAL_INT(refIsAlpha(s), ft_str_is_alpha(s));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The empty string is considered all-letters: returns 1. */
static void	testIsAlphaEmptyReturnsOne(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_alpha(""));
}

/* All lowercase. */
static void	testIsAlphaAllLowercase(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_alpha("abcxyz"));
}

/* All uppercase. */
static void	testIsAlphaAllUppercase(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_alpha("ABCXYZ"));
}

/* Mixed case is still all-letters. */
static void	testIsAlphaMixedCase(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_alpha("AbCdEfGh"));
}

/* A single letter. */
static void	testIsAlphaSingleLetter(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_alpha("q"));
}

/* A digit anywhere makes it non-alpha. */
static void	testIsAlphaWithDigit(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_alpha("abc1def"));
}

/* A space makes it non-alpha. */
static void	testIsAlphaWithSpace(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_alpha("ab cd"));
}

/* Punctuation makes it non-alpha. */
static void	testIsAlphaWithPunctuation(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_alpha("abc!"));
}

/* A control character makes it non-alpha. */
static void	testIsAlphaWithControlChar(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_alpha("abc\n"));
}

/* A string with no letters at all. */
static void	testIsAlphaNoLetters(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_alpha("12345"));
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_alpha("5"));
}

/* A single non-letter byte at every position is detected. */
static void	testIsAlphaDetectsNonLetterAtAnyPosition(void)
{
    char	s[7];
    int		pos;
    int		i;

    pos = 0;
    while (pos < 6)
    {
        i = 0;
        while (i < 6)
        {
            s[i] = 'a';
            i++;
        }
        s[6] = '\0';
        s[pos] = '1';
        TEST_ASSERT_EQUAL_INT(0, ft_str_is_alpha(s));
        pos++;
    }
}

/* Every byte value 1..255 as a one-char string must classify like the
** reference -- this pins down the a/z and A/Z boundaries ('@','[','`','{') and
** high bytes (0x80..0xFF). */
static void	testIsAlphaSingleByteClassification(void)
{
    char	s[2];
    int		c;

    s[1] = '\0';
    c = 1;
    while (c <= 255)
    {
        s[0] = (char)c;
        TEST_ASSERT_EQUAL_INT(refIsAlpha(s), ft_str_is_alpha(s));
        c++;
    }
}

/* The full alphabet, both cases, is all-letters. */
static void	testIsAlphaFullAlphabet(void)
{
    assertIsAlpha("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

void	glorpRun(void)
{
    RUN_TEST(testIsAlphaEmptyReturnsOne);
    RUN_TEST(testIsAlphaAllLowercase);
    RUN_TEST(testIsAlphaAllUppercase);
    RUN_TEST(testIsAlphaMixedCase);
    RUN_TEST(testIsAlphaSingleLetter);
    RUN_TEST(testIsAlphaWithDigit);
    RUN_TEST(testIsAlphaWithSpace);
    RUN_TEST(testIsAlphaWithPunctuation);
    RUN_TEST(testIsAlphaWithControlChar);
    RUN_TEST(testIsAlphaNoLetters);
    RUN_TEST(testIsAlphaDetectsNonLetterAtAnyPosition);
    RUN_TEST(testIsAlphaSingleByteClassification);
    RUN_TEST(testIsAlphaFullAlphabet);
}
