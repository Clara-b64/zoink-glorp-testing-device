/*
** c11_ex04 -- ft_is_sort: int ft_is_sort(int *tab, int length,
** int (*f)(int, int));
** Returns 1 if tab's length elements are sorted with respect to the comparison
** function f, 0 otherwise. Only arrays whose verdict is identical under every
** accepted reading of the subject are asserted (a descending array is never
** paired with an ascending comparator).
*/

#include "runner.h"

extern int	ft_is_sort(int *tab, int length, int (*f)(int, int));

/* Ascending comparator: negative when a < b, zero when equal. */
static int	cmpAscending(int a, int b)
{
    return (a - b);
}

/* Descending comparator: negative when a > b, zero when equal. */
static int	cmpDescending(int a, int b)
{
    return (b - a);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A strictly ascending array is sorted for the ascending comparator. */
static void	testIsSortStrictlyAscendingReturnsOne(void)
{
    int	tab[5] = {1, 2, 5, 9, 40};

    TEST_ASSERT_EQUAL_INT(1, ft_is_sort(tab, 5, cmpAscending));
}

/* Equal neighbors do not break sortedness. */
static void	testIsSortAscendingWithEqualNeighborsReturnsOne(void)
{
    int	tab[6] = {1, 2, 2, 3, 3, 7};

    TEST_ASSERT_EQUAL_INT(1, ft_is_sort(tab, 6, cmpAscending));
}

/* An all-equal array is sorted for any comparator. */
static void	testIsSortAllEqualReturnsOne(void)
{
    int	tab[4] = {4, 4, 4, 4};

    TEST_ASSERT_EQUAL_INT(1, ft_is_sort(tab, 4, cmpAscending));
    TEST_ASSERT_EQUAL_INT(1, ft_is_sort(tab, 4, cmpDescending));
}

/* An up-down-up array is unsorted whichever direction is considered. */
static void	testIsSortUnsortedReturnsZero(void)
{
    int	tab[4] = {1, 5, 2, 7};

    TEST_ASSERT_EQUAL_INT(0, ft_is_sort(tab, 4, cmpAscending));
    TEST_ASSERT_EQUAL_INT(0, ft_is_sort(tab, 4, cmpDescending));
}

/* A strictly descending array is sorted for the descending comparator. */
static void	testIsSortDescendingWithDescendingComparatorReturnsOne(void)
{
    int	tab[4] = {9, 7, 4, 1};

    TEST_ASSERT_EQUAL_INT(1, ft_is_sort(tab, 4, cmpDescending));
}

/* A single element is always sorted. */
static void	testIsSortSingleElementReturnsOne(void)
{
    int	tab[1] = {42};

    TEST_ASSERT_EQUAL_INT(1, ft_is_sort(tab, 1, cmpAscending));
    TEST_ASSERT_EQUAL_INT(1, ft_is_sort(tab, 1, cmpDescending));
}

void	glorpRun(void)
{
    RUN_TEST(testIsSortStrictlyAscendingReturnsOne);
    RUN_TEST(testIsSortAscendingWithEqualNeighborsReturnsOne);
    RUN_TEST(testIsSortAllEqualReturnsOne);
    RUN_TEST(testIsSortUnsortedReturnsZero);
    RUN_TEST(testIsSortDescendingWithDescendingComparatorReturnsOne);
    RUN_TEST(testIsSortSingleElementReturnsOne);
}
