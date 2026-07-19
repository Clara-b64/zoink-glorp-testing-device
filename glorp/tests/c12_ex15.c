/*
** c12_ex15 -- ft_list_reverse_fun: void ft_list_reverse_fun(t_list
** *begin_list);
** Reverses the list content without relinking: the node sequence stays
** identical, the data pointers end up in reversed order.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_reverse_fun(t_list *begin_list);

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

/* Four elements: every node keeps its position, the data are reversed. */
static void	testReverseFunFourElementsDataReversedNodesKept(void)
{
    t_list	*nodes[4];
    void	*datas[4];
    t_list	*list;
    t_list	*walk;
    int		i;

    list = makeNode("a", makeNode("b", makeNode("c", makeNode("d", NULL))));
    walk = list;
    i = 0;
    while (i < 4)
    {
        nodes[i] = walk;
        datas[i] = walk->data;
        walk = walk->next;
        i++;
    }
    ft_list_reverse_fun(list);
    walk = list;
    i = 0;
    while (i < 4)
    {
        TEST_ASSERT_EQUAL_PTR(nodes[i], walk);
        TEST_ASSERT_EQUAL_PTR(datas[3 - i], walk->data);
        walk = walk->next;
        i++;
    }
    TEST_ASSERT_NULL(walk);
    freeNodes(list);
}

/* A single-element list is left unchanged. */
static void	testReverseFunSingleElementUnchanged(void)
{
    t_list	*list;
    void	*data;

    list = makeNode("a", NULL);
    data = list->data;
    ft_list_reverse_fun(list);
    TEST_ASSERT_EQUAL_PTR(data, list->data);
    TEST_ASSERT_NULL(list->next);
    freeNodes(list);
}

void	glorpRun(void)
{
    RUN_TEST(testReverseFunFourElementsDataReversedNodesKept);
    RUN_TEST(testReverseFunSingleElementUnchanged);
}
