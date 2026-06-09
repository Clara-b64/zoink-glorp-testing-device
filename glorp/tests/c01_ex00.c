/*
** c01_ex00 -- ft_ft: void ft_ft(int *nbr);
** Sets the int pointed to by nbr to 42, regardless of its previous value, and
** touches nothing else.
*/

#include "runner.h"
#include <limits.h>

extern void	ft_ft(int *nbr);

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* From a fresh zero, the value becomes 42. */
static void	testFtSetsValueToFortyTwo(void)
{
    int	x;

    x = 0;
    ft_ft(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* Any previous positive value is overwritten. */
static void	testFtOverwritesExistingValue(void)
{
    int	x;

    x = 99;
    ft_ft(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* A negative value is overwritten too. */
static void	testFtOverwritesNegativeValue(void)
{
    int	x;

    x = -12345;
    ft_ft(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* The int extremes are overwritten as well. */
static void	testFtOverwritesIntExtremes(void)
{
    int	x;

    x = INT_MAX;
    ft_ft(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
    x = INT_MIN;
    ft_ft(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* Setting an already-42 value keeps it 42. */
static void	testFtAlreadyFortyTwoStaysFortyTwo(void)
{
    int	x;

    x = 42;
    ft_ft(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* Only the targeted int changes; its neighbours are left untouched. */
static void	testFtDoesNotTouchNeighbours(void)
{
    int	arr[3];

    arr[0] = 7;
    arr[1] = 0;
    arr[2] = 9;
    ft_ft(&arr[1]);
    TEST_ASSERT_EQUAL_INT(7, arr[0]);
    TEST_ASSERT_EQUAL_INT(42, arr[1]);
    TEST_ASSERT_EQUAL_INT(9, arr[2]);
}

void	glorpRun(void)
{
    RUN_TEST(testFtSetsValueToFortyTwo);
    RUN_TEST(testFtOverwritesExistingValue);
    RUN_TEST(testFtOverwritesNegativeValue);
    RUN_TEST(testFtOverwritesIntExtremes);
    RUN_TEST(testFtAlreadyFortyTwoStaysFortyTwo);
    RUN_TEST(testFtDoesNotTouchNeighbours);
}
