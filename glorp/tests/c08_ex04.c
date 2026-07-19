/*
** c08_ex04 -- ft_strs_to_tab: struct s_stock_str *ft_strs_to_tab(int ac,
** char **av);
** Returns a malloc'd array of ac + 1 t_stock_str: for each av[i], size is its
** length, str holds the argument and copy a malloc'd duplicate of it; the
** array is terminated by an element whose str is 0. The struct is redefined
** here field-for-field (the test never includes the piscine's header). The
** copy must be a real duplicate: not av[i] itself, and editable without
** touching av[i]. str may be av[i] itself or another copy, so only its content
** is asserted; the test frees accordingly.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_stock_str
{
    int		size;
    char	*str;
    char	*copy;
}	t_stock_str;

extern struct s_stock_str	*ft_strs_to_tab(int ac, char **av);

/* Frees one returned table, tolerating str being av[i], a separate copy, or
** even the same block as copy (never double-freed). */
static void	freeTab(t_stock_str *tab, int ac, char **av)
{
    int	i;

    i = 0;
    while (i < ac)
    {
        free(tab[i].copy);
        if (tab[i].str != av[i] && tab[i].str != tab[i].copy)
            free(tab[i].str);
        i++;
    }
    free(tab);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Each element carries the right size, content and a genuine copy. */
static void	testStrsToTabBasic(void)
{
    char		*av[] = {"hello", "", "42 rocks"};
    t_stock_str	*tab;
    int			i;

    tab = ft_strs_to_tab(3, av);
    TEST_ASSERT_NOT_NULL(tab);
    i = 0;
    while (i < 3)
    {
        TEST_ASSERT_EQUAL_INT((int)strlen(av[i]), tab[i].size);
        TEST_ASSERT_NOT_NULL(tab[i].str);
        TEST_ASSERT_EQUAL_STRING(av[i], tab[i].str);
        TEST_ASSERT_NOT_NULL(tab[i].copy);
        TEST_ASSERT_EQUAL_STRING(av[i], tab[i].copy);
        i++;
    }
    freeTab(tab, 3, av);
}

/* The array ends with an element whose str is 0, right after the ac-th. */
static void	testStrsToTabTerminator(void)
{
    char		*av[] = {"one", "two"};
    t_stock_str	*tab;

    tab = ft_strs_to_tab(2, av);
    TEST_ASSERT_NOT_NULL(tab);
    TEST_ASSERT_NULL(tab[2].str);
    freeTab(tab, 2, av);
}

/* copy is a distinct allocation: never av[i], and editing it leaves the
** original argument untouched. */
static void	testStrsToTabCopyIsIndependent(void)
{
    char		buf[] = "mutable";
    char		*av[1];
    t_stock_str	*tab;

    av[0] = buf;
    tab = ft_strs_to_tab(1, av);
    TEST_ASSERT_NOT_NULL(tab);
    TEST_ASSERT_TRUE(tab[0].copy != buf);
    TEST_ASSERT_EQUAL_STRING("mutable", tab[0].copy);
    tab[0].copy[0] = 'X';
    TEST_ASSERT_EQUAL_STRING("mutable", buf);
    freeTab(tab, 1, av);
}

/* ac == 0 still returns a table holding just the terminator. */
static void	testStrsToTabEmpty(void)
{
    char		*av[] = {NULL};
    t_stock_str	*tab;

    tab = ft_strs_to_tab(0, av);
    TEST_ASSERT_NOT_NULL(tab);
    TEST_ASSERT_NULL(tab[0].str);
    free(tab);
}

void	glorpRun(void)
{
    RUN_TEST(testStrsToTabBasic);
    RUN_TEST(testStrsToTabTerminator);
    RUN_TEST(testStrsToTabCopyIsIndependent);
    RUN_TEST(testStrsToTabEmpty);
}
