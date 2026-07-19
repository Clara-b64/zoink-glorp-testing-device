/*
** c12_ex14 -- ft_list_sort: void ft_list_sort(t_list **begin_list,
** int (*cmp)());
** Sorts the list ascending per cmp, in place. Implementations may relink
** nodes or swap data, so only the resulting data sequence is asserted.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_sort(t_list **begin_list, int (*cmp)());

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

/* A shuffled five-element list comes out in ascending order. */
static void	testSortShuffledFiveElements(void)
{
    static const char	*expected[] = {"a", "b", "c", "d", "e"};
    t_list				*list;

    list = makeNode("d", makeNode("b",
                makeNode("e", makeNode("a", makeNode("c", NULL)))));
    ft_list_sort(&list, (int (*)())cmpStr);
    assertDataSequence(list, expected, 5);
    freeNodes(list);
}

/* An already sorted list keeps its data sequence. */
static void	testSortAlreadySortedList(void)
{
    static const char	*expected[] = {"a", "b", "c"};
    t_list				*list;

    list = makeNode("a", makeNode("b", makeNode("c", NULL)));
    ft_list_sort(&list, (int (*)())cmpStr);
    assertDataSequence(list, expected, 3);
    freeNodes(list);
}

/* Duplicate keys all survive and end up grouped in order. */
static void	testSortWithDuplicateKeys(void)
{
    static const char	*expected[] = {"a", "a", "b", "b"};
    t_list				*list;

    list = makeNode("b", makeNode("a", makeNode("b", makeNode("a", NULL))));
    ft_list_sort(&list, (int (*)())cmpStr);
    assertDataSequence(list, expected, 4);
    freeNodes(list);
}

/* A single-element list stays a single element. */
static void	testSortSingleElement(void)
{
    static const char	*expected[] = {"a"};
    t_list				*list;

    list = makeNode("a", NULL);
    ft_list_sort(&list, (int (*)())cmpStr);
    assertDataSequence(list, expected, 1);
    freeNodes(list);
}

void	glorpRun(void)
{
    RUN_TEST(testSortShuffledFiveElements);
    RUN_TEST(testSortAlreadySortedList);
    RUN_TEST(testSortWithDuplicateKeys);
    RUN_TEST(testSortSingleElement);
}
