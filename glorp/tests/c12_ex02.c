/*
** c12_ex02 -- ft_list_size: int ft_list_size(t_list *begin_list);
** Returns the number of elements in the list; an empty (NULL) list has 0.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern int	ft_list_size(t_list *begin_list);

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

/* Builds a list of count nodes (all data NULL). */
static t_list	*listOfSize(int count)
{
    t_list	*head;
    t_list	*node;

    head = NULL;
    while (count > 0)
    {
        node = listNew(NULL);
        node->next = head;
        head = node;
        count--;
    }
    return (head);
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

/* A NULL list has size 0. */
static void	testSizeOfNullListIsZero(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_list_size(NULL));
}

/* A single element list has size 1. */
static void	testSizeOfOneElementList(void)
{
    t_list	*list;

    list = listOfSize(1);
    TEST_ASSERT_EQUAL_INT(1, ft_list_size(list));
    listFreeAll(list);
}

/* A five element list has size 5. */
static void	testSizeOfFiveElementList(void)
{
    t_list	*list;

    list = listOfSize(5);
    TEST_ASSERT_EQUAL_INT(5, ft_list_size(list));
    listFreeAll(list);
}

void	glorpRun(void)
{
    RUN_TEST(testSizeOfNullListIsZero);
    RUN_TEST(testSizeOfOneElementList);
    RUN_TEST(testSizeOfFiveElementList);
}
