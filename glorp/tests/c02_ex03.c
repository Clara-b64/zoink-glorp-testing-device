/*
** c02_ex03 -- ft_str_is_numeric: int ft_str_is_numeric(char *str);
** Returns 1 if str is empty or contains only digits (0-9), 0 if it contains any
** non-digit byte. Only the digit characters count -- a sign or a '.' is not
** numeric here.
*/

#include "runner.h"

extern int	ft_str_is_numeric(char *str);

/* Reference: 1 if every char is an ASCII digit (empty -> 1), else 0. */
static int	refIsNumeric(char *s)
{
    int		i;
    char	c;

    i = 0;
    while (s[i] != '\0')
    {
        c = s[i];
        if (!(c >= '0' && c <= '9'))
            return (0);
        i++;
    }
    return (1);
}

/* Asserts ft_str_is_numeric matches the reference (which only returns 0/1, so
** this also pins the result to exactly 0 or 1). */
static void	assertIsNumeric(char *s)
{
    TEST_ASSERT_EQUAL_INT(refIsNumeric(s), ft_str_is_numeric(s));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The empty string is considered all-digits: returns 1. */
static void	testIsNumericEmptyReturnsOne(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_numeric(""));
}

/* All ten digits. */
static void	testIsNumericAllDigits(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_numeric("0123456789"));
}

/* A single digit. */
static void	testIsNumericSingleDigit(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_numeric("7"));
}

/* A long run of digits. */
static void	testIsNumericLongRun(void)
{
    assertIsNumeric("99999999990000000000123456789");
}

/* A letter anywhere makes it non-numeric. */
static void	testIsNumericWithLetter(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_numeric("123a456"));
}

/* A space makes it non-numeric. */
static void	testIsNumericWithSpace(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_numeric("12 34"));
}

/* A sign is not a digit, so "-5" / "+5" are non-numeric. */
static void	testIsNumericWithSign(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_numeric("-5"));
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_numeric("+5"));
}

/* A decimal point is not a digit. */
static void	testIsNumericWithDecimalPoint(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_numeric("3.14"));
}

/* A control character makes it non-numeric. */
static void	testIsNumericWithControlChar(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_numeric("123\n"));
}

/* A string with no digits at all. */
static void	testIsNumericNoDigits(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_numeric("abcde"));
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_numeric("!"));
}

/* A single non-digit byte at every position is detected. */
static void	testIsNumericDetectsNonDigitAtAnyPosition(void)
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
            s[i] = '5';
            i++;
        }
        s[6] = '\0';
        s[pos] = 'x';
        TEST_ASSERT_EQUAL_INT(0, ft_str_is_numeric(s));
        pos++;
    }
}

/* Every byte value 1..255 as a one-char string must classify like the
** reference -- this pins down the digit boundaries ('/' = 0x2F just below '0',
** ':' = 0x3A just above '9') and high bytes. */
static void	testIsNumericSingleByteClassification(void)
{
    char	s[2];
    int		c;

    s[1] = '\0';
    c = 1;
    while (c <= 255)
    {
        s[0] = (char)c;
        TEST_ASSERT_EQUAL_INT(refIsNumeric(s), ft_str_is_numeric(s));
        c++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testIsNumericEmptyReturnsOne);
    RUN_TEST(testIsNumericAllDigits);
    RUN_TEST(testIsNumericSingleDigit);
    RUN_TEST(testIsNumericLongRun);
    RUN_TEST(testIsNumericWithLetter);
    RUN_TEST(testIsNumericWithSpace);
    RUN_TEST(testIsNumericWithSign);
    RUN_TEST(testIsNumericWithDecimalPoint);
    RUN_TEST(testIsNumericWithControlChar);
    RUN_TEST(testIsNumericNoDigits);
    RUN_TEST(testIsNumericDetectsNonDigitAtAnyPosition);
    RUN_TEST(testIsNumericSingleByteClassification);
}
