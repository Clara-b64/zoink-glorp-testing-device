/*
** c01_ex01 -- ft_ultimate_ft: void ft_ultimate_ft(int *********nbr);
** Sets the int reached through nine levels of indirection to 42. Same job as
** ft_ft, but nbr must be dereferenced nine times to reach the value.
*/

#include "runner.h"
#include <limits.h>

extern void	ft_ultimate_ft(int *********nbr);

/* Builds a full 9-level pointer chain that ultimately points at `target`, then
** calls ft_ultimate_ft. The intermediate pointers live for the whole call. */
static void	callUltimate(int *target)
{
    int			*p1;
    int			**p2;
    int			***p3;
    int			****p4;
    int			*****p5;
    int			******p6;
    int			*******p7;
    int			********p8;
    int			*********p9;

    p1 = target;
    p2 = &p1;
    p3 = &p2;
    p4 = &p3;
    p5 = &p4;
    p6 = &p5;
    p7 = &p6;
    p8 = &p7;
    p9 = &p8;
    ft_ultimate_ft(p9);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* From a fresh zero, the deeply-pointed int becomes 42. */
static void	testUltimateFtSetsValueToFortyTwo(void)
{
    int	x;

    x = 0;
    callUltimate(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* Any previous positive value is overwritten. */
static void	testUltimateFtOverwritesExistingValue(void)
{
    int	x;

    x = 99;
    callUltimate(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* A negative value is overwritten too. */
static void	testUltimateFtOverwritesNegativeValue(void)
{
    int	x;

    x = -12345;
    callUltimate(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* The int extremes are overwritten as well. */
static void	testUltimateFtOverwritesIntExtremes(void)
{
    int	x;

    x = INT_MAX;
    callUltimate(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
    x = INT_MIN;
    callUltimate(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* Setting an already-42 value keeps it 42. */
static void	testUltimateFtAlreadyFortyTwoStaysFortyTwo(void)
{
    int	x;

    x = 42;
    callUltimate(&x);
    TEST_ASSERT_EQUAL_INT(42, x);
}

/* Only the targeted int changes; its neighbours are left untouched. */
static void	testUltimateFtDoesNotTouchNeighbours(void)
{
    int	arr[3];

    arr[0] = 7;
    arr[1] = 0;
    arr[2] = 9;
    callUltimate(&arr[1]);
    TEST_ASSERT_EQUAL_INT(7, arr[0]);
    TEST_ASSERT_EQUAL_INT(42, arr[1]);
    TEST_ASSERT_EQUAL_INT(9, arr[2]);
}

void	glorpRun(void)
{
    RUN_TEST(testUltimateFtSetsValueToFortyTwo);
    RUN_TEST(testUltimateFtOverwritesExistingValue);
    RUN_TEST(testUltimateFtOverwritesNegativeValue);
    RUN_TEST(testUltimateFtOverwritesIntExtremes);
    RUN_TEST(testUltimateFtAlreadyFortyTwoStaysFortyTwo);
    RUN_TEST(testUltimateFtDoesNotTouchNeighbours);
}
