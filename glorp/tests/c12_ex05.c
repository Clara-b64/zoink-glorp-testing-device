/*
** c12_ex05 -- ft_list_push_strs: t_list *ft_list_push_strs(int size,
** char **strs);
** Creates a list of the size strings of strs, pushed in order such that
** strs[0] ends up in the last link; returns the head of the list. Content is
** asserted by string equality: an implementation may store the original
** pointer or a copy.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern t_list	*ft_list_push_strs(int size, char **strs);

/* Frees the list: node k (from the head) matches strs[size - 1 - k]; its
** data is freed only when it is not that original pointer (copy case),
** then the node itself is freed. */
static void	freeStrsList(t_list *list, int size, char **strs)
{
    t_list	*next;
    int		i;

    i = 0;
    while (list)
    {
        next = list->next;
        if (i < size && list->data != (void *)strs[size - 1 - i])
            free(list->data);
        free(list);
        list = next;
        i++;
    }
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* size 1: a single link holding strs[0]. */
static void	testPushStrsSizeOne(void)
{
    char	*strs[1];
    t_list	*list;

    strs[0] = "solo";
    list = ft_list_push_strs(1, strs);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_STRING("solo", list->data);
    TEST_ASSERT_NULL(list->next);
    freeStrsList(list, 1, strs);
}

/* size 3: strs[2] first, strs[1] second, strs[0] in the last link. */
static void	testPushStrsSizeThreeOrder(void)
{
    char	*strs[3];
    t_list	*list;

    strs[0] = "alpha";
    strs[1] = "beta";
    strs[2] = "gamma";
    list = ft_list_push_strs(3, strs);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_STRING("gamma", list->data);
    TEST_ASSERT_NOT_NULL(list->next);
    TEST_ASSERT_EQUAL_STRING("beta", list->next->data);
    TEST_ASSERT_NOT_NULL(list->next->next);
    TEST_ASSERT_EQUAL_STRING("alpha", list->next->next->data);
    TEST_ASSERT_NULL(list->next->next->next);
    freeStrsList(list, 3, strs);
}

void	glorpRun(void)
{
    RUN_TEST(testPushStrsSizeOne);
    RUN_TEST(testPushStrsSizeThreeOrder);
}
