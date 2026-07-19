/*
** c13_ex05 -- btree_search_item: void *btree_search_item(t_btree *root,
** void *data_ref, int (*cmpf)(void *, void *));
** Searches the tree by infix traversal and returns the first item for which
** cmpf(item, data_ref) == 0, or NULL if none. The tree is any binary tree
** (not necessarily a BST); infix order decides which match is "first".
*/

#include "runner.h"
#include <string.h>

typedef struct s_btree
{
    struct s_btree	*left;
    struct s_btree	*right;
    void			*item;
}	t_btree;

extern void	*btree_search_item(t_btree *root, void *data_ref,
        int (*cmpf)(void *, void *));

/* strcmp adapter matching the cmpf prototype. */
static int	cmpStrings(void *a, void *b)
{
    return (strcmp((const char *)a, (const char *)b));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A single matching node returns its item pointer (not data_ref). */
static void	testSearchSingleNodeFindsItem(void)
{
    char	item[] = "hello";
    char	ref[] = "hello";
    t_btree	leaf = {NULL, NULL, item};

    TEST_ASSERT_EQUAL_PTR(item, btree_search_item(&leaf, ref, cmpStrings));
}

/* Non-BST tree with three items that all compare equal to "42" but live in
** distinct buffers, so pointer identity tells them apart:
**              X(dup3)
**            /        \
**       L(dup1)        R("c")
**       /    \         /    \
**   LL("a")  LR(dup2) RL("b") RR("d")
** Infix order is LL L LR X RL R RR, so dup1 is the first match. Prefix
** would meet dup3 first, suffix dup2 first, and a BST-style descent stops
** at the root's dup3 -- only the infix-first pointer dup1 is correct. */
static void	testSearchReturnsInfixFirstMatch(void)
{
    char	dup1[] = "42";
    char	dup2[] = "42";
    char	dup3[] = "42";
    char	ref[] = "42";
    t_btree	ll = {NULL, NULL, "a"};
    t_btree	lr = {NULL, NULL, dup2};
    t_btree	rl = {NULL, NULL, "b"};
    t_btree	rr = {NULL, NULL, "d"};
    t_btree	l = {&ll, &lr, dup1};
    t_btree	r = {&rl, &rr, "c"};
    t_btree	x = {&l, &r, dup3};
    void	*found;

    found = btree_search_item(&x, ref, cmpStrings);
    TEST_ASSERT_NOT_NULL(found);
    TEST_ASSERT_EQUAL_STRING("42", (char *)found);
    TEST_ASSERT_EQUAL_PTR(dup1, found);
}

/* No item matches: the result is NULL. */
static void	testSearchNoMatchReturnsNull(void)
{
    t_btree	leftLeaf = {NULL, NULL, "alpha"};
    t_btree	rightLeaf = {NULL, NULL, "gamma"};
    t_btree	root = {&leftLeaf, &rightLeaf, "beta"};
    char	ref[] = "delta";

    TEST_ASSERT_NULL(btree_search_item(&root, ref, cmpStrings));
}

/* A NULL root returns NULL. */
static void	testSearchNullRootReturnsNull(void)
{
    char	ref[] = "42";

    TEST_ASSERT_NULL(btree_search_item(NULL, ref, cmpStrings));
}

void	glorpRun(void)
{
    RUN_TEST(testSearchSingleNodeFindsItem);
    RUN_TEST(testSearchReturnsInfixFirstMatch);
    RUN_TEST(testSearchNoMatchReturnsNull);
    RUN_TEST(testSearchNullRootReturnsNull);
}
