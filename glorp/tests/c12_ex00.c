/*
** c12_ex00 -- ft_create_elem: t_list *ft_create_elem(void *data);
** Returns a freshly allocated element: data holds the given pointer untouched
** (NULL allowed) and next is NULL.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern t_list	*ft_create_elem(void *data);

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Returns a non-NULL freshly allocated element. */
static void	testCreateElemReturnsNonNull(void)
{
    t_list	*elem;

    elem = ft_create_elem("glorp");
    TEST_ASSERT_NOT_NULL(elem);
    free(elem);
}

/* data is exactly the pointer that was passed in. */
static void	testCreateElemStoresDataPointer(void)
{
    char	*str;
    t_list	*elem;

    str = "hello world";
    elem = ft_create_elem(str);
    TEST_ASSERT_NOT_NULL(elem);
    TEST_ASSERT_EQUAL_PTR(str, elem->data);
    free(elem);
}

/* NULL data is stored untouched. */
static void	testCreateElemAcceptsNullData(void)
{
    t_list	*elem;

    elem = ft_create_elem(NULL);
    TEST_ASSERT_NOT_NULL(elem);
    TEST_ASSERT_NULL(elem->data);
    free(elem);
}

/* next of a fresh element is NULL. */
static void	testCreateElemNextIsNull(void)
{
    t_list	*elem;

    elem = ft_create_elem("data");
    TEST_ASSERT_NOT_NULL(elem);
    TEST_ASSERT_NULL(elem->next);
    free(elem);
}

/* Two calls hand back two distinct elements, each with its own data. */
static void	testCreateElemTwoCallsAreDistinct(void)
{
    char	*one;
    char	*two;
    t_list	*first;
    t_list	*second;

    one = "one";
    two = "two";
    first = ft_create_elem(one);
    second = ft_create_elem(two);
    TEST_ASSERT_NOT_NULL(first);
    TEST_ASSERT_NOT_NULL(second);
    TEST_ASSERT_TRUE(first != second);
    TEST_ASSERT_EQUAL_PTR(one, first->data);
    TEST_ASSERT_EQUAL_PTR(two, second->data);
    free(first);
    free(second);
}

void	glorpRun(void)
{
    RUN_TEST(testCreateElemReturnsNonNull);
    RUN_TEST(testCreateElemStoresDataPointer);
    RUN_TEST(testCreateElemAcceptsNullData);
    RUN_TEST(testCreateElemNextIsNull);
    RUN_TEST(testCreateElemTwoCallsAreDistinct);
}
