/*
** c12_ex07 -- ft_list_at: t_list *ft_list_at(t_list *begin_list,
** unsigned int nbr);
** Returns the nbr-th link of the list counting from 0, or NULL (0) when the
** index is out of range.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern t_list	*ft_list_at(t_list *begin_list, unsigned int nbr);

static t_list	*gFirst;
static t_list	*gSecond;
static t_list	*gThird;

/* Allocates one list node holding data (test-owned, plain malloc). */
static t_list	*listNew(void *data)
{
    t_list	*node;

    node = malloc(sizeof(t_list));
    TEST_ASSERT_NOT_NULL(node);
    node->data = data;
    node->next = NULL;
    return (node);
}

void	glorpUp(void)
{
    gFirst = listNew("one");
    gSecond = listNew("two");
    gThird = listNew("three");
    gFirst->next = gSecond;
    gSecond->next = gThird;
}

void	glorpDown(void)
{
    free(gFirst);
    free(gSecond);
    free(gThird);
    gFirst = NULL;
    gSecond = NULL;
    gThird = NULL;
}

/* Index 0 is the first link. */
static void	testAtIndexZeroIsFirstNode(void)
{
    TEST_ASSERT_EQUAL_PTR(gFirst, ft_list_at(gFirst, 0));
}

/* Index 1 is the second link. */
static void	testAtIndexOneIsSecondNode(void)
{
    TEST_ASSERT_EQUAL_PTR(gSecond, ft_list_at(gFirst, 1));
}

/* Index 2 is the third link. */
static void	testAtIndexTwoIsThirdNode(void)
{
    TEST_ASSERT_EQUAL_PTR(gThird, ft_list_at(gFirst, 2));
}

/* An index equal to the list size is out of range -> NULL. */
static void	testAtIndexEqualToSizeIsNull(void)
{
    TEST_ASSERT_NULL(ft_list_at(gFirst, 3));
}

/* A huge index is out of range -> NULL. */
static void	testAtHugeIndexIsNull(void)
{
    TEST_ASSERT_NULL(ft_list_at(gFirst, 1000000u));
}

void	glorpRun(void)
{
    RUN_TEST(testAtIndexZeroIsFirstNode);
    RUN_TEST(testAtIndexOneIsSecondNode);
    RUN_TEST(testAtIndexTwoIsThirdNode);
    RUN_TEST(testAtIndexEqualToSizeIsNull);
    RUN_TEST(testAtHugeIndexIsNull);
}
