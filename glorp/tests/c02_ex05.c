/*
** c02_ex05 -- ft_str_is_uppercase: int ft_str_is_uppercase(char *str);
** Returns 1 if str is empty or contains only uppercase letters (A-Z), 0
** otherwise. Lowercase letters count as non-uppercase.
*/

#include "runner.h"

extern int	ft_str_is_uppercase(char *str);

/* Reference: 1 if every char is in A-Z (empty -> 1), else 0. */
static int	refIsUppercase(char *s)
{
    int		i;
    char	c;

    i = 0;
    while (s[i] != '\0')
    {
        c = s[i];
        if (!(c >= 'A' && c <= 'Z'))
            return (0);
        i++;
    }
    return (1);
}

/* Asserts ft_str_is_uppercase matches the reference (which only returns 0/1, so
** this also pins the result to exactly 0 or 1). */
static void	assertIsUppercase(char *s)
{
    TEST_ASSERT_EQUAL_INT(refIsUppercase(s), ft_str_is_uppercase(s));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The empty string is considered all-uppercase: returns 1. */
static void	testIsUppercaseEmptyReturnsOne(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_uppercase(""));
}

/* The whole uppercase alphabet. */
static void	testIsUppercaseFullAlphabet(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_uppercase("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
}

/* A single uppercase letter. */
static void	testIsUppercaseSingleLetter(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_uppercase("M"));
}

/* Lowercase letters are NOT uppercase. */
static void	testIsUppercaseRejectsLowercase(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_uppercase("abc"));
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_uppercase("a"));
}

/* A single lowercase letter mixed in makes it non-uppercase. */
static void	testIsUppercaseMixedCase(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_uppercase("ABCdEF"));
}

/* A digit makes it non-uppercase. */
static void	testIsUppercaseWithDigit(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_uppercase("ABC1"));
}

/* A space makes it non-uppercase. */
static void	testIsUppercaseWithSpace(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_uppercase("AB CD"));
}

/* Punctuation makes it non-uppercase. */
static void	testIsUppercaseWithPunctuation(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_uppercase("ABC!"));
}

/* A control character makes it non-uppercase. */
static void	testIsUppercaseWithControlChar(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_uppercase("ABC\n"));
}

/* A lowercase letter at every position is detected. */
static void	testIsUppercaseDetectsLowercaseAtAnyPosition(void)
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
            s[i] = 'A';
            i++;
        }
        s[6] = '\0';
        s[pos] = 'a';
        TEST_ASSERT_EQUAL_INT(0, ft_str_is_uppercase(s));
        pos++;
    }
}

/* Every byte value 1..255 as a one-char string must classify like the
** reference -- this pins the boundaries ('@' = 0x40 just below 'A', '[' = 0x5B
** just above 'Z'), the whole lowercase range, and high bytes. */
static void	testIsUppercaseSingleByteClassification(void)
{
    char	s[2];
    int		c;

    s[1] = '\0';
    c = 1;
    while (c <= 255)
    {
        s[0] = (char)c;
        TEST_ASSERT_EQUAL_INT(refIsUppercase(s), ft_str_is_uppercase(s));
        c++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testIsUppercaseEmptyReturnsOne);
    RUN_TEST(testIsUppercaseFullAlphabet);
    RUN_TEST(testIsUppercaseSingleLetter);
    RUN_TEST(testIsUppercaseRejectsLowercase);
    RUN_TEST(testIsUppercaseMixedCase);
    RUN_TEST(testIsUppercaseWithDigit);
    RUN_TEST(testIsUppercaseWithSpace);
    RUN_TEST(testIsUppercaseWithPunctuation);
    RUN_TEST(testIsUppercaseWithControlChar);
    RUN_TEST(testIsUppercaseDetectsLowercaseAtAnyPosition);
    RUN_TEST(testIsUppercaseSingleByteClassification);
}
