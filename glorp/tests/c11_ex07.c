/*
** c11_ex07 -- ft_advanced_sort_string_tab:
** void ft_advanced_sort_string_tab(char **tab, int (*cmp)(char *, char *));
** Sorts, in place, the strings of a NULL-terminated array so that
** cmp(tab[i], tab[i + 1]) <= 0 holds for every adjacent pair.
*/

#include "runner.h"
#include <string.h>

extern void	ft_advanced_sort_string_tab(char **tab,
                int (*cmp)(char *, char *));

/* strcmp wrapper with the subject's non-const signature. */
static int	cmpAscending(char *a, char *b)
{
    return (strcmp(a, b));
}

/* Reversed strcmp: orders strings in descending order. */
static int	cmpDescending(char *a, char *b)
{
    return (strcmp(b, a));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* With a strcmp wrapper the array ends up in exact ascending order. */
static void	testAdvancedSortAscendingOrder(void)
{
    char	*tab[] = {"delta", "alpha", "echo", "charlie", "bravo", NULL};

    ft_advanced_sort_string_tab(tab, cmpAscending);
    TEST_ASSERT_EQUAL_STRING("alpha", tab[0]);
    TEST_ASSERT_EQUAL_STRING("bravo", tab[1]);
    TEST_ASSERT_EQUAL_STRING("charlie", tab[2]);
    TEST_ASSERT_EQUAL_STRING("delta", tab[3]);
    TEST_ASSERT_EQUAL_STRING("echo", tab[4]);
    TEST_ASSERT_NULL(tab[5]);
}

/* With a reversed comparator the array ends up in exact descending order. */
static void	testAdvancedSortDescendingOrder(void)
{
    char	*tab[] = {"delta", "alpha", "echo", "charlie", "bravo", NULL};

    ft_advanced_sort_string_tab(tab, cmpDescending);
    TEST_ASSERT_EQUAL_STRING("echo", tab[0]);
    TEST_ASSERT_EQUAL_STRING("delta", tab[1]);
    TEST_ASSERT_EQUAL_STRING("charlie", tab[2]);
    TEST_ASSERT_EQUAL_STRING("bravo", tab[3]);
    TEST_ASSERT_EQUAL_STRING("alpha", tab[4]);
    TEST_ASSERT_NULL(tab[5]);
}

/* After sorting, tab holds exactly the original pointers, each once. */
static void	testAdvancedSortKeepsOriginalPointers(void)
{
    char	*tab[] = {"delta", "alpha", "charlie", "bravo", NULL};
    char	*orig[4];
    int		hits;
    int		i;
    int		j;

    i = 0;
    while (i < 4)
    {
        orig[i] = tab[i];
        i++;
    }
    ft_advanced_sort_string_tab(tab, cmpAscending);
    TEST_ASSERT_NULL(tab[4]);
    i = 0;
    while (i < 4)
    {
        hits = 0;
        j = 0;
        while (j < 4)
        {
            if (tab[j] == orig[i])
                hits++;
            j++;
        }
        TEST_ASSERT_EQUAL_INT(1, hits);
        i++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testAdvancedSortAscendingOrder);
    RUN_TEST(testAdvancedSortDescendingOrder);
    RUN_TEST(testAdvancedSortKeepsOriginalPointers);
}
