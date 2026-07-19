/*
** c12_ex16 -- ft_sorted_list_insert: void ft_sorted_list_insert(t_list
** **begin_list, void *data, int (*cmp)());
** Inserts a new element (allocated like ft_create_elem) holding data into a
** cmp-sorted list so the list stays sorted.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_sorted_list_insert(t_list **begin_list, void *data,
                int (*cmp)());

/* strcmp-model comparator: sorted means cmp(a, b) <= 0 for neighbours. */
static int	cmpStr(void *a, void *b)
{
    return (strcmp(a, b));
}

/* Allocates one link holding data, chained to next. */
static t_list	*makeNode(void *data, t_list *next)
{
    t_list	*node;

    node = malloc(sizeof(t_list));
    TEST_ASSERT_NOT_NULL(node);
    node->data = data;
    node->next = next;
    return (node);
}

/* Frees every link of the list, including the one the function allocated
** (the data is not heap-allocated here). */
static void	freeNodes(t_list *list)
{
    t_list	*next;

    while (list != NULL)
    {
        next = list->next;
        free(list);
        list = next;
    }
}

/* Walks the list and asserts its data strings equal expected, then NULL. */
static void	assertDataSequence(t_list *list, const char **expected, int count)
{
    int	i;

    i = 0;
    while (i < count)
    {
        TEST_ASSERT_NOT_NULL(list);
        TEST_ASSERT_EQUAL_STRING(expected[i], list->data);
        list = list->next;
        i++;
    }
    TEST_ASSERT_NULL(list);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Inserting into an empty list creates a single element holding data. */
static void	testSortedInsertIntoEmptyList(void)
{
    t_list	*list;
    void	*data;

    data = "a";
    list = NULL;
    ft_sorted_list_insert(&list, data, (int (*)())cmpStr);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_PTR(data, list->data);
    TEST_ASSERT_NULL(list->next);
    freeNodes(list);
}

/* The smallest value ends up first in the data sequence. */
static void	testSortedInsertSmallestValue(void)
{
    static const char	*expected[] = {"a", "b", "c"};
    t_list				*list;

    list = makeNode("b", makeNode("c", NULL));
    ft_sorted_list_insert(&list, "a", (int (*)())cmpStr);
    assertDataSequence(list, expected, 3);
    freeNodes(list);
}

/* The largest value ends up last, and the list still ends with NULL. */
static void	testSortedInsertLargestValue(void)
{
    static const char	*expected[] = {"a", "b", "c"};
    t_list				*list;

    list = makeNode("a", makeNode("b", NULL));
    ft_sorted_list_insert(&list, "c", (int (*)())cmpStr);
    assertDataSequence(list, expected, 3);
    freeNodes(list);
}

/* A middle value slots in between its neighbours. */
static void	testSortedInsertMiddleValue(void)
{
    static const char	*expected[] = {"a", "b", "c"};
    t_list				*list;

    list = makeNode("a", makeNode("c", NULL));
    ft_sorted_list_insert(&list, "b", (int (*)())cmpStr);
    assertDataSequence(list, expected, 3);
    freeNodes(list);
}

/* Inserting a duplicate key keeps the list sorted and grows it by one. */
static void	testSortedInsertDuplicateKey(void)
{
    static const char	*expected[] = {"a", "b", "b", "c"};
    t_list				*list;

    list = makeNode("a", makeNode("b", makeNode("c", NULL)));
    ft_sorted_list_insert(&list, "b", (int (*)())cmpStr);
    assertDataSequence(list, expected, 4);
    freeNodes(list);
}

void	glorpRun(void)
{
    RUN_TEST(testSortedInsertIntoEmptyList);
    RUN_TEST(testSortedInsertSmallestValue);
    RUN_TEST(testSortedInsertLargestValue);
    RUN_TEST(testSortedInsertMiddleValue);
    RUN_TEST(testSortedInsertDuplicateKey);
}
