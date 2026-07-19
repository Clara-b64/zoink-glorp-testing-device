/*
** c13_ex02 -- btree_apply_infix:
** void btree_apply_infix(t_btree *root, void (*applyf)(void *));
** Applies applyf to every item using infix traversal: the left subtree
** first, then the current node, then the right subtree.
*/

#include "runner.h"

typedef struct s_btree
{
    struct s_btree	*left;
    struct s_btree	*right;
    void			*item;
}	t_btree;

extern void	btree_apply_infix(t_btree *root, void (*applyf)(void *));

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
static void	testInfixSingleNode(void)
{
    char	item[] = "1";
    t_btree	leaf = {NULL, NULL, item};

    btree_apply_infix(&leaf, recordItem);
    TEST_ASSERT_EQUAL_INT(1, gCallCount);
    TEST_ASSERT_EQUAL_PTR(item, gItems[0]);
}

/* Full 3-level tree shaped so infix order (left, node, right) is "1".."7":
**             "4"
**           /     \
**        "2"       "6"
**       /   \     /   \
**     "1"   "3" "5"   "7"
** (prefix would give 4 2 1 3 6 5 7, suffix 1 3 2 5 7 6 4, and level-order
** 4 2 6 1 3 5 7, so the expected order is unambiguous.) */
static void	testInfixSevenNodeTree(void)
{
    t_btree	n1 = {NULL, NULL, "1"};
    t_btree	n3 = {NULL, NULL, "3"};
    t_btree	n5 = {NULL, NULL, "5"};
    t_btree	n7 = {NULL, NULL, "7"};
    t_btree	n2 = {&n1, &n3, "2"};
    t_btree	n6 = {&n5, &n7, "6"};
    t_btree	n4 = {&n2, &n6, "4"};
    const char	*expected[7] = {"1", "2", "3", "4", "5", "6", "7"};

    btree_apply_infix(&n4, recordItem);
    assertCallOrder(expected, 7);
}

/* A NULL root applies the function zero times. */
static void	testInfixNullRootZeroCalls(void)
{
    btree_apply_infix(NULL, recordItem);
    TEST_ASSERT_EQUAL_INT(0, gCallCount);
}

void	glorpRun(void)
{
    RUN_TEST(testInfixSingleNode);
    RUN_TEST(testInfixSevenNodeTree);
    RUN_TEST(testInfixNullRootZeroCalls);
}
