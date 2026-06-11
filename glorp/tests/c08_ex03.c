/*
** c08_ex03 -- ft_point.h: a HEADER defining the type t_point, a struct with int
** members x and y, so the provided code (set_point sets point->x / point->y, and
** main declares a t_point) compiles and runs. Using t_point and p.x / p.y in this
** test only compiles if the type and member names are correct; the runtime checks
** that the members store independent int values (INT_MAX / INT_MIN included).
**
** Built via the header-test path (-I<piscine ex03 dir>); the provided ft_point.c
** is never compiled.
*/

#include "runner.h"
#include "ft_point.h"
#include <limits.h>

/* Mirrors the subject's set_point: fills a point through a pointer. */
static void	fillPoint(t_point *point)
{
    point->x = 42;
    point->y = 21;
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The two members are assignable and read back the stored values. */
static void	testPointSetAndGetMembers(void)
{
    t_point	p;

    p.x = 42;
    p.y = 21;
    TEST_ASSERT_EQUAL_INT(42, p.x);
    TEST_ASSERT_EQUAL_INT(21, p.y);
}

/* x and y are independent fields. */
static void	testPointMembersAreIndependent(void)
{
    t_point	p;

    p.x = 5;
    p.y = 10;
    p.x = 7;
    TEST_ASSERT_EQUAL_INT(7, p.x);
    TEST_ASSERT_EQUAL_INT(10, p.y);
    p.y = -3;
    TEST_ASSERT_EQUAL_INT(7, p.x);
    TEST_ASSERT_EQUAL_INT(-3, p.y);
}

/* Setting the members through a pointer, exactly as set_point does. */
static void	testPointViaPointer(void)
{
    t_point	p;

    fillPoint(&p);
    TEST_ASSERT_EQUAL_INT(42, p.x);
    TEST_ASSERT_EQUAL_INT(21, p.y);
}

/* The members hold the full int range (so they are int, not a narrower type). */
static void	testPointHoldsIntRange(void)
{
    t_point	p;

    p.x = INT_MAX;
    p.y = INT_MIN;
    TEST_ASSERT_EQUAL_INT(INT_MAX, p.x);
    TEST_ASSERT_EQUAL_INT(INT_MIN, p.y);
    p.x = 0;
    p.y = -100000;
    TEST_ASSERT_EQUAL_INT(0, p.x);
    TEST_ASSERT_EQUAL_INT(-100000, p.y);
}

void	glorpRun(void)
{
    RUN_TEST(testPointSetAndGetMembers);
    RUN_TEST(testPointMembersAreIndependent);
    RUN_TEST(testPointViaPointer);
    RUN_TEST(testPointHoldsIntRange);
}
