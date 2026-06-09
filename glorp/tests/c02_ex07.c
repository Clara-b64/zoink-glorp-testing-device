/*
** c02_ex07 -- ft_strupcase: char *ft_strupcase(char *str);
** Uppercases every lowercase letter (a-z -> A-Z) in str, in place, leaving all
** other bytes untouched, and returns str.
**
** The helper runs an explicit-range reference on a parallel copy and compares
** the WHOLE buffers (sentinel-filled), verifying the transform, that non-letters
** are untouched, and that nothing past the terminator is written.
*/

#include "runner.h"
#include <string.h>

#define UP_BUFSIZE 1024

extern char	*ft_strupcase(char *str);

/* Reference: a-z -> A-Z in place, everything else unchanged. */
static void	refStrupcase(char *s)
{
    int	i;

    i = 0;
    while (s[i] != '\0')
    {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] = (char)(s[i] - 32);
        i++;
    }
}

/* Copies input into two sentinel-filled buffers, upcases one with ft_strupcase
** and one with the reference, and asserts full-buffer equality plus that
** ft_strupcase returns its argument. */
static void	assertStrupcase(char *input)
{
    char	mine[UP_BUFSIZE];
    char	ref[UP_BUFSIZE];
    char	*ret;
    int		len;

    len = (int)strlen(input);
    memset(mine, 'X', sizeof(mine));
    memset(ref, 'X', sizeof(ref));
    memcpy(mine, input, len + 1);
    memcpy(ref, input, len + 1);
    ret = ft_strupcase(mine);
    refStrupcase(ref);
    TEST_ASSERT_EQUAL_PTR(mine, ret);
    TEST_ASSERT_EQUAL_MEMORY(ref, mine, sizeof(mine));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* All lowercase becomes all uppercase. */
static void	testStrupcaseAllLowercase(void)
{
    assertStrupcase("hello");
}

/* Already-uppercase text is unchanged. */
static void	testStrupcaseAlreadyUppercase(void)
{
    assertStrupcase("HELLO");
}

/* Mixed case: only the lowercase letters move. */
static void	testStrupcaseMixedCase(void)
{
    assertStrupcase("Hello, World!");
}

/* Non-letters (digits, punctuation, spaces) are left untouched. */
static void	testStrupcaseLeavesNonLetters(void)
{
    assertStrupcase("abc 123 !@# xyz");
}

/* The empty string stays empty. */
static void	testStrupcaseEmpty(void)
{
    assertStrupcase("");
}

/* A single lowercase letter. */
static void	testStrupcaseSingleLowercase(void)
{
    assertStrupcase("a");
}

/* A single uppercase letter is unchanged. */
static void	testStrupcaseSingleUppercase(void)
{
    assertStrupcase("Z");
}

/* The return value is the same pointer that was passed in. */
static void	testStrupcaseReturnsArgument(void)
{
    char	buf[8];
    char	*ret;

    memcpy(buf, "abc", 4);
    ret = ft_strupcase(buf);
    TEST_ASSERT_EQUAL_PTR(buf, ret);
}

/* The chars just outside a-z ('`' = 0x60, '{' = 0x7B) must NOT be changed. */
static void	testStrupcaseBoundaryCharsUnchanged(void)
{
    char	buf[8];

    buf[0] = '`';
    buf[1] = 'a';
    buf[2] = 'z';
    buf[3] = '{';
    buf[4] = '\0';
    ft_strupcase(buf);
    TEST_ASSERT_EQUAL_CHAR('`', buf[0]);
    TEST_ASSERT_EQUAL_CHAR('A', buf[1]);
    TEST_ASSERT_EQUAL_CHAR('Z', buf[2]);
    TEST_ASSERT_EQUAL_CHAR('{', buf[3]);
}

/* Every byte value 1..255 (as a one-char string) transforms like the
** reference -- pins the a/z boundaries and leaves high bytes alone. */
static void	testStrupcaseSingleByteTransform(void)
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
        ft_strupcase(mine);
        refStrupcase(ref);
        TEST_ASSERT_EQUAL_MEMORY(ref, mine, 2);
        c++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testStrupcaseAllLowercase);
    RUN_TEST(testStrupcaseAlreadyUppercase);
    RUN_TEST(testStrupcaseMixedCase);
    RUN_TEST(testStrupcaseLeavesNonLetters);
    RUN_TEST(testStrupcaseEmpty);
    RUN_TEST(testStrupcaseSingleLowercase);
    RUN_TEST(testStrupcaseSingleUppercase);
    RUN_TEST(testStrupcaseReturnsArgument);
    RUN_TEST(testStrupcaseBoundaryCharsUnchanged);
    RUN_TEST(testStrupcaseSingleByteTransform);
}
