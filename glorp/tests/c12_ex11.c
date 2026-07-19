/*
** c12_ex11 -- ft_list_find: t_list *ft_list_find(t_list *begin_list,
** void *data_ref, int (*cmp)());
** Returns the first link whose data matches data_ref (cmp == 0), else NULL.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern t_list	*ft_list_find(t_list *begin_list, void *data_ref,
                    int (*cmp)());

/* strcmp-model comparator: 0 means the two strings match. */
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

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A match in the middle returns that very link (pointer identity). */
static void	testFindMatchInMiddleReturnsThatLink(void)
{
    t_list	*third;
    t_list	*second;
    t_list	*first;
    t_list	*found;
    char	ref[2];

    strcpy(ref, "b");
    third = makeNode("c", NULL);
    second = makeNode("b", third);
    first = makeNode("a", second);
    found = ft_list_find(first, ref, (int (*)())cmpStr);
    TEST_ASSERT_EQUAL_PTR(second, found);
    freeNodes(first);
}

/* A match at the head returns the first link. */
static void	testFindMatchAtHeadReturnsFirstLink(void)
{
    t_list	*third;
    t_list	*second;
    t_list	*first;
    t_list	*found;
    char	ref[2];

    strcpy(ref, "a");
    third = makeNode("c", NULL);
    second = makeNode("b", third);
    first = makeNode("a", second);
    found = ft_list_find(first, ref, (int (*)())cmpStr);
    TEST_ASSERT_EQUAL_PTR(first, found);
    freeNodes(first);
}

/* Two links match: the FIRST one (in list order) is returned, not the last. */
static void	testFindDuplicateMatchesReturnsFirst(void)
{
    t_list	*third;
    t_list	*second;
    t_list	*first;
    t_list	*found;
    char	dupA[2];
    char	dupB[2];
    char	ref[2];

    strcpy(dupA, "x");
    strcpy(dupB, "x");
    strcpy(ref, "x");
    third = makeNode(dupB, NULL);
    second = makeNode(dupA, third);
    first = makeNode("a", second);
    found = ft_list_find(first, ref, (int (*)())cmpStr);
    TEST_ASSERT_EQUAL_PTR(second, found);
    freeNodes(first);
}

/* No element matching the reference returns NULL. */
static void	testFindNoMatchReturnsNull(void)
{
    t_list	*second;
    t_list	*first;
    char	ref[2];

    strcpy(ref, "z");
    second = makeNode("b", NULL);
    first = makeNode("a", second);
    TEST_ASSERT_NULL(ft_list_find(first, ref, (int (*)())cmpStr));
    freeNodes(first);
}

/* A NULL list returns NULL. */
static void	testFindNullListReturnsNull(void)
{
    char	ref[2];

    strcpy(ref, "a");
    TEST_ASSERT_NULL(ft_list_find(NULL, ref, (int (*)())cmpStr));
}

void	glorpRun(void)
{
    RUN_TEST(testFindMatchInMiddleReturnsThatLink);
    RUN_TEST(testFindMatchAtHeadReturnsFirstLink);
    RUN_TEST(testFindDuplicateMatchesReturnsFirst);
    RUN_TEST(testFindNoMatchReturnsNull);
    RUN_TEST(testFindNullListReturnsNull);
}
