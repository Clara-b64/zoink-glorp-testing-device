/*
** c12_ex03 -- ft_list_last: t_list *ft_list_last(t_list *begin_list);
** Returns the last link of the list.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern t_list	*ft_list_last(t_list *begin_list);

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

/* The last link of a 1-element list is that element itself. */
static void	testLastOfOneElementListIsThatNode(void)
{
    t_list	*only;

    only = listNew("only");
    TEST_ASSERT_EQUAL_PTR(only, ft_list_last(only));
    listFreeAll(only);
}

/* The last link of a 3-element list is the third node. */
static void	testLastOfThreeElementListIsThirdNode(void)
{
    t_list	*first;
    t_list	*second;
    t_list	*third;

    first = listNew("one");
    second = listNew("two");
    third = listNew("three");
    first->next = second;
    second->next = third;
    TEST_ASSERT_EQUAL_PTR(third, ft_list_last(first));
    listFreeAll(first);
}

void	glorpRun(void)
{
    RUN_TEST(testLastOfOneElementListIsThatNode);
    RUN_TEST(testLastOfThreeElementListIsThirdNode);
}
