/*
** c08_ex05 -- ft_show_tab: void ft_show_tab(struct s_stock_str *par);
** Prints, for every element of the array up to the terminator (str == 0):
** the string str, a newline, size in decimal, a newline, the string copy and
** a newline, all on stdout. The struct is redefined here field-for-field; the
** test feeds a hand-built table, so it does not depend on ft_strs_to_tab.
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>

typedef struct s_stock_str
{
    int		size;
    char	*str;
    char	*copy;
}	t_stock_str;

extern void	ft_show_tab(struct s_stock_str *par);

/* Runs ft_show_tab(par) with stdout captured into out. */
static int	captureShowTab(t_stock_str *par, char *out, int cap)
{
    int	pfd[2];
    int	saved;
    int	n;

    out[0] = '\0';
    saved = dup(STDOUT_FILENO);
    if (saved == -1)
        return (-1);
    if (pipe(pfd) == -1)
    {
        close(saved);
        return (-1);
    }
    dup2(pfd[1], STDOUT_FILENO);
    close(pfd[1]);
    ft_show_tab(par);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    return (n);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* One element: str, size and copy each on their own line. */
static void	testShowTabSingle(void)
{
    t_stock_str	tab[2];
    char		out[256];

    tab[0].size = 5;
    tab[0].str = "Hello";
    tab[0].copy = "Hello";
    tab[1].size = 0;
    tab[1].str = 0;
    tab[1].copy = 0;
    captureShowTab(tab, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("Hello\n5\nHello\n", out);
}

/* Several elements print back to back, in order. */
static void	testShowTabSeveral(void)
{
    t_stock_str	tab[4];
    char		out[512];

    tab[0].size = 5;
    tab[0].str = "Hello";
    tab[0].copy = "Hello";
    tab[1].size = 8;
    tab[1].str = "42 rocks";
    tab[1].copy = "42 rocks";
    tab[2].size = 12;
    tab[2].str = "multi digit!";
    tab[2].copy = "multi digit!";
    tab[3].size = 0;
    tab[3].str = 0;
    tab[3].copy = 0;
    captureShowTab(tab, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING(
        "Hello\n5\nHello\n42 rocks\n8\n42 rocks\nmulti digit!\n12\nmulti digit!\n",
        out);
}

/* An empty string element prints two empty lines around its 0 size. */
static void	testShowTabEmptyString(void)
{
    t_stock_str	tab[2];
    char		out[128];

    tab[0].size = 0;
    tab[0].str = "";
    tab[0].copy = "";
    tab[1].size = 0;
    tab[1].str = 0;
    tab[1].copy = 0;
    captureShowTab(tab, out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("\n0\n\n", out);
}

/* A table with only the terminator prints nothing. */
static void	testShowTabEmptyTable(void)
{
    t_stock_str	tab[1];
    char		out[64];
    int			n;

    tab[0].size = 0;
    tab[0].str = 0;
    tab[0].copy = 0;
    n = captureShowTab(tab, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(0, n);
}

void	glorpRun(void)
{
    RUN_TEST(testShowTabSingle);
    RUN_TEST(testShowTabSeveral);
    RUN_TEST(testShowTabEmptyString);
    RUN_TEST(testShowTabEmptyTable);
}
