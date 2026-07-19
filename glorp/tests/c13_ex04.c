/*
** c13_ex04 -- btree_insert_data: void btree_insert_data(t_btree **root,
** void *item, int (*cmpf)(void *, void *));
** Allocates a node for item (as btree_create_node does) and inserts it into
** the binary search tree ordered by cmpf (strcmp model: negative = less),
** so smaller items go left and larger items go right.
*/

#include "runner.h"
#include <stdlib.h>
#include <string.h>

typedef struct s_btree
{
    struct s_btree	*left;
    struct s_btree	*right;
    void			*item;
}	t_btree;

extern void	btree_insert_data(t_btree **root, void *item,
        int (*cmpf)(void *, void *));

#define MAX_CALLS 16
#define KEY_COUNT 7

/* Distinct keys in scrambled insertion order, plus the index of each key in
** ascending strcmp order (apple banana grape kiwi mango pear zebra). */
static const char	*gKeys[KEY_COUNT] = {"mango", "apple", "zebra", "kiwi",
    "pear", "banana", "grape"};
static const int	gSortedIdx[KEY_COUNT] = {1, 5, 6, 3, 0, 4, 2};

static void	*gItems[MAX_CALLS];
static int	gCallCount;

/* strcmp adapter matching the cmpf prototype. */
static int	cmpStrings(void *a, void *b)
{
    return (strcmp((const char *)a, (const char *)b));
}

/* Our own reference walk: records every item of the tree in infix order. */
static void	recordInfix(t_btree *root)
{
    if (!root)
        return ;
    recordInfix(root->left);
    if (gCallCount < MAX_CALLS)
        gItems[gCallCount] = root->item;
    gCallCount++;
    recordInfix(root->right);
}

/* Frees every node of the tree. */
static void	freeTree(t_btree *root)
{
    if (!root)
        return ;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
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

/* Inserting into an empty tree makes *root a single node holding item (the
** same pointer), with both children NULL. */
static void	testInsertIntoEmptyTree(void)
{
    t_btree	*root;
    char	item[] = "m";
    void	*storedItem;
    t_btree	*left;
    t_btree	*right;

    root = NULL;
    btree_insert_data(&root, item, cmpStrings);
    TEST_ASSERT_NOT_NULL(root);
    storedItem = root->item;
    left = root->left;
    right = root->right;
    freeTree(root);
    TEST_ASSERT_EQUAL_PTR(item, storedItem);
    TEST_ASSERT_NULL(left);
    TEST_ASSERT_NULL(right);
}

/* Per cmpf, a smaller item becomes the left child of the root and a larger
** one the right child. */
static void	testInsertSmallerLeftLargerRight(void)
{
    t_btree	*root;
    char	mid[] = "m";
    char	low[] = "a";
    char	high[] = "z";
    void	*rootItem;
    void	*leftItem;
    void	*rightItem;

    root = NULL;
    btree_insert_data(&root, mid, cmpStrings);
    btree_insert_data(&root, low, cmpStrings);
    btree_insert_data(&root, high, cmpStrings);
    TEST_ASSERT_NOT_NULL(root);
    rootItem = root->item;
    leftItem = NULL;
    rightItem = NULL;
    if (root->left)
        leftItem = root->left->item;
    if (root->right)
        rightItem = root->right->item;
    freeTree(root);
    TEST_ASSERT_EQUAL_PTR(mid, rootItem);
    TEST_ASSERT_EQUAL_PTR(low, leftItem);
    TEST_ASSERT_EQUAL_PTR(high, rightItem);
}

/* Seven distinct keys inserted in scrambled order: our own infix walk must
** see all of them (same pointers), in ascending strcmp order, once each. */
static void	testInsertScrambledKeysGiveSortedInfix(void)
{
    t_btree	*root;
    int		i;

    root = NULL;
    i = 0;
    while (i < KEY_COUNT)
    {
        btree_insert_data(&root, (void *)gKeys[i], cmpStrings);
        i++;
    }
    recordInfix(root);
    freeTree(root);
    TEST_ASSERT_EQUAL_INT(KEY_COUNT, gCallCount);
    i = 0;
    while (i < KEY_COUNT)
    {
        TEST_ASSERT_EQUAL_STRING(gKeys[gSortedIdx[i]], (char *)gItems[i]);
        TEST_ASSERT_EQUAL_PTR(gKeys[gSortedIdx[i]], gItems[i]);
        i++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testInsertIntoEmptyTree);
    RUN_TEST(testInsertSmallerLeftLargerRight);
    RUN_TEST(testInsertScrambledKeysGiveSortedInfix);
}
