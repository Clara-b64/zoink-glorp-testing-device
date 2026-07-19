/*
** c13_ex06 -- btree_level_count: int btree_level_count(t_btree *root);
** Returns the number of levels of the tree (its maximum depth): 0 for a
** NULL tree, 1 for a single node, and 1 + the deeper subtree otherwise.
*/

#include "runner.h"

typedef struct s_btree
{
    struct s_btree	*left;
    struct s_btree	*right;
    void			*item;
}	t_btree;

extern int	btree_level_count(t_btree *root);

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A NULL tree has zero levels. */
static void	testLevelCountNullIsZero(void)
{
    TEST_ASSERT_EQUAL_INT(0, btree_level_count(NULL));
}

/* A single node is one level. */
static void	testLevelCountSingleNodeIsOne(void)
{
    t_btree	leaf = {NULL, NULL, "1"};

    TEST_ASSERT_EQUAL_INT(1, btree_level_count(&leaf));
}

/* A full, balanced 3-level tree (7 nodes) counts 3. */
static void	testLevelCountBalancedThreeLevels(void)
{
    t_btree	n3 = {NULL, NULL, "3"};
    t_btree	n4 = {NULL, NULL, "4"};
    t_btree	n6 = {NULL, NULL, "6"};
    t_btree	n7 = {NULL, NULL, "7"};
    t_btree	n2 = {&n3, &n4, "2"};
    t_btree	n5 = {&n6, &n7, "5"};
    t_btree	n1 = {&n2, &n5, "1"};

    TEST_ASSERT_EQUAL_INT(3, btree_level_count(&n1));
}

/* A degenerate all-left chain of 4 nodes counts 4. */
static void	testLevelCountLeftChainOfFour(void)
{
    t_btree	d = {NULL, NULL, "4"};
    t_btree	c = {&d, NULL, "3"};
    t_btree	b = {&c, NULL, "2"};
    t_btree	a = {&b, NULL, "1"};

    TEST_ASSERT_EQUAL_INT(4, btree_level_count(&a));
}

/* A degenerate all-right chain of 4 nodes counts 4. */
static void	testLevelCountRightChainOfFour(void)
{
    t_btree	d = {NULL, NULL, "4"};
    t_btree	c = {NULL, &d, "3"};
    t_btree	b = {NULL, &c, "2"};
    t_btree	a = {NULL, &b, "1"};

    TEST_ASSERT_EQUAL_INT(4, btree_level_count(&a));
}

void	glorpRun(void)
{
    RUN_TEST(testLevelCountNullIsZero);
    RUN_TEST(testLevelCountSingleNodeIsOne);
    RUN_TEST(testLevelCountBalancedThreeLevels);
    RUN_TEST(testLevelCountLeftChainOfFour);
    RUN_TEST(testLevelCountRightChainOfFour);
}
