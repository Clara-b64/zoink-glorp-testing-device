/*
** c13_ex07 -- btree_apply_by_level: void btree_apply_by_level(t_btree *root,
** void (*applyf)(void *item, int current_level, int is_first_elem));
** Applies applyf to every item in level order (breadth-first): the root
** first (current_level 0), then level 1 left to right, then level 2, etc.
** is_first_elem is 1 for the first item applied of its level, else 0.
*/

#include "runner.h"

typedef struct s_btree
{
    struct s_btree	*left;
    struct s_btree	*right;
    void			*item;
}	t_btree;

extern void	btree_apply_by_level(t_btree *root,
        void (*applyf)(void *item, int current_level, int is_first_elem));

#define MAX_CALLS 16

static void	*gItems[MAX_CALLS];
static int	gLevels[MAX_CALLS];
static int	gFirsts[MAX_CALLS];
static int	gCallCount;

/* Recording callback: stores item, level and first-flag in call order. */
static void	recordCall(void *item, int currentLevel, int isFirstElem)
{
    if (gCallCount < MAX_CALLS)
    {
        gItems[gCallCount] = item;
        gLevels[gCallCount] = currentLevel;
        gFirsts[gCallCount] = isFirstElem;
    }
    gCallCount++;
}

void	glorpUp(void)
{
    int	i;

    gCallCount = 0;
    i = 0;
    while (i < MAX_CALLS)
    {
        gItems[i] = NULL;
        gLevels[i] = -1;
        gFirsts[i] = -1;
        i++;
    }
}

void	glorpDown(void)
{
}

/* A single node yields exactly one call: (item, level 0, is_first 1). */
static void	testByLevelSingleNode(void)
{
    char	item[] = "1";
    t_btree	leaf = {NULL, NULL, item};

    btree_apply_by_level(&leaf, recordCall);
    TEST_ASSERT_EQUAL_INT(1, gCallCount);
    TEST_ASSERT_EQUAL_PTR(item, gItems[0]);
    TEST_ASSERT_EQUAL_INT(0, gLevels[0]);
    TEST_ASSERT_EQUAL_INT(1, gFirsts[0]);
}

/* Full 3-level tree shaped so breadth-first order is "1".."7":
**             "1"
**           /     \
**        "2"       "3"
**       /   \     /   \
**     "4"   "5" "6"   "7"
** Expected calls: 1@L0 first, 2@L1 first, 3@L1, 4@L2 first, 5@L2, 6@L2,
** 7@L2. (Prefix order would be 1 2 4 5 3 6 7, so BFS is unambiguous.) */
static void	testByLevelSevenNodeTree(void)
{
    t_btree	n4 = {NULL, NULL, "4"};
    t_btree	n5 = {NULL, NULL, "5"};
    t_btree	n6 = {NULL, NULL, "6"};
    t_btree	n7 = {NULL, NULL, "7"};
    t_btree	n2 = {&n4, &n5, "2"};
    t_btree	n3 = {&n6, &n7, "3"};
    t_btree	n1 = {&n2, &n3, "1"};
    static const char	*expectedItems[7] = {"1", "2", "3", "4", "5", "6",
        "7"};
    static const int	expectedLevels[7] = {0, 1, 1, 2, 2, 2, 2};
    static const int	expectedFirsts[7] = {1, 1, 0, 1, 0, 0, 0};
    int	i;

    btree_apply_by_level(&n1, recordCall);
    TEST_ASSERT_EQUAL_INT(7, gCallCount);
    i = 0;
    while (i < 7)
    {
        TEST_ASSERT_EQUAL_STRING(expectedItems[i], (char *)gItems[i]);
        TEST_ASSERT_EQUAL_INT(expectedLevels[i], gLevels[i]);
        TEST_ASSERT_EQUAL_INT(expectedFirsts[i], gFirsts[i]);
        i++;
    }
}

/* A NULL root applies the function zero times. */
static void	testByLevelNullRootZeroCalls(void)
{
    btree_apply_by_level(NULL, recordCall);
    TEST_ASSERT_EQUAL_INT(0, gCallCount);
}

void	glorpRun(void)
{
    RUN_TEST(testByLevelSingleNode);
    RUN_TEST(testByLevelSevenNodeTree);
    RUN_TEST(testByLevelNullRootZeroCalls);
}
