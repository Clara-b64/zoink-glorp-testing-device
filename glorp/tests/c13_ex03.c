/*
** c13_ex03 -- btree_apply_suffix:
** void btree_apply_suffix(t_btree *root, void (*applyf)(void *));
** Applies applyf to every item using suffix traversal: the left subtree
** first, then the right subtree, then the current node.
*/

#include "runner.h"

typedef struct s_btree
{
    struct s_btree	*left;
    struct s_btree	*right;
    void			*item;
}	t_btree;

extern void	btree_apply_suffix(t_btree *root, void (*applyf)(void *));

#define MAX_CALLS 16

static void	*gItems[MAX_CALLS];
static int	gCallCount;

/* Recording callback: stores each applied item pointer in call order. */
static void	recordItem(void *item)
{
    if (gCallCount < MAX_CALLS)
        gItems[gCallCount] = item;
    gCallCount++;
}

/* Asserts exactly `count` calls whose items, read as strings, match
** `expected` in order. */
static void	assertCallOrder(const char **expected, int count)
{
    int	i;

    TEST_ASSERT_EQUAL_INT(count, gCallCount);
    i = 0;
    while (i < count)
    {
        TEST_ASSERT_EQUAL_STRING(expected[i], (char *)gItems[i]);
        i++;
    }
}

void	glorpUp(void)
{
    int	i;

    gCallCount = 0;
    i = 0;
    while (i < MAX_CALLS)
    {
        gItems[i] = NULL;
        i++;
    }
}

void	glorpDown(void)
{
}

/* A single node gets exactly one call, with its item pointer as given. */
static void	testSuffixSingleNode(void)
{
    char	item[] = "1";
    t_btree	leaf = {NULL, NULL, item};

    btree_apply_suffix(&leaf, recordItem);
    TEST_ASSERT_EQUAL_INT(1, gCallCount);
    TEST_ASSERT_EQUAL_PTR(item, gItems[0]);
}

/* Full 3-level tree shaped so suffix order (left, right, node) is "1".."7":
**             "7"
**           /     \
**        "3"       "6"
**       /   \     /   \
**     "1"   "2" "4"   "5"
** (prefix would give 7 3 1 2 6 4 5, infix 1 3 2 7 4 6 5, and level-order
** 7 3 6 1 2 4 5, so the expected order is unambiguous.) */
static void	testSuffixSevenNodeTree(void)
{
    t_btree	n1 = {NULL, NULL, "1"};
    t_btree	n2 = {NULL, NULL, "2"};
    t_btree	n4 = {NULL, NULL, "4"};
    t_btree	n5 = {NULL, NULL, "5"};
    t_btree	n3 = {&n1, &n2, "3"};
    t_btree	n6 = {&n4, &n5, "6"};
    t_btree	n7 = {&n3, &n6, "7"};
    const char	*expected[7] = {"1", "2", "3", "4", "5", "6", "7"};

    btree_apply_suffix(&n7, recordItem);
    assertCallOrder(expected, 7);
}

/* A NULL root applies the function zero times. */
static void	testSuffixNullRootZeroCalls(void)
{
    btree_apply_suffix(NULL, recordItem);
    TEST_ASSERT_EQUAL_INT(0, gCallCount);
}

void	glorpRun(void)
{
    RUN_TEST(testSuffixSingleNode);
    RUN_TEST(testSuffixSevenNodeTree);
    RUN_TEST(testSuffixNullRootZeroCalls);
}
