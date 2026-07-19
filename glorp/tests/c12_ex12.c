/*
** c12_ex12 -- ft_list_remove_if: void ft_list_remove_if(t_list **begin_list,
** void *data_ref, int (*cmp)(), void (*free_fct)(void *));
** Removes every link whose data matches data_ref (cmp == 0), applying
** free_fct to its data and free() to the link itself.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_remove_if(t_list **begin_list, void *data_ref,
                int (*cmp)(), void (*free_fct)(void *));

#define MAX_FREES 16

static void	*gFreed[MAX_FREES];
static int	gFreeCount;

/* Counting free_fct: records the data pointer, then really frees it. */
static void	countingFree(void *data)
{
    if (gFreeCount < MAX_FREES)
        gFreed[gFreeCount] = data;
    gFreeCount++;
    free(data);
}

/* True when p was one of the pointers handed to countingFree. */
static int	wasFreed(void *p)
{
    int	i;

    i = 0;
    while (i < gFreeCount && i < MAX_FREES)
    {
        if (gFreed[i] == p)
            return (1);
        i++;
    }
    return (0);
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

/* Frees every surviving link of the list and its malloc'd data. */
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

    gFreeCount = 0;
    i = 0;
    while (i < MAX_FREES)
    {
        gFreed[i] = NULL;
        i++;
    }
}

void	glorpDown(void)
{
}

/* Removing the middle element relinks head to tail and frees its data. */
static void	testRemoveIfMiddleElement(void)
{
    t_list	*third;
    t_list	*second;
    t_list	*first;
    t_list	*list;
    void	*removedData;
    char	ref[2];

    strcpy(ref, "b");
    third = makeNode("c", NULL);
    second = makeNode("b", third);
    first = makeNode("a", second);
    removedData = second->data;
    list = first;
    ft_list_remove_if(&list, ref, (int (*)())cmpStr, countingFree);
    TEST_ASSERT_EQUAL_PTR(first, list);
    TEST_ASSERT_EQUAL_PTR(third, first->next);
    TEST_ASSERT_NULL(third->next);
    TEST_ASSERT_EQUAL_STRING("a", first->data);
    TEST_ASSERT_EQUAL_STRING("c", third->data);
    TEST_ASSERT_EQUAL_INT(1, gFreeCount);
    TEST_ASSERT_EQUAL_PTR(removedData, gFreed[0]);
    freeList(list);
}

/* Removing the head makes the second link the new first link. */
static void	testRemoveIfHeadElement(void)
{
    t_list	*third;
    t_list	*second;
    t_list	*list;
    void	*removedData;
    char	ref[2];

    strcpy(ref, "a");
    third = makeNode("c", NULL);
    second = makeNode("b", third);
    list = makeNode("a", second);
    removedData = list->data;
    ft_list_remove_if(&list, ref, (int (*)())cmpStr, countingFree);
    TEST_ASSERT_EQUAL_PTR(second, list);
    TEST_ASSERT_EQUAL_PTR(third, second->next);
    TEST_ASSERT_NULL(third->next);
    TEST_ASSERT_EQUAL_INT(1, gFreeCount);
    TEST_ASSERT_EQUAL_PTR(removedData, gFreed[0]);
    freeList(list);
}

/* Every match is removed, including consecutive ones and the tail. */
static void	testRemoveIfSeveralIncludingConsecutive(void)
{
    t_list	*fifth;
    t_list	*fourth;
    t_list	*third;
    t_list	*second;
    t_list	*first;
    t_list	*list;
    void	*removedOne;
    void	*removedTwo;
    void	*removedThree;
    char	ref[2];

    strcpy(ref, "a");
    fifth = makeNode("a", NULL);
    fourth = makeNode("b", fifth);
    third = makeNode("a", fourth);
    second = makeNode("a", third);
    first = makeNode("x", second);
    removedOne = second->data;
    removedTwo = third->data;
    removedThree = fifth->data;
    list = first;
    ft_list_remove_if(&list, ref, (int (*)())cmpStr, countingFree);
    TEST_ASSERT_EQUAL_PTR(first, list);
    TEST_ASSERT_EQUAL_PTR(fourth, first->next);
    TEST_ASSERT_NULL(fourth->next);
    TEST_ASSERT_EQUAL_STRING("x", first->data);
    TEST_ASSERT_EQUAL_STRING("b", fourth->data);
    TEST_ASSERT_EQUAL_INT(3, gFreeCount);
    TEST_ASSERT_TRUE(wasFreed(removedOne));
    TEST_ASSERT_TRUE(wasFreed(removedTwo));
    TEST_ASSERT_TRUE(wasFreed(removedThree));
    freeList(list);
}

/* When every element matches, the list becomes empty (NULL). */
static void	testRemoveIfAllElements(void)
{
    t_list	*second;
    t_list	*list;
    void	*removedOne;
    void	*removedTwo;
    char	ref[2];

    strcpy(ref, "a");
    second = makeNode("a", NULL);
    list = makeNode("a", second);
    removedOne = list->data;
    removedTwo = second->data;
    ft_list_remove_if(&list, ref, (int (*)())cmpStr, countingFree);
    TEST_ASSERT_NULL(list);
    TEST_ASSERT_EQUAL_INT(2, gFreeCount);
    TEST_ASSERT_TRUE(wasFreed(removedOne));
    TEST_ASSERT_TRUE(wasFreed(removedTwo));
}

/* No match leaves the list unchanged: same links, same data, no free. */
static void	testRemoveIfNoMatchLeavesListUnchanged(void)
{
    t_list	*second;
    t_list	*first;
    t_list	*list;
    void	*dataOne;
    void	*dataTwo;
    char	ref[2];

    strcpy(ref, "z");
    second = makeNode("b", NULL);
    first = makeNode("a", second);
    dataOne = first->data;
    dataTwo = second->data;
    list = first;
    ft_list_remove_if(&list, ref, (int (*)())cmpStr, countingFree);
    TEST_ASSERT_EQUAL_PTR(first, list);
    TEST_ASSERT_EQUAL_PTR(second, first->next);
    TEST_ASSERT_NULL(second->next);
    TEST_ASSERT_EQUAL_PTR(dataOne, first->data);
    TEST_ASSERT_EQUAL_PTR(dataTwo, second->data);
    TEST_ASSERT_EQUAL_INT(0, gFreeCount);
    freeList(list);
}

void	glorpRun(void)
{
    RUN_TEST(testRemoveIfMiddleElement);
    RUN_TEST(testRemoveIfHeadElement);
    RUN_TEST(testRemoveIfSeveralIncludingConsecutive);
    RUN_TEST(testRemoveIfAllElements);
    RUN_TEST(testRemoveIfNoMatchLeavesListUnchanged);
}
