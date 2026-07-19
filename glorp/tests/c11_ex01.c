/*
** c11_ex01 -- ft_map: int *ft_map(int *tab, int length, int (*f)(int));
** Returns a newly allocated array of length ints where element i holds
** f(tab[i]). The caller owns (and here always frees) the result.
*/

#include "runner.h"
#include <stdlib.h>

extern int	*ft_map(int *tab, int length, int (*f)(int));

static int	square(int n)
{
    return (n * n);
}

static int	negate(int n)
{
    return (-n);
}

static int	addOne(int n)
{
    return (n + 1);
}

/* Copies count ints from result into copy, then frees result. Copying before
** asserting keeps the test leak-free even when an assertion fails. */
static void	copyAndFree(int *result, int *copy, int count)
{
    int	i;

    i = 0;
    while (i < count)
    {
        copy[i] = result[i];
        i++;
    }
    free(result);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Each slot of the result holds f(tab[i]) with f = square. */
static void	testMapSquareValues(void)
{
    int	tab[5] = {1, 2, 3, 4, 5};
    int	expected[5] = {1, 4, 9, 16, 25};
    int	copy[5];
    int	*result;

    result = ft_map(tab, 5, square);
    TEST_ASSERT_NOT_NULL(result);
    copyAndFree(result, copy, 5);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, copy, 5);
}

/* Each slot of the result holds f(tab[i]) with f = negate. */
static void	testMapNegateValues(void)
{
    int	tab[4] = {-7, 0, 13, 42};
    int	expected[4] = {7, 0, -13, -42};
    int	copy[4];
    int	*result;

    result = ft_map(tab, 4, negate);
    TEST_ASSERT_NOT_NULL(result);
    copyAndFree(result, copy, 4);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, copy, 4);
}

/* Results come back in the same order as the input elements. */
static void	testMapAddOneKeepsOrder(void)
{
    int	tab[3] = {10, -10, 0};
    int	expected[3] = {11, -9, 1};
    int	copy[3];
    int	*result;

    result = ft_map(tab, 3, addOne);
    TEST_ASSERT_NOT_NULL(result);
    copyAndFree(result, copy, 3);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, copy, 3);
}

/* The result is a distinct allocation: writing to it leaves tab untouched. */
static void	testMapResultIsDistinctAllocation(void)
{
    int	tab[3] = {7, 8, 9};
    int	*result;

    result = ft_map(tab, 3, addOne);
    TEST_ASSERT_NOT_NULL(result);
    result[0] = 12345;
    result[1] = 12346;
    result[2] = 12347;
    free(result);
    TEST_ASSERT_EQUAL_INT(7, tab[0]);
    TEST_ASSERT_EQUAL_INT(8, tab[1]);
    TEST_ASSERT_EQUAL_INT(9, tab[2]);
}

void	glorpRun(void)
{
    RUN_TEST(testMapSquareValues);
    RUN_TEST(testMapNegateValues);
    RUN_TEST(testMapAddOneKeepsOrder);
    RUN_TEST(testMapResultIsDistinctAllocation);
}
