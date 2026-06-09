/*
** c01_ex02 -- ft_swap: void ft_swap(int *a, int *b);
** Exchanges the contents of the two ints pointed to by a and b.
*/

#include "runner.h"
#include <limits.h>

extern void	ft_swap(int *a, int *b);

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Two distinct values end up exchanged. */
static void	testSwapExchangesTwoValues(void)
{
    int	a;
    int	b;

    a = 3;
    b = 7;
    ft_swap(&a, &b);
    TEST_ASSERT_EQUAL_INT(7, a);
    TEST_ASSERT_EQUAL_INT(3, b);
}

/* Swapping with a zero on one side works (catches a missing temp). */
static void	testSwapWithZero(void)
{
    int	a;
    int	b;

    a = 0;
    b = 99;
    ft_swap(&a, &b);
    TEST_ASSERT_EQUAL_INT(99, a);
    TEST_ASSERT_EQUAL_INT(0, b);
}

/* Sign is carried across the swap. */
static void	testSwapNegativeAndPositive(void)
{
    int	a;
    int	b;

    a = -5;
    b = 10;
    ft_swap(&a, &b);
    TEST_ASSERT_EQUAL_INT(10, a);
    TEST_ASSERT_EQUAL_INT(-5, b);
}

/* Equal values stay equal. */
static void	testSwapEqualValues(void)
{
    int	a;
    int	b;

    a = 4;
    b = 4;
    ft_swap(&a, &b);
    TEST_ASSERT_EQUAL_INT(4, a);
    TEST_ASSERT_EQUAL_INT(4, b);
}

/* The int extremes swap cleanly. */
static void	testSwapIntExtremes(void)
{
    int	a;
    int	b;

    a = INT_MIN;
    b = INT_MAX;
    ft_swap(&a, &b);
    TEST_ASSERT_EQUAL_INT(INT_MAX, a);
    TEST_ASSERT_EQUAL_INT(INT_MIN, b);
}

/* Swapping a variable with itself leaves it unchanged (catches XOR swaps). */
static void	testSwapSamePointerKeepsValue(void)
{
    int	x;

    x = 42;
    ft_swap(&x, &x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* Only the two targets change; their neighbours are untouched. */
static void	testSwapDoesNotTouchNeighbours(void)
{
    int	arr[4];

    arr[0] = 7;
    arr[1] = 1;
    arr[2] = 2;
    arr[3] = 9;
    ft_swap(&arr[1], &arr[2]);
    TEST_ASSERT_EQUAL_INT(7, arr[0]);
    TEST_ASSERT_EQUAL_INT(2, arr[1]);
    TEST_ASSERT_EQUAL_INT(1, arr[2]);
    TEST_ASSERT_EQUAL_INT(9, arr[3]);
}

void	glorpRun(void)
{
    RUN_TEST(testSwapExchangesTwoValues);
    RUN_TEST(testSwapWithZero);
    RUN_TEST(testSwapNegativeAndPositive);
    RUN_TEST(testSwapEqualValues);
    RUN_TEST(testSwapIntExtremes);
    RUN_TEST(testSwapSamePointerKeepsValue);
    RUN_TEST(testSwapDoesNotTouchNeighbours);
}
