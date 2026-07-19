/*
** c12_ex04 -- ft_list_push_back: void ft_list_push_back(t_list **begin_list,
** void *data);
** Appends a freshly allocated element holding data at the end of the list;
** on an empty list *begin_list becomes the new element.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_push_back(t_list **begin_list, void *data);

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

/* Pushing onto an empty list makes *begin_list a 1-element list. */
static void	testPushBackOntoEmptyList(void)
{
    char	*str;
    t_list	*list;

    str = "back";
    list = NULL;
    ft_list_push_back(&list, str);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_PTR(str, list->data);
    TEST_ASSERT_NULL(list->next);
    listFreeAll(list);
}

/* Pushing onto an existing list: previous nodes and their order are
** untouched, the new node is last and its next is NULL. */
static void	testPushBackOntoExistingList(void)
{
    char	*str;
    t_list	*first;
    t_list	*second;
    t_list	*list;
    t_list	*added;

    str = "new tail";
    first = listNew("one");
    second = listNew("two");
    first->next = second;
    list = first;
    ft_list_push_back(&list, str);
    TEST_ASSERT_EQUAL_PTR(first, list);
    TEST_ASSERT_EQUAL_PTR(second, first->next);
    TEST_ASSERT_EQUAL_STRING("one", first->data);
    TEST_ASSERT_EQUAL_STRING("two", second->data);
    added = second->next;
    TEST_ASSERT_NOT_NULL(added);
    TEST_ASSERT_EQUAL_PTR(str, added->data);
    TEST_ASSERT_NULL(added->next);
    listFreeAll(list);
}

/* Two pushes onto an empty list keep insertion order. */
static void	testPushBackTwiceKeepsInsertionOrder(void)
{
    char	*firstStr;
    char	*secondStr;
    t_list	*list;

    firstStr = "first";
    secondStr = "second";
    list = NULL;
    ft_list_push_back(&list, firstStr);
    ft_list_push_back(&list, secondStr);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_PTR(firstStr, list->data);
    TEST_ASSERT_NOT_NULL(list->next);
    TEST_ASSERT_EQUAL_PTR(secondStr, list->next->data);
    TEST_ASSERT_NULL(list->next->next);
    listFreeAll(list);
}

void	glorpRun(void)
{
    RUN_TEST(testPushBackOntoEmptyList);
    RUN_TEST(testPushBackOntoExistingList);
    RUN_TEST(testPushBackTwiceKeepsInsertionOrder);
}
