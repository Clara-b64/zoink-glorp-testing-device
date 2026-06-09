/*
** c01_ex06 -- ft_strlen: int ft_strlen(char *str);
** Returns the number of characters in str before the terminating '\0' (which is
** not counted). The standard strlen() is used as the oracle.
*/

#include "runner.h"
#include <string.h>

extern int	ft_strlen(char *str);

/* Asserts ft_strlen matches the standard strlen for the given string. */
static void	assertStrlen(char *str)
{
    TEST_ASSERT_EQUAL_INT((int)strlen(str), ft_strlen(str));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The empty string has length 0. */
static void	testStrlenEmpty(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_strlen(""));
}

/* A single character has length 1. */
static void	testStrlenSingleCharacter(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_strlen("a"));
}

/* A typical string. */
static void	testStrlenBasic(void)
{
    TEST_ASSERT_EQUAL_INT(13, ft_strlen("Hello, World!"));
    assertStrlen("Hello, World!");
}

/* Spaces are counted like any other character. */
static void	testStrlenCountsSpaces(void)
{
    assertStrlen("   ");
    assertStrlen(" leading and trailing ");
}

/* Embedded control characters are counted (they are not terminators). */
static void	testStrlenCountsControlChars(void)
{
    assertStrlen("a\nb\tc\rd");
}

/* Digits and punctuation. */
static void	testStrlenDigitsAndPunctuation(void)
{
    assertStrlen("0123456789");
    assertStrlen("!@#$%^&*()");
}

/* Counting stops at the first '\0', even if more bytes follow it. */
static void	testStrlenStopsAtFirstTerminator(void)
{
    char	buf[8];

    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    buf[3] = '\0';
    buf[4] = 'd';
    buf[5] = 'e';
    buf[6] = 'f';
    buf[7] = '\0';
    TEST_ASSERT_EQUAL_INT(3, ft_strlen(buf));
}

/* A long string. */
static void	testStrlenLongString(void)
{
    char	big[2001];
    int		i;

    i = 0;
    while (i < 2000)
    {
        big[i] = 'A' + (i % 26);
        i++;
    }
    big[2000] = '\0';
    TEST_ASSERT_EQUAL_INT(2000, ft_strlen(big));
}

void	glorpRun(void)
{
    RUN_TEST(testStrlenEmpty);
    RUN_TEST(testStrlenSingleCharacter);
    RUN_TEST(testStrlenBasic);
    RUN_TEST(testStrlenCountsSpaces);
    RUN_TEST(testStrlenCountsControlChars);
    RUN_TEST(testStrlenDigitsAndPunctuation);
    RUN_TEST(testStrlenStopsAtFirstTerminator);
    RUN_TEST(testStrlenLongString);
}
