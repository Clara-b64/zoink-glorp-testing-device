/*
** c12_ex08 -- ft_list_reverse: void ft_list_reverse(t_list **begin_list);
** Reverses the list in place: the very same links are rechained in the
** opposite order (no allocation), data untouched, and *begin_list becomes
** the old tail.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_reverse(t_list **begin_list);

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

/* A 1-element list is unchanged: same head, next stays NULL. */
static void	testReverseOneElementListIsUnchanged(void)
{
    char	*str;
    t_list	*only;
    t_list	*list;

    str = "only";
    only = listNew(str);
    list = only;
    ft_list_reverse(&list);
    TEST_ASSERT_EQUAL_PTR(only, list);
    TEST_ASSERT_NULL(list->next);
    TEST_ASSERT_EQUAL_PTR(str, list->data);
    listFreeAll(list);
}

/* A 4-element list is fully reversed: head becomes the old tail, every link
** is the same node rechained, the new tail's next is NULL, data untouched. */
static void	testReverseFourElementList(void)
{
    t_list	*first;
    t_list	*second;
    t_list	*third;
    t_list	*fourth;
    t_list	*list;

    first = listNew("one");
    second = listNew("two");
    third = listNew("three");
    fourth = listNew("four");
    first->next = second;
    second->next = third;
    third->next = fourth;
    list = first;
    ft_list_reverse(&list);
    TEST_ASSERT_EQUAL_PTR(fourth, list);
    TEST_ASSERT_EQUAL_PTR(third, fourth->next);
    TEST_ASSERT_EQUAL_PTR(second, third->next);
    TEST_ASSERT_EQUAL_PTR(first, second->next);
    TEST_ASSERT_NULL(first->next);
    TEST_ASSERT_EQUAL_STRING("one", first->data);
    TEST_ASSERT_EQUAL_STRING("two", second->data);
    TEST_ASSERT_EQUAL_STRING("three", third->data);
    TEST_ASSERT_EQUAL_STRING("four", fourth->data);
    listFreeAll(list);
}

void	glorpRun(void)
{
    RUN_TEST(testReverseOneElementListIsUnchanged);
    RUN_TEST(testReverseFourElementList);
}
