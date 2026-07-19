/*
** c12_ex01 -- ft_list_push_front: void ft_list_push_front(t_list **begin_list,
** void *data);
** Inserts a freshly allocated element holding data at the front of the list;
** *begin_list is updated to the new element.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_push_front(t_list **begin_list, void *data);

/* Allocates one list node holding data (test-owned, plain malloc). */
static t_list	*listNew(void *data)
{
    t_list	*node;

    node = malloc(sizeof(t_list));
    TEST_ASSERT_NOT_NULL(node);
    node->data = data;
    node->next = NULL;
    return (node);
}

/* Frees every node of the list. */
static void	listFreeAll(t_list *list)
{
    t_list	*next;

    while (list)
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

/* Pushing onto an empty list makes a 1-element list holding data. */
static void	testPushFrontOntoEmptyList(void)
{
    char	*str;
    t_list	*list;

    str = "front";
    list = NULL;
    ft_list_push_front(&list, str);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_PTR(str, list->data);
    TEST_ASSERT_NULL(list->next);
    listFreeAll(list);
}

/* Pushing onto an existing list: new head, old head becomes ->next, and the
** old nodes are untouched. */
static void	testPushFrontOntoExistingList(void)
{
    char	*str;
    t_list	*first;
    t_list	*second;
    t_list	*list;

    str = "new head";
    first = listNew("one");
    second = listNew("two");
    first->next = second;
    list = first;
    ft_list_push_front(&list, str);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_TRUE(list != first);
    TEST_ASSERT_EQUAL_PTR(str, list->data);
    TEST_ASSERT_EQUAL_PTR(first, list->next);
    TEST_ASSERT_EQUAL_PTR(second, first->next);
    TEST_ASSERT_NULL(second->next);
    TEST_ASSERT_EQUAL_STRING("one", first->data);
    TEST_ASSERT_EQUAL_STRING("two", second->data);
    listFreeAll(list);
}

/* Two pushes in a row stack up in reverse order of insertion. */
static void	testPushFrontTwiceStacksInReverseOrder(void)
{
    char	*older;
    char	*newer;
    t_list	*list;

    older = "older";
    newer = "newer";
    list = NULL;
    ft_list_push_front(&list, older);
    ft_list_push_front(&list, newer);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_PTR(newer, list->data);
    TEST_ASSERT_NOT_NULL(list->next);
    TEST_ASSERT_EQUAL_PTR(older, list->next->data);
    TEST_ASSERT_NULL(list->next->next);
    listFreeAll(list);
}

void	glorpRun(void)
{
    RUN_TEST(testPushFrontOntoEmptyList);
    RUN_TEST(testPushFrontOntoExistingList);
    RUN_TEST(testPushFrontTwiceStacksInReverseOrder);
}
