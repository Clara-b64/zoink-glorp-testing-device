/*
** c12_ex10 -- ft_list_foreach_if: void ft_list_foreach_if(t_list *begin_list,
** void (*f)(void *), void *data_ref, int (*cmp)());
** Applies f only to the data of elements for which cmp(data, data_ref) == 0.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_foreach_if(t_list *begin_list, void (*f)(void *),
                void *data_ref, int (*cmp)());

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

/* strcmp-model comparator: 0 means the two strings match. */
static int	cmpStr(void *a, void *b)
{
    return (strcmp(a, b));
}

/* Duplicates s into a fresh malloc'd buffer. */
static char	*dupStr(const char *s)
{
    char	*copy;

    copy = malloc(strlen(s) + 1);
    TEST_ASSERT_NOT_NULL(copy);
    strcpy(copy, s);
    return (copy);
}

/* Allocates one link holding a malloc'd copy of s, chained to next. */
static t_list	*makeNode(const char *s, t_list *next)
{
    t_list	*node;

    node = malloc(sizeof(t_list));
    TEST_ASSERT_NOT_NULL(node);
    node->data = dupStr(s);
    node->next = next;
    return (node);
}

/* Frees every link of the list and its malloc'd data. */
static void	freeList(t_list *list)
{
    t_list	*next;

    while (list != NULL)
    {
        next = list->next;
        free(list->data);
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

/* On ["a","b","a","c"] with ref "a" (a separate buffer), f is called
** exactly twice, with the two "a" data pointers, in list order. */
static void	testForeachIfAppliesOnlyToMatchesInOrder(void)
{
    t_list	*fourth;
    t_list	*third;
    t_list	*second;
    t_list	*first;
    char	ref[2];

    strcpy(ref, "a");
    fourth = makeNode("c", NULL);
    third = makeNode("a", fourth);
    second = makeNode("b", third);
    first = makeNode("a", second);
    ft_list_foreach_if(first, recordCall, ref, (int (*)())cmpStr);
    TEST_ASSERT_EQUAL_INT(2, gCallCount);
    TEST_ASSERT_EQUAL_PTR(first->data, gCalls[0]);
    TEST_ASSERT_EQUAL_PTR(third->data, gCalls[1]);
    freeList(first);
}

/* A reference matching no element yields zero calls. */
static void	testForeachIfNoMatchMakesNoCall(void)
{
    t_list	*second;
    t_list	*first;
    char	ref[2];

    strcpy(ref, "z");
    second = makeNode("b", NULL);
    first = makeNode("a", second);
    ft_list_foreach_if(first, recordCall, ref, (int (*)())cmpStr);
    TEST_ASSERT_EQUAL_INT(0, gCallCount);
    freeList(first);
}

void	glorpRun(void)
{
    RUN_TEST(testForeachIfAppliesOnlyToMatchesInOrder);
    RUN_TEST(testForeachIfNoMatchMakesNoCall);
}
