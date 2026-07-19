/*
** c11_ex03 -- ft_count_if: int ft_count_if(char **tab, int length,
** int (*f)(char *));
** Returns how many of the first length strings of tab make f return nonzero.
*/

#include "runner.h"

extern int	ft_count_if(char **tab, int length, int (*f)(char *));

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
static void	testCountIfZeroMatches(void)
{
    char	*tab[] = {"hello", "world", "zzz", NULL};

    TEST_ASSERT_EQUAL_INT(0, ft_count_if(tab, 3, containsA));
}

/* Only the satisfying strings are counted. */
static void	testCountIfSomeMatches(void)
{
    char	*tab[] = {"Abc", "def", "ghA", "jkl", "AAA", NULL};

    TEST_ASSERT_EQUAL_INT(3, ft_count_if(tab, 5, containsA));
}

/* Every string satisfies f -> length. */
static void	testCountIfAllMatch(void)
{
    char	*tab[] = {"AAA", "bAnAnA", "zA", NULL};

    TEST_ASSERT_EQUAL_INT(3, ft_count_if(tab, 3, containsA));
}

/* With length 0 nothing is inspected -> 0. */
static void	testCountIfLengthZeroReturnsZero(void)
{
    char	*tab[] = {"AAA", "bAnAnA", NULL};

    TEST_ASSERT_EQUAL_INT(0, ft_count_if(tab, 0, containsA));
}

void	glorpRun(void)
{
    RUN_TEST(testCountIfZeroMatches);
    RUN_TEST(testCountIfSomeMatches);
    RUN_TEST(testCountIfAllMatch);
    RUN_TEST(testCountIfLengthZeroReturnsZero);
}
