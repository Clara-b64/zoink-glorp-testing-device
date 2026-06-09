/*
** c01_ex08 -- ft_sort_int_tab: void ft_sort_int_tab(int *tab, int size);
** Sorts the first `size` elements of tab into ascending order, in place.
**
** A correct sort must satisfy TWO properties, not one:
**   1. the result is non-decreasing, and
**   2. the result is a permutation of the input (no value lost or invented).
** Checking only (1) would pass a function that, say, zeroes the array. The main
** helper checks both at once by comparing the result to a trusted qsort of the
** same input -- the unique sorted permutation -- and there is also an explicit
** multiset/permutation test.
*/

#include "runner.h"
#include <limits.h>
#include <stdlib.h>

#define ARR_MAX 1024

extern void	ft_sort_int_tab(int *tab, int size);

/* Trusted comparator (overflow-safe -- no a-b subtraction). */
static int	cmpInt(const void *x, const void *y)
{
    int	a;
    int	b;

    a = *(const int *)x;
    b = *(const int *)y;
    if (a < b)
        return (-1);
    if (a > b)
        return (1);
    return (0);
}

/* Counts how many times `value` appears in the first `size` ints of arr. */
static int	countOccurrences(int *arr, int size, int value)
{
    int	i;
    int	count;

    i = 0;
    count = 0;
    while (i < size)
    {
        if (arr[i] == value)
            count++;
        i++;
    }
    return (count);
}

