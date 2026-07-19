/*
** c11_ex06 -- ft_sort_string_tab: void ft_sort_string_tab(char **tab);
** Sorts, in place and in ascending strcmp order, the strings of a
** NULL-terminated array. Only the pointers move; the strings stay untouched.
*/

#include "runner.h"

extern void	ft_sort_string_tab(char **tab);

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A shuffled array (including a prefix pair) ends up in exact strcmp order. */
static void	testSortStringTabSortsShuffledArray(void)
{
    char	*tab[] = {"pear", "apple", "orange", "app", "banana", NULL};

    ft_sort_string_tab(tab);
    TEST_ASSERT_EQUAL_STRING("app", tab[0]);
    TEST_ASSERT_EQUAL_STRING("apple", tab[1]);
    TEST_ASSERT_EQUAL_STRING("banana", tab[2]);
    TEST_ASSERT_EQUAL_STRING("orange", tab[3]);
    TEST_ASSERT_EQUAL_STRING("pear", tab[4]);
    TEST_ASSERT_NULL(tab[5]);
}

/* Duplicate strings are all preserved. */
static void	testSortStringTabKeepsDuplicates(void)
{
    char	*tab[] = {"beta", "alpha", "beta", "alpha", NULL};

    ft_sort_string_tab(tab);
    TEST_ASSERT_EQUAL_STRING("alpha", tab[0]);
    TEST_ASSERT_EQUAL_STRING("alpha", tab[1]);
    TEST_ASSERT_EQUAL_STRING("beta", tab[2]);
    TEST_ASSERT_EQUAL_STRING("beta", tab[3]);
    TEST_ASSERT_NULL(tab[4]);
}

/* An already sorted array is left unchanged. */
static void	testSortStringTabAlreadySortedUnchanged(void)
{
    char	*tab[] = {"alpha", "beta", "gamma", NULL};

    ft_sort_string_tab(tab);
    TEST_ASSERT_EQUAL_STRING("alpha", tab[0]);
    TEST_ASSERT_EQUAL_STRING("beta", tab[1]);
    TEST_ASSERT_EQUAL_STRING("gamma", tab[2]);
    TEST_ASSERT_NULL(tab[3]);
}

/* A single-element array is handled and left unchanged. */
static void	testSortStringTabSingleElement(void)
{
    char	*tab[] = {"solo", NULL};

    ft_sort_string_tab(tab);
    TEST_ASSERT_EQUAL_STRING("solo", tab[0]);
    TEST_ASSERT_NULL(tab[1]);
}

void	glorpRun(void)
{
    RUN_TEST(testSortStringTabSortsShuffledArray);
    RUN_TEST(testSortStringTabKeepsDuplicates);
    RUN_TEST(testSortStringTabAlreadySortedUnchanged);
    RUN_TEST(testSortStringTabSingleElement);
}
