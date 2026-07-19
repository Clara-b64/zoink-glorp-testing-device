/*
** c12_ex13 -- ft_list_merge: void ft_list_merge(t_list **begin_list1,
** t_list *begin_list2);
** Appends list2 to the end of list1, reusing the existing links (no copies).
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_merge(t_list **begin_list1, t_list *begin_list2);

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

/* Frees every link of the list (the data is not heap-allocated here). */
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

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The tail of list1 now links to the old list2 head (same links), and the
** full order is list1's elements followed by list2's. */
static void	testMergeAppendsListTwoAfterListOne(void)
{
    t_list	*secondB;
    t_list	*firstB;
    t_list	*secondA;
    t_list	*firstA;
    t_list	*list;

    secondB = makeNode("d", NULL);
    firstB = makeNode("c", secondB);
    secondA = makeNode("b", NULL);
    firstA = makeNode("a", secondA);
    list = firstA;
    ft_list_merge(&list, firstB);
    TEST_ASSERT_EQUAL_PTR(firstA, list);
    TEST_ASSERT_EQUAL_PTR(secondA, firstA->next);
    TEST_ASSERT_EQUAL_PTR(firstB, secondA->next);
    TEST_ASSERT_EQUAL_PTR(secondB, firstB->next);
    TEST_ASSERT_NULL(secondB->next);
    TEST_ASSERT_EQUAL_STRING("a", firstA->data);
    TEST_ASSERT_EQUAL_STRING("b", secondA->data);
    TEST_ASSERT_EQUAL_STRING("c", firstB->data);
    TEST_ASSERT_EQUAL_STRING("d", secondB->data);
    freeNodes(list);
}

/* Merging into an empty list1 makes *begin_list1 point to list2. */
static void	testMergeIntoEmptyListOneYieldsListTwo(void)
{
    t_list	*secondB;
    t_list	*firstB;
    t_list	*list;

    secondB = makeNode("d", NULL);
    firstB = makeNode("c", secondB);
    list = NULL;
    ft_list_merge(&list, firstB);
    TEST_ASSERT_EQUAL_PTR(firstB, list);
    TEST_ASSERT_EQUAL_PTR(secondB, firstB->next);
    TEST_ASSERT_NULL(secondB->next);
    freeNodes(list);
}

void	glorpRun(void)
{
    RUN_TEST(testMergeAppendsListTwoAfterListOne);
    RUN_TEST(testMergeIntoEmptyListOneYieldsListTwo);
}
