/*
** c12_ex06 -- ft_list_clear: void ft_list_clear(t_list *begin_list,
** void (*free_fct)(void *));
** Removes and frees every element of the list: free_fct is applied to each
** element's data and the links themselves are freed with free(). The nodes
** are NOT freed by the test afterwards -- the leak checker catches any node
** the function failed to free.
*/

#include "runner.h"
#include <stdlib.h>

typedef struct s_list
{
    struct s_list	*next;
    void			*data;
}	t_list;

extern void	ft_list_clear(t_list *begin_list, void (*free_fct)(void *));

#define RECORD_CAP 8

static int	gFreeCount;
static void	*gFreedData[RECORD_CAP];

/* free_fct under test: records the data pointer it was given, then really
** frees it. */
static void	countingFree(void *data)
{
    if (gFreeCount < RECORD_CAP)
        gFreedData[gFreeCount] = data;
    gFreeCount++;
    free(data);
}

/* How many times countingFree saw this exact pointer. */
static int	recordedCount(void *data)
{
    int	count;
    int	i;

    count = 0;
    i = 0;
    while (i < gFreeCount && i < RECORD_CAP)
    {
        if (gFreedData[i] == data)
            count++;
        i++;
    }
    return (count);
}

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

/* Malloc'd copy of src, so free_fct has something real to free. */
static char	*dupStr(const char *src)
{
    char	*copy;
    int		i;

    i = 0;
    while (src[i])
        i++;
    copy = malloc(i + 1);
    TEST_ASSERT_NOT_NULL(copy);
    i = 0;
    while (src[i])
    {
        copy[i] = src[i];
        i++;
    }
    copy[i] = '\0';
    return (copy);
}

void	glorpUp(void)
{
    int	i;

    gFreeCount = 0;
    i = 0;
    while (i < RECORD_CAP)
    {
        gFreedData[i] = NULL;
        i++;
    }
}

void	glorpDown(void)
{
}

/* Clearing a 1-element list applies free_fct to its data exactly once. */
static void	testClearOneElementList(void)
{
    char	*data;
    t_list	*list;

    data = dupStr("lonely");
    list = listNew(data);
    ft_list_clear(list, countingFree);
    TEST_ASSERT_EQUAL_INT(1, gFreeCount);
    TEST_ASSERT_EQUAL_INT(1, recordedCount(data));
}

/* Clearing a 3-element list runs free_fct exactly 3 times and hands it
** exactly the 3 data pointers of the list. */
static void	testClearThreeElementList(void)
{
    char	*dataOne;
    char	*dataTwo;
    char	*dataThree;
    t_list	*list;

    dataOne = dupStr("one");
    dataTwo = dupStr("two");
    dataThree = dupStr("three");
    list = listNew(dataOne);
    list->next = listNew(dataTwo);
    list->next->next = listNew(dataThree);
    ft_list_clear(list, countingFree);
    TEST_ASSERT_EQUAL_INT(3, gFreeCount);
    TEST_ASSERT_EQUAL_INT(1, recordedCount(dataOne));
    TEST_ASSERT_EQUAL_INT(1, recordedCount(dataTwo));
    TEST_ASSERT_EQUAL_INT(1, recordedCount(dataThree));
}

void	glorpRun(void)
{
    RUN_TEST(testClearOneElementList);
    RUN_TEST(testClearThreeElementList);
}