/* Sorts a copy of `input` with ft_sort_int_tab and asserts it equals a trusted
** qsort of the same data -- which proves both "ascending" and "permutation". */
static void	assertSorts(int *input, int size)
{
    int	mine[ARR_MAX];
    int	ref[ARR_MAX];
    int	i;

    i = 0;
    while (i < size)
    {
        mine[i] = input[i];
        ref[i] = input[i];
        i++;
    }
    ft_sort_int_tab(mine, size);
    qsort(ref, size, sizeof(int), cmpInt);
    i = 0;
    while (i < size)
    {
        TEST_ASSERT_EQUAL_INT(ref[i], mine[i]);
        if (i > 0)
            TEST_ASSERT_TRUE(mine[i - 1] <= mine[i]);
        i++;
    }
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* size 0: nothing happens and it must not crash. */
static void	testSortEmpty(void)
{
    int	tab[3];

    tab[0] = 9;
    tab[1] = 8;
    tab[2] = 7;
    ft_sort_int_tab(tab, 0);
    TEST_ASSERT_EQUAL_INT(9, tab[0]);
    TEST_ASSERT_EQUAL_INT(8, tab[1]);
    TEST_ASSERT_EQUAL_INT(7, tab[2]);
}

/* A single element is already sorted. */
static void	testSortSingleElement(void)
{
    int	tab[1];

    tab[0] = 42;
    ft_sort_int_tab(tab, 1);
    TEST_ASSERT_EQUAL_INT(42, tab[0]);
}

/* Two elements out of order get swapped. */
static void	testSortTwoElementsUnsorted(void)
{
    int	tab[2];

    tab[0] = 2;
    tab[1] = 1;
    assertSorts(tab, 2);
}

/* Two elements already in order stay put. */
static void	testSortTwoElementsSorted(void)
{
    int	tab[2];

    tab[0] = 1;
    tab[1] = 2;
    assertSorts(tab, 2);
}

/* An already-sorted array stays sorted. */
static void	testSortAlreadySorted(void)
{
    int	tab[7];
    int	i;

    i = 0;
    while (i < 7)
    {
        tab[i] = i * 2 - 3;
        i++;
    }
    assertSorts(tab, 7);
}

/* A reverse-sorted array (the worst case for many simple sorts). */
static void	testSortReverseSorted(void)
{
    int	tab[8];
    int	i;

    i = 0;
    while (i < 8)
    {
        tab[i] = 100 - i * 10;
        i++;
    }
    assertSorts(tab, 8);
}

/* A small unsorted array. */
static void	testSortUnsortedSmall(void)
{
    int	tab[8];

    tab[0] = 3;
    tab[1] = 1;
    tab[2] = 4;
    tab[3] = 1;
    tab[4] = 5;
    tab[5] = 9;
    tab[6] = 2;
    tab[7] = 6;
    assertSorts(tab, 8);
}

/* Duplicates are preserved (not collapsed). */
static void	testSortWithDuplicates(void)
{
    int	tab[6];

    tab[0] = 5;
    tab[1] = 1;
    tab[2] = 5;
    tab[3] = 1;
    tab[4] = 5;
    tab[5] = 1;
    assertSorts(tab, 6);
}

/* All elements equal: stays a flat run of the same value. */
static void	testSortAllEqual(void)
{
    int	tab[5];

    tab[0] = 7;
    tab[1] = 7;
    tab[2] = 7;
    tab[3] = 7;
    tab[4] = 7;
    assertSorts(tab, 5);
}

/* Negative numbers sort correctly. */
static void	testSortNegatives(void)
{
    int	tab[5];

    tab[0] = -3;
    tab[1] = -1;
    tab[2] = -2;
    tab[3] = -5;
    tab[4] = -4;
    assertSorts(tab, 5);
}

/* A mix of negatives, zero and positives. */
static void	testSortMixedSigns(void)
{
    int	tab[7];

    tab[0] = 0;
    tab[1] = -10;
    tab[2] = 10;
    tab[3] = -20;
    tab[4] = 20;
    tab[5] = 5;
    tab[6] = -5;
    assertSorts(tab, 7);
}

/* The int extremes, with duplicates, sort without overflow tricks. */
static void	testSortIntExtremes(void)
{
    int	tab[6];

    tab[0] = INT_MAX;
    tab[1] = INT_MIN;
    tab[2] = 0;
    tab[3] = INT_MAX;
    tab[4] = INT_MIN;
    tab[5] = 1;
    assertSorts(tab, 6);
}

/* Explicit permutation check: every value appears the same number of times
** before and after sorting (nothing dropped, duplicated or invented). */
static void	testSortIsPermutationOfInput(void)
{
    int	input[8];
    int	sorted[8];
    int	i;

    input[0] = 5;
    input[1] = 3;
    input[2] = 8;
    input[3] = 3;
    input[4] = 1;
    input[5] = 8;
    input[6] = 5;
    input[7] = 3;
    i = 0;
    while (i < 8)
    {
        sorted[i] = input[i];
        i++;
    }
    ft_sort_int_tab(sorted, 8);
    i = 0;
    while (i < 8)
    {
        TEST_ASSERT_EQUAL_INT(countOccurrences(input, 8, input[i]),
            countOccurrences(sorted, 8, input[i]));
        i++;
    }
}

/* Only the first `size` elements are touched; the tail is left intact. */
static void	testSortDoesNotTouchBeyondSize(void)
{
    int	tab[6];

    tab[0] = 5;
    tab[1] = 3;
    tab[2] = 1;
    tab[3] = 77;
    tab[4] = 88;
    tab[5] = 99;
    ft_sort_int_tab(tab, 3);
    TEST_ASSERT_EQUAL_INT(1, tab[0]);
    TEST_ASSERT_EQUAL_INT(3, tab[1]);
    TEST_ASSERT_EQUAL_INT(5, tab[2]);
    TEST_ASSERT_EQUAL_INT(77, tab[3]);
    TEST_ASSERT_EQUAL_INT(88, tab[4]);
    TEST_ASSERT_EQUAL_INT(99, tab[5]);
}

/* Sorting an already-sorted array again changes nothing. */
static void	testSortIsIdempotent(void)
{
    int	tab[8];
    int	first[8];
    int	i;

    tab[0] = 4;
    tab[1] = -2;
    tab[2] = 9;
    tab[3] = 0;
    tab[4] = 4;
    tab[5] = -7;
    tab[6] = 3;
    tab[7] = 1;
    ft_sort_int_tab(tab, 8);
    i = 0;
    while (i < 8)
    {
        first[i] = tab[i];
        i++;
    }
    ft_sort_int_tab(tab, 8);
    i = 0;
    while (i < 8)
    {
        TEST_ASSERT_EQUAL_INT(first[i], tab[i]);
        i++;
    }
}

/* A large pseudo-random array (deterministic LCG, signed values). */
static void	testSortLargePseudoRandom(void)
{
    int				tab[500];
    unsigned int	seed;
    int				i;

    seed = 123456789u;
    i = 0;
    while (i < 500)
    {
        seed = seed * 1103515245u + 12345u;
        tab[i] = (int)seed;
        i++;
    }
    assertSorts(tab, 500);
}

/* A sawtooth pattern with a long plateau in the middle. */
static void	testSortSawtoothWithPlateau(void)
{
    int	tab[12];

    tab[0] = 3;
    tab[1] = 1;
    tab[2] = 4;
    tab[3] = 4;
    tab[4] = 4;
    tab[5] = 4;
    tab[6] = 2;
    tab[7] = 0;
    tab[8] = 4;
    tab[9] = 1;
    tab[10] = 4;
    tab[11] = 2;
    assertSorts(tab, 12);
}

void	glorpRun(void)
{
    RUN_TEST(testSortEmpty);
    RUN_TEST(testSortSingleElement);
    RUN_TEST(testSortTwoElementsUnsorted);
    RUN_TEST(testSortTwoElementsSorted);
    RUN_TEST(testSortAlreadySorted);
    RUN_TEST(testSortReverseSorted);
    RUN_TEST(testSortUnsortedSmall);
    RUN_TEST(testSortWithDuplicates);
    RUN_TEST(testSortAllEqual);
    RUN_TEST(testSortNegatives);
    RUN_TEST(testSortMixedSigns);
    RUN_TEST(testSortIntExtremes);
    RUN_TEST(testSortIsPermutationOfInput);
    RUN_TEST(testSortDoesNotTouchBeyondSize);
    RUN_TEST(testSortIsIdempotent);
    RUN_TEST(testSortLargePseudoRandom);
    RUN_TEST(testSortSawtoothWithPlateau);
}
