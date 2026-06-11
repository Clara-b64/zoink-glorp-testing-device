/*
** c08_ex01 -- ft_boolean.h: the deliverable is a HEADER, not a function. It must
** define EVEN(nbr) (truthy for an even number), TRUE / FALSE, the type t_bool,
** SUCCESS (0), and the messages EVEN_MSG / ODD_MSG, so the provided program
** prints the right line for its argument count.
**
** Because the header differs per piscine, this test is NOT linked against a
** prebuilt archive: the root makefile compiles it together with glorp/shared and
** -I<piscine ex01 dir>, so #include "ft_boolean.h" resolves to that piscine's
** header. It checks the macros/constants directly and replays the program's
** even/odd decision; it never compiles the provided ft_boolean.c (its main).
*/

#include "runner.h"
#include "ft_boolean.h"

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* EVEN(n) is truthy for an even n, falsy for an odd one. */
static void	testEvenMacro(void)
{
    TEST_ASSERT_TRUE(EVEN(0));
    TEST_ASSERT_TRUE(EVEN(2));
    TEST_ASSERT_TRUE(EVEN(-4));
    TEST_ASSERT_TRUE(EVEN(100));
    TEST_ASSERT_FALSE(EVEN(1));
    TEST_ASSERT_FALSE(EVEN(3));
    TEST_ASSERT_FALSE(EVEN(-5));
    TEST_ASSERT_FALSE(EVEN(99));
}

/* TRUE and FALSE are distinct; TRUE is truthy, FALSE is falsy. */
static void	testTrueFalse(void)
{
    TEST_ASSERT_TRUE(TRUE);
    TEST_ASSERT_FALSE(FALSE);
    TEST_ASSERT_TRUE(TRUE != FALSE);
}

/* SUCCESS is the zero exit code main returns. */
static void	testSuccess(void)
{
    TEST_ASSERT_EQUAL_INT(0, SUCCESS);
}

/* t_bool can hold the boolean values. */
static void	testTbool(void)
{
    t_bool	yes;
    t_bool	no;

    yes = TRUE;
    no = FALSE;
    TEST_ASSERT_TRUE(yes == TRUE);
    TEST_ASSERT_TRUE(no == FALSE);
    TEST_ASSERT_TRUE(yes != no);
}

/* The exact messages, newline included. */
static void	testMessages(void)
{
    TEST_ASSERT_EQUAL_STRING("I have an even number of arguments.\n", EVEN_MSG);
    TEST_ASSERT_EQUAL_STRING("I have an odd number of arguments.\n", ODD_MSG);
}

/* Replays the program's decision: even count -> EVEN_MSG, odd -> ODD_MSG, exactly
** as `if (ft_is_even(count) == TRUE) ft_putstr(EVEN_MSG) else ft_putstr(ODD_MSG)`
** with ft_is_even being `(EVEN(nbr)) ? TRUE : FALSE`. */
static void	assertChoice(int count, char *expected)
{
    char	*msg;

    if (((EVEN(count)) ? TRUE : FALSE) == TRUE)
        msg = EVEN_MSG;
    else
        msg = ODD_MSG;
    TEST_ASSERT_EQUAL_STRING(expected, msg);
}

static void	testProgramChoice(void)
{
    assertChoice(0, "I have an even number of arguments.\n");
    assertChoice(1, "I have an odd number of arguments.\n");
    assertChoice(2, "I have an even number of arguments.\n");
    assertChoice(3, "I have an odd number of arguments.\n");
    assertChoice(10, "I have an even number of arguments.\n");
    assertChoice(7, "I have an odd number of arguments.\n");
}

void	glorpRun(void)
{
    RUN_TEST(testEvenMacro);
    RUN_TEST(testTrueFalse);
    RUN_TEST(testSuccess);
    RUN_TEST(testTbool);
    RUN_TEST(testMessages);
    RUN_TEST(testProgramChoice);
}
