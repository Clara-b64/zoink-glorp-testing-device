/*
** c02_ex06 -- ft_str_is_printable: int ft_str_is_printable(char *str);
** Returns 1 if str is empty or contains only printable characters (0x20 space
** through 0x7E '~'), 0 otherwise. Control chars, DEL (0x7F) and high bytes are
** not printable.
*/

#include "runner.h"

extern int	ft_str_is_printable(char *str);

/* Reference: 1 if every char is in ' '..'~' (empty -> 1), else 0. Uses signed
** char like the function, so high bytes (negative) are non-printable. */
static int	refIsPrintable(char *s)
{
    int		i;
    char	c;

    i = 0;
    while (s[i] != '\0')
    {
        c = s[i];
        if (!(c >= ' ' && c <= '~'))
            return (0);
        i++;
    }
    return (1);
}

/* Asserts ft_str_is_printable matches the reference (which only returns 0/1, so
** this also pins the result to exactly 0 or 1). */
static void	assertIsPrintable(char *s)
{
    TEST_ASSERT_EQUAL_INT(refIsPrintable(s), ft_str_is_printable(s));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The empty string is considered all-printable: returns 1. */
static void	testIsPrintableEmptyReturnsOne(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_printable(""));
}

/* A typical printable string. */
static void	testIsPrintableTypical(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_printable("Hello, World! 123 ~"));
}

/* The boundary characters themselves are printable. */
static void	testIsPrintableBoundaryChars(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_printable(" "));
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_printable("~"));
}

/* The whole printable range (0x20..0x7E) is printable. */
static void	testIsPrintableFullRange(void)
{
    char	all[96];
    int		i;

    i = 0;
    while (i < 95)
    {
        all[i] = (char)(' ' + i);
        i++;
    }
    all[95] = '\0';
    TEST_ASSERT_EQUAL_INT(1, ft_str_is_printable(all));
}

/* A newline is not printable. */
static void	testIsPrintableWithNewline(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_printable("abc\ndef"));
}

/* A tab is not printable. */
static void	testIsPrintableWithTab(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_printable("abc\tdef"));
}

/* DEL (0x7F) is not printable -- just above the '~' boundary. */
static void	testIsPrintableWithDel(void)
{
    char	s[4];

    s[0] = 'a';
    s[1] = (char)0x7F;
    s[2] = 'b';
    s[3] = '\0';
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_printable(s));
}

/* A high byte (>= 0x80) is not printable. */
static void	testIsPrintableWithHighByte(void)
{
    char	s[4];

    s[0] = 'a';
    s[1] = (char)0xC3;
    s[2] = 'b';
    s[3] = '\0';
    TEST_ASSERT_EQUAL_INT(0, ft_str_is_printable(s));
}

/* A non-printable byte at every position is detected. */
static void	testIsPrintableDetectsNonPrintableAtAnyPosition(void)
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
        s[pos] = '\n';
        TEST_ASSERT_EQUAL_INT(0, ft_str_is_printable(s));
        pos++;
    }
}

/* Every byte value 1..255 as a one-char string must classify like the
** reference -- pins the boundaries (0x1F below ' ', 0x7F above '~') and high
** bytes. */
static void	testIsPrintableSingleByteClassification(void)
{
    char	s[2];
    int		c;

    s[1] = '\0';
    c = 1;
    while (c <= 255)
    {
        s[0] = (char)c;
        TEST_ASSERT_EQUAL_INT(refIsPrintable(s), ft_str_is_printable(s));
        c++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testIsPrintableEmptyReturnsOne);
    RUN_TEST(testIsPrintableTypical);
    RUN_TEST(testIsPrintableBoundaryChars);
    RUN_TEST(testIsPrintableFullRange);
    RUN_TEST(testIsPrintableWithNewline);
    RUN_TEST(testIsPrintableWithTab);
    RUN_TEST(testIsPrintableWithDel);
    RUN_TEST(testIsPrintableWithHighByte);
    RUN_TEST(testIsPrintableDetectsNonPrintableAtAnyPosition);
    RUN_TEST(testIsPrintableSingleByteClassification);
}
