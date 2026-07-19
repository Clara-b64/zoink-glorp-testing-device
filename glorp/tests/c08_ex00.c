/*
** c08_ex00 -- ft.h: a HEADER that declares the five prototypes below and is
** protected against double inclusion. Built via the header-test path (compiled
** with -I<piscine ex00 dir>); there are no function definitions to link, so
** the whole check happens at compile time. Each _Static_assert takes the
** function's address inside _Generic (an unevaluated context, so no symbol
** reference is emitted): a missing declaration fails as an undeclared
** identifier, a wrong prototype fails the _Generic type match. The header is
** included twice so a broken include guard that ever declares a conflicting
** object would surface too.
*/

#include "runner.h"
#include "ft.h"
#include "ft.h"

_Static_assert(_Generic(&ft_putchar, void (*)(char): 1, default: 0),
    "ft.h must declare void ft_putchar(char c)");
_Static_assert(_Generic(&ft_swap, void (*)(int *, int *): 1, default: 0),
    "ft.h must declare void ft_swap(int *a, int *b)");
_Static_assert(_Generic(&ft_putstr, void (*)(char *): 1, default: 0),
    "ft.h must declare void ft_putstr(char *str)");
_Static_assert(_Generic(&ft_strlen, int (*)(char *): 1, default: 0),
    "ft.h must declare int ft_strlen(char *str)");
_Static_assert(_Generic(&ft_strcmp, int (*)(char *, char *): 1, default: 0),
    "ft.h must declare int ft_strcmp(char *s1, char *s2)");

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Compiling this file at all is the test: the header included twice, plus the
** five prototypes presence- and type-checked at compile time. */
static void	testHeaderCompiles(void)
{
    TEST_ASSERT_TRUE(1);
}

void	glorpRun(void)
{
    RUN_TEST(testHeaderCompiles);
}
