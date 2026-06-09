/*
** c01_ex07 -- ft_rev_int_tab: void ft_rev_int_tab(int *tab, int size);
** Reverses the order of the first `size` elements of tab, in place.
*/

#include "runner.h"
#include <limits.h>

extern void	ft_rev_int_tab(int *tab, int size);

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* An odd-length array is reversed (the middle element stays put). */
static void	testRevOddLength(void)
{
    int	tab[5];
    int	expected[5];
    int	i;

    tab[0] = 1;
    tab[1] = 2;
    tab[2] = 3;
    tab[3] = 4;
    tab[4] = 5;
    expected[0] = 5;
    expected[1] = 4;
    expected[2] = 3;
    expected[3] = 2;
    expected[4] = 1;
    ft_rev_int_tab(tab, 5);
    i = 0;
    while (i < 5)
    {
        TEST_ASSERT_EQUAL_INT(expected[i], tab[i]);
        i++;
    }
}

/* An even-length array is reversed. */
static void	testRevEvenLength(void)
{
    int	tab[4];

    tab[0] = 1;
    tab[1] = 2;
    tab[2] = 3;
    tab[3] = 4;
    ft_rev_int_tab(tab, 4);
    TEST_ASSERT_EQUAL_INT(4, tab[0]);
    TEST_ASSERT_EQUAL_INT(3, tab[1]);
    TEST_ASSERT_EQUAL_INT(2, tab[2]);
    TEST_ASSERT_EQUAL_INT(1, tab[3]);
}

/* Two elements are swapped. */
static void	testRevTwoElements(void)
{
    int	tab[2];

    tab[0] = 7;
    tab[1] = 9;
    ft_rev_int_tab(tab, 2);
    TEST_ASSERT_EQUAL_INT(9, tab[0]);
    TEST_ASSERT_EQUAL_INT(7, tab[1]);
}

/* A single element is unchanged. */
static void	testRevSingleElement(void)
{
    int	tab[1];

    tab[0] = 42;
    ft_rev_int_tab(tab, 1);
    TEST_ASSERT_EQUAL_INT(42, tab[0]);
}

/* size 0 changes nothing and must not crash. */
static void	testRevSizeZeroDoesNothing(void)
{
    int	tab[3];

    tab[0] = 1;
    tab[1] = 2;
    tab[2] = 3;
    ft_rev_int_tab(tab, 0);
    TEST_ASSERT_EQUAL_INT(1, tab[0]);
    TEST_ASSERT_EQUAL_INT(2, tab[1]);
    TEST_ASSERT_EQUAL_INT(3, tab[2]);
}

/* Negative values and the int extremes are reversed like any others. */
static void	testRevNegativesAndExtremes(void)
{
    int	tab[4];

    tab[0] = INT_MIN;
    tab[1] = -1;
    tab[2] = 0;
    tab[3] = INT_MAX;
    ft_rev_int_tab(tab, 4);
    TEST_ASSERT_EQUAL_INT(INT_MAX, tab[0]);
    TEST_ASSERT_EQUAL_INT(0, tab[1]);
    TEST_ASSERT_EQUAL_INT(-1, tab[2]);
    TEST_ASSERT_EQUAL_INT(INT_MIN, tab[3]);
}

/* Only the first `size` elements are touched; the tail is left intact. */
static void	testRevDoesNotTouchBeyondSize(void)
{
    int	tab[6];

    tab[0] = 1;
    tab[1] = 2;
    tab[2] = 3;
    tab[3] = 4;
    tab[4] = 5;
    tab[5] = 6;
    ft_rev_int_tab(tab, 3);
    TEST_ASSERT_EQUAL_INT(3, tab[0]);
    TEST_ASSERT_EQUAL_INT(2, tab[1]);
    TEST_ASSERT_EQUAL_INT(1, tab[2]);
    TEST_ASSERT_EQUAL_INT(4, tab[3]);
    TEST_ASSERT_EQUAL_INT(5, tab[4]);
    TEST_ASSERT_EQUAL_INT(6, tab[5]);
}

/* A larger array: tab[i] must end up as the original tab[size-1-i]. */
static void	testRevLargeArrayMatchesOracle(void)
{
    int	tab[100];
    int	i;

    i = 0;
    while (i < 100)
    {
        tab[i] = i * 3 - 50;
        i++;
    }
    ft_rev_int_tab(tab, 100);
    i = 0;
    while (i < 100)
    {
        TEST_ASSERT_EQUAL_INT((99 - i) * 3 - 50, tab[i]);
        i++;
    }
}

/* Reversing twice restores the original order. */
static void	testRevTwiceRestoresOriginal(void)
{
    int	tab[7];
    int	i;

    i = 0;
    while (i < 7)
    {
        tab[i] = i * i - 10;
        i++;
    }
    ft_rev_int_tab(tab, 7);
    ft_rev_int_tab(tab, 7);
    i = 0;
    while (i < 7)
    {
        TEST_ASSERT_EQUAL_INT(i * i - 10, tab[i]);
        i++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testRevOddLength);
    RUN_TEST(testRevEvenLength);
    RUN_TEST(testRevTwoElements);
    RUN_TEST(testRevSingleElement);
    RUN_TEST(testRevSizeZeroDoesNothing);
    RUN_TEST(testRevNegativesAndExtremes);
    RUN_TEST(testRevDoesNotTouchBeyondSize);
    RUN_TEST(testRevLargeArrayMatchesOracle);
    RUN_TEST(testRevTwiceRestoresOriginal);
}
