/*
** c12_ex17 -- ft_sorted_list_merge: void ft_sorted_list_merge(t_list
** **begin_list1, t_list *begin_list2, int (*cmp)());
** Merges the sorted list2 into the sorted list1 so that *begin_list1 is
** sorted, reusing the existing links (no new allocations).
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_sorted_list_merge(t_list **begin_list1, t_list *begin_list2,
                int (*cmp)());

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

/* Asserts the list is made of exactly the count expected nodes, once each
** (in any order): the links are reused, none added, none dropped. */
static void	assertNodesAreExactly(t_list *list, t_list **expected, int count)
{
    int	used[8];
    int	seen;
    int	i;

    i = 0;
    while (i < count)
    {
        used[i] = 0;
        i++;
    }
    seen = 0;
    while (list != NULL)
    {
        i = 0;
        while (i < count && (used[i] || expected[i] != list))
            i++;
        TEST_ASSERT_TRUE_MESSAGE(i < count, "link not from the input lists");
        used[i] = 1;
        seen++;
        list = list->next;
    }
    TEST_ASSERT_EQUAL_INT(count, seen);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* ["a","c","e"] merged with ["b","d"] gives "a b c d e", made of exactly
** the five original links. */
static void	testSortedMergeInterleavedValues(void)
{
    static const char	*expected[] = {"a", "b", "c", "d", "e"};
    t_list				*original[5];
    t_list				*list1;
    t_list				*list2;

    list1 = makeNode("a", makeNode("c", makeNode("e", NULL)));
    list2 = makeNode("b", makeNode("d", NULL));
    original[0] = list1;
    original[1] = list1->next;
    original[2] = list1->next->next;
    original[3] = list2;
    original[4] = list2->next;
    ft_sorted_list_merge(&list1, list2, (int (*)())cmpStr);
    assertDataSequence(list1, expected, 5);
    assertNodesAreExactly(list1, original, 5);
    freeNodes(list1);
}

/* ["b","d"] merged with ["a","c"] must move list2's first link in front of
** list1's old head: an implementation that can only insert after existing
** list1 nodes fails here. */
static void	testSortedMergeNewHeadComesFromListTwo(void)
{
    static const char	*expected[] = {"a", "b", "c", "d"};
    t_list				*original[4];
    t_list				*list2Head;
    t_list				*list1;
    t_list				*list2;

    list1 = makeNode("b", makeNode("d", NULL));
    list2 = makeNode("a", makeNode("c", NULL));
    list2Head = list2;
    original[0] = list1;
    original[1] = list1->next;
    original[2] = list2;
    original[3] = list2->next;
    ft_sorted_list_merge(&list1, list2, (int (*)())cmpStr);
    TEST_ASSERT_EQUAL_PTR(list2Head, list1);
    assertDataSequence(list1, expected, 4);
    assertNodesAreExactly(list1, original, 4);
    freeNodes(list1);
}

/* Merging into an empty list1 yields exactly list2's nodes in order. */
static void	testSortedMergeIntoEmptyListOne(void)
{
    t_list	*secondB;
    t_list	*list1;
    t_list	*list2;

    secondB = makeNode("d", NULL);
    list2 = makeNode("b", secondB);
    list1 = NULL;
    ft_sorted_list_merge(&list1, list2, (int (*)())cmpStr);
    TEST_ASSERT_EQUAL_PTR(list2, list1);
    TEST_ASSERT_EQUAL_PTR(secondB, list1->next);
    TEST_ASSERT_NULL(secondB->next);
    freeNodes(list1);
}

void	glorpRun(void)
{
    RUN_TEST(testSortedMergeInterleavedValues);
    RUN_TEST(testSortedMergeNewHeadComesFromListTwo);
    RUN_TEST(testSortedMergeIntoEmptyListOne);
}
