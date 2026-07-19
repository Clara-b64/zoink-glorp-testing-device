/*
** c11_ex02 -- ft_any: int ft_any(char **tab, int (*f)(char *));
** tab is a NULL-terminated array of strings. Returns 1 if f returns a nonzero
** value for at least one string, 0 otherwise.
*/

#include "runner.h"

extern int	ft_any(char **tab, int (*f)(char *));

/* Returns 1 if str contains the character 'A', 0 otherwise. */
static int	containsA(char *str)
{
    int	i;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == 'A')
            return (1);
        i++;
    }
    return (0);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* No string satisfies f -> 0. */
static void	testAnyNoMatchReturnsZero(void)
{
    char	*tab[] = {"hello", "world", "zzz", NULL};

    TEST_ASSERT_EQUAL_INT(0, ft_any(tab, containsA));
}

/* Exactly one string satisfies f -> 1. */
static void	testAnyOneMatchReturnsOne(void)
{
    char	*tab[] = {"hello", "wArld", "zzz", NULL};

    TEST_ASSERT_EQUAL_INT(1, ft_any(tab, containsA));
}

/* Every string satisfies f -> 1. */
static void	testAnyAllMatchReturnsOne(void)
{
    char	*tab[] = {"AAA", "bAnAnA", NULL};

    TEST_ASSERT_EQUAL_INT(1, ft_any(tab, containsA));
}

/* An empty array (only the NULL terminator) -> 0. */
static void	testAnyEmptyTabReturnsZero(void)
{
    char	*tab[] = {NULL};

    TEST_ASSERT_EQUAL_INT(0, ft_any(tab, containsA));
}

void	glorpRun(void)
{
    RUN_TEST(testAnyNoMatchReturnsZero);
    RUN_TEST(testAnyOneMatchReturnsOne);
    RUN_TEST(testAnyAllMatchReturnsOne);
    RUN_TEST(testAnyEmptyTabReturnsZero);
}
