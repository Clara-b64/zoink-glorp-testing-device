/*
** c02_ex08 -- ft_strlowcase: char *ft_strlowcase(char *str);
** Lowercases every uppercase letter (A-Z -> a-z) in str, in place, leaving all
** other bytes untouched, and returns str.
**
** The helper runs an explicit-range reference on a parallel copy and compares
** the WHOLE buffers (sentinel-filled), verifying the transform, that non-letters
** are untouched, and that nothing past the terminator is written.
*/

#include "runner.h"
#include <string.h>

#define LOW_BUFSIZE 1024

extern char	*ft_strlowcase(char *str);

/* Reference: A-Z -> a-z in place, everything else unchanged. */
static void	refStrlowcase(char *s)
{
    int	i;

    i = 0;
    while (s[i] != '\0')
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = (char)(s[i] + 32);
        i++;
    }
}

/* Copies input into two sentinel-filled buffers, lowercases one with
** ft_strlowcase and one with the reference, and asserts full-buffer equality
** plus that ft_strlowcase returns its argument. */
static void	assertStrlowcase(char *input)
{
    char	mine[LOW_BUFSIZE];
    char	ref[LOW_BUFSIZE];
    char	*ret;
    int		len;

    len = (int)strlen(input);
    memset(mine, 'X', sizeof(mine));
    memset(ref, 'X', sizeof(ref));
    memcpy(mine, input, len + 1);
    memcpy(ref, input, len + 1);
    ret = ft_strlowcase(mine);
    refStrlowcase(ref);
    TEST_ASSERT_EQUAL_PTR(mine, ret);
    TEST_ASSERT_EQUAL_MEMORY(ref, mine, sizeof(mine));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* All uppercase becomes all lowercase. */
static void	testStrlowcaseAllUppercase(void)
{
    assertStrlowcase("HELLO");
}

/* Already-lowercase text is unchanged. */
static void	testStrlowcaseAlreadyLowercase(void)
{
    assertStrlowcase("hello");
}

/* Mixed case: only the uppercase letters move. */
static void	testStrlowcaseMixedCase(void)
{
    assertStrlowcase("Hello, World!");
}

/* Non-letters (digits, punctuation, spaces) are left untouched. */
static void	testStrlowcaseLeavesNonLetters(void)
{
    assertStrlowcase("ABC 123 !@# XYZ");
}

/* The empty string stays empty. */
static void	testStrlowcaseEmpty(void)
{
    assertStrlowcase("");
}

/* A single uppercase letter. */
static void	testStrlowcaseSingleUppercase(void)
{
    assertStrlowcase("A");
}

/* A single lowercase letter is unchanged. */
static void	testStrlowcaseSingleLowercase(void)
{
    assertStrlowcase("z");
}

/* The return value is the same pointer that was passed in. */
static void	testStrlowcaseReturnsArgument(void)
{
    char	buf[8];
    char	*ret;

    memcpy(buf, "ABC", 4);
    ret = ft_strlowcase(buf);
    TEST_ASSERT_EQUAL_PTR(buf, ret);
}

/* The chars just outside A-Z ('@' = 0x40, '[' = 0x5B) must NOT be changed. */
static void	testStrlowcaseBoundaryCharsUnchanged(void)
{
    char	buf[8];

    buf[0] = '@';
    buf[1] = 'A';
    buf[2] = 'Z';
    buf[3] = '[';
    buf[4] = '\0';
    ft_strlowcase(buf);
    TEST_ASSERT_EQUAL_CHAR('@', buf[0]);
    TEST_ASSERT_EQUAL_CHAR('a', buf[1]);
    TEST_ASSERT_EQUAL_CHAR('z', buf[2]);
    TEST_ASSERT_EQUAL_CHAR('[', buf[3]);
}

/* Every byte value 1..255 (as a one-char string) transforms like the
** reference -- pins the A/Z boundaries and leaves high bytes alone. */
static void	testStrlowcaseSingleByteTransform(void)
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
        ft_strlowcase(mine);
        refStrlowcase(ref);
        TEST_ASSERT_EQUAL_MEMORY(ref, mine, 2);
        c++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testStrlowcaseAllUppercase);
    RUN_TEST(testStrlowcaseAlreadyLowercase);
    RUN_TEST(testStrlowcaseMixedCase);
    RUN_TEST(testStrlowcaseLeavesNonLetters);
    RUN_TEST(testStrlowcaseEmpty);
    RUN_TEST(testStrlowcaseSingleUppercase);
    RUN_TEST(testStrlowcaseSingleLowercase);
    RUN_TEST(testStrlowcaseReturnsArgument);
    RUN_TEST(testStrlowcaseBoundaryCharsUnchanged);
    RUN_TEST(testStrlowcaseSingleByteTransform);
}
