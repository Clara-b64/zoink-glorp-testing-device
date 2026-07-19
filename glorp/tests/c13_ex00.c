/*
** c13_ex00 -- btree_create_node: t_btree *btree_create_node(void *item);
** Returns a freshly allocated node: item is stored as given (same pointer,
** no copy), left and right are NULL.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_btree
{
    struct s_btree	*left;
    struct s_btree	*right;
    void			*item;
}	t_btree;

extern t_btree	*btree_create_node(void *item);

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The returned node is non-NULL (an allocation happened). */
static void	testCreateNodeReturnsNonNull(void)
{
    t_btree	*node;

    node = btree_create_node("hello");
    TEST_ASSERT_NOT_NULL(node);
    free(node);
}

/* item is stored as given: the exact same pointer, not a copy. */
static void	testCreateNodeStoresItemPointerIdentically(void)
{
    char	payload[] = "payload";
    t_btree	*node;
    void	*storedItem;

    node = btree_create_node(payload);
    TEST_ASSERT_NOT_NULL(node);
    storedItem = node->item;
    free(node);
    TEST_ASSERT_EQUAL_PTR(payload, storedItem);
}

/* A NULL item is stored as given too. */
static void	testCreateNodeStoresNullItem(void)
{
    t_btree	*node;
    void	*storedItem;

    node = btree_create_node(NULL);
    TEST_ASSERT_NOT_NULL(node);
    storedItem = node->item;
    free(node);
    TEST_ASSERT_NULL(storedItem);
}

/* Both children of a fresh node are NULL. */
static void	testCreateNodeChildrenAreNull(void)
{
    t_btree	*node;
    t_btree	*left;
    t_btree	*right;

    node = btree_create_node("x");
    TEST_ASSERT_NOT_NULL(node);
    left = node->left;
    right = node->right;
    free(node);
    TEST_ASSERT_NULL(left);
    TEST_ASSERT_NULL(right);
}

/* Two calls return two distinct nodes, each holding its own item. */
static void	testCreateNodeTwoCallsAreDistinct(void)
{
    char	itemA[] = "a";
    char	itemB[] = "b";
    t_btree	*first;
    t_btree	*second;
    int		bothAllocated;
    int		distinct;
    void	*firstItem;
    void	*secondItem;

    first = btree_create_node(itemA);
    second = btree_create_node(itemB);
    bothAllocated = (first != NULL && second != NULL);
    distinct = (first != second);
    firstItem = NULL;
    secondItem = NULL;
    if (first)
        firstItem = first->item;
    if (second)
        secondItem = second->item;
    free(first);
    free(second);
    TEST_ASSERT_TRUE(bothAllocated);
    TEST_ASSERT_TRUE(distinct);
    TEST_ASSERT_EQUAL_PTR(itemA, firstItem);
    TEST_ASSERT_EQUAL_PTR(itemB, secondItem);
}

void	glorpRun(void)
{
    RUN_TEST(testCreateNodeReturnsNonNull);
    RUN_TEST(testCreateNodeStoresItemPointerIdentically);
    RUN_TEST(testCreateNodeStoresNullItem);
    RUN_TEST(testCreateNodeChildrenAreNull);
    RUN_TEST(testCreateNodeTwoCallsAreDistinct);
}
