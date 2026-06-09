/*
** c02_ex04 -- ft_str_is_lowercase: int ft_str_is_lowercase(char *str);
** Returns 1 if str is empty or contains only lowercase letters (a-z), 0
** otherwise. Uppercase letters count as non-lowercase.
*/

#include "runner.h"

extern int	ft_str_is_lowercase(char *str);

/* Reference: 1 if every char is in a-z (empty -> 1), else 0. */
static int	refIsLowercase(char *s)
{
    int		i;
    char	c;

    i = 0;
    while (s[i] != '\0')
    {
        c = s[i];
        if (!(c >= 'a' && c <= 'z'))
            return (0);
        i++;
    }
    return (1);
}

/* Asserts ft_str_is_lowercase matches the reference (which only returns 0/1, so
** this also pins the result to exactly 0 or 1). */
static void	assertIsLowercase(char *s)
{
    TEST_ASSERT_EQUAL_INT(refIsLowercase(s), ft_str_is_lowercase(s));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The empty string is considered all-lowercase: returns 1. */
static void	testIsLowercaseEmptyReturnsOne(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_lowercase(""));
}

/* The whole lowercase alphabet. */
static void	testIsLowercaseFullAlphabet(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_lowercase("abcdefghijklmnopqrstuvwxyz"));
}

/* A single lowercase letter. */
static void	testIsLowercaseSingleLetter(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_lowercase("m"));
}

/* Uppercase letters are NOT lowercase. */
static void	testIsLowercaseRejectsUppercase(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_lowercase("ABC"));
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_lowercase("A"));
}

/* A single uppercase letter mixed in makes it non-lowercase. */
static void	testIsLowercaseMixedCase(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_lowercase("abcDef"));
}

/* A digit makes it non-lowercase. */
static void	testIsLowercaseWithDigit(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_lowercase("abc1"));
}

/* A space makes it non-lowercase. */
static void	testIsLowercaseWithSpace(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_lowercase("ab cd"));
}

/* Punctuation makes it non-lowercase. */
static void	testIsLowercaseWithPunctuation(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_lowercase("abc!"));
}

/* A control character makes it non-lowercase. */
static void	testIsLowercaseWithControlChar(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_lowercase("abc\n"));
}

/* An uppercase letter at every position is detected. */
static void	testIsLowercaseDetectsUppercaseAtAnyPosition(void)
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
        s[pos] = 'A';
        TEST_ASSERT_EQUAL_INT(0, ft_str_is_lowercase(s));
        pos++;
    }
}

/* Every byte value 1..255 as a one-char string must classify like the
** reference -- this pins the boundaries ('`' = 0x60 just below 'a', '{' = 0x7B
** just above 'z'), the whole uppercase range, and high bytes. */
static void	testIsLowercaseSingleByteClassification(void)
{
    char	s[2];
    int		c;

    s[1] = '\0';
    c = 1;
    while (c <= 255)
    {
        s[0] = (char)c;
        TEST_ASSERT_EQUAL_INT(refIsLowercase(s), ft_str_is_lowercase(s));
        c++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testIsLowercaseEmptyReturnsOne);
    RUN_TEST(testIsLowercaseFullAlphabet);
    RUN_TEST(testIsLowercaseSingleLetter);
    RUN_TEST(testIsLowercaseRejectsUppercase);
    RUN_TEST(testIsLowercaseMixedCase);
    RUN_TEST(testIsLowercaseWithDigit);
    RUN_TEST(testIsLowercaseWithSpace);
    RUN_TEST(testIsLowercaseWithPunctuation);
    RUN_TEST(testIsLowercaseWithControlChar);
    RUN_TEST(testIsLowercaseDetectsUppercaseAtAnyPosition);
    RUN_TEST(testIsLowercaseSingleByteClassification);
}
