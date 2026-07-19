/*
** c09_ex00 -- libft: the five functions the library must ship:
** ft_putchar, ft_putstr, ft_strcmp, ft_strlen, ft_swap. The exercise's real
** deliverable is libft_creator.sh, which this harness cannot run; what it can
** do is compile every .c in the exercise directory and check each function
** behaves. ft_strcmp is only sign-checked, like the c03_ex00 subject.
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>

extern void	ft_putchar(char c);
extern void	ft_putstr(char *str);
extern int	ft_strcmp(char *s1, char *s2);
extern int	ft_strlen(char *str);
extern void	ft_swap(int *a, int *b);

/* Captures stdout while action(arg) runs; the two writers below wrap the
** functions under test so one capture helper serves both. */
static int	captureCall(void (*action)(char *), char *arg, char *out, int cap)
{
    int	pfd[2];
    int	saved;
    int	n;

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
    action(arg);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    return (n);
}

static void	runPutchar(char *arg)
{
    ft_putchar(arg[0]);
}

static void	runPutstr(char *arg)
{
    ft_putstr(arg);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* ft_putchar writes exactly its one character. */
static void	testPutchar(void)
{
    char	out[8];
    int		n;

    n = captureCall(runPutchar, "x", out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_CHAR('x', out[0]);
    n = captureCall(runPutchar, "\n", out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_CHAR('\n', out[0]);
}

/* ft_putstr writes the string, nothing more (empty string writes nothing). */
static void	testPutstr(void)
{
    char	out[64];
    int		n;

    n = captureCall(runPutstr, "Hello, 42!", out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(10, n);
    TEST_ASSERT_EQUAL_STRING("Hello, 42!", out);
    n = captureCall(runPutstr, "", out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ft_strlen matches strlen. */
static void	testStrlen(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_strlen(""));
    TEST_ASSERT_EQUAL_INT(1, ft_strlen("a"));
    TEST_ASSERT_EQUAL_INT(5, ft_strlen("hello"));
    TEST_ASSERT_EQUAL_INT(11, ft_strlen("hello world"));
}

/* ft_strcmp: zero on equal, sign of the first difference otherwise. */
static void	testStrcmp(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_strcmp("", ""));
    TEST_ASSERT_EQUAL_INT(0, ft_strcmp("abc", "abc"));
    TEST_ASSERT_TRUE(ft_strcmp("abc", "abd") < 0);
    TEST_ASSERT_TRUE(ft_strcmp("abd", "abc") > 0);
    TEST_ASSERT_TRUE(ft_strcmp("ab", "abc") < 0);
    TEST_ASSERT_TRUE(ft_strcmp("abc", "ab") > 0);
}

/* ft_swap exchanges the two ints. */
static void	testSwap(void)
{
    int	a;
    int	b;

    a = 1;
    b = 2;
    ft_swap(&a, &b);
    TEST_ASSERT_EQUAL_INT(2, a);
    TEST_ASSERT_EQUAL_INT(1, b);
    a = -42;
    b = -42;
    ft_swap(&a, &b);
    TEST_ASSERT_EQUAL_INT(-42, a);
    TEST_ASSERT_EQUAL_INT(-42, b);
}

void	glorpRun(void)
{
    RUN_TEST(testPutchar);
    RUN_TEST(testPutstr);
    RUN_TEST(testStrlen);
    RUN_TEST(testStrcmp);
    RUN_TEST(testSwap);
}
