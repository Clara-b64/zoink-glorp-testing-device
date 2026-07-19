/*
** c12_ex09 -- ft_list_foreach: void ft_list_foreach(t_list *begin_list,
** void (*f)(void *));
** Applies f to the data of every element of the list, in list order.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_foreach(t_list *begin_list, void (*f)(void *));

#define MAX_CALLS 16

static void	*gCalls[MAX_CALLS];
static int	gCallCount;

/* Recording callback: stores every data pointer it is called with. */
static void	recordCall(void *data)
{
    if (gCallCount < MAX_CALLS)
        gCalls[gCallCount] = data;
    gCallCount++;
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
    int	i;

    gCallCount = 0;
    i = 0;
    while (i < MAX_CALLS)
    {
        gCalls[i] = NULL;
        i++;
    }
}

void	glorpDown(void)
{
}

/* f is applied to each element's data, once each, in list order. */
static void	testForeachAppliesToEachDataInOrder(void)
{
    t_list	*third;
    t_list	*second;
    t_list	*first;

    third = makeNode("three", NULL);
    second = makeNode("two", third);
    first = makeNode("one", second);
    ft_list_foreach(first, recordCall);
    TEST_ASSERT_EQUAL_INT(3, gCallCount);
    TEST_ASSERT_EQUAL_PTR(first->data, gCalls[0]);
    TEST_ASSERT_EQUAL_PTR(second->data, gCalls[1]);
    TEST_ASSERT_EQUAL_PTR(third->data, gCalls[2]);
    freeNodes(first);
}

/* A NULL list triggers no call at all. */
static void	testForeachNullListMakesNoCall(void)
{
    ft_list_foreach(NULL, recordCall);
    TEST_ASSERT_EQUAL_INT(0, gCallCount);
}

void	glorpRun(void)
{
    RUN_TEST(testForeachAppliesToEachDataInOrder);
    RUN_TEST(testForeachNullListMakesNoCall);
}
