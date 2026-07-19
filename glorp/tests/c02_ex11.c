/*
** c02_ex11 -- ft_putstr_non_printable: void ft_putstr_non_printable(char *str);
** Writes str to stdout, replacing every non-printable character (anything
** outside 32..126) with a backslash followed by its two lowercase hex digits,
** e.g. "Coucou\ntu vas bien ?" prints as "Coucou\0atu vas bien ?". Bytes above
** 127 must be treated as unsigned (0x80 prints "\80", not sign-extended junk).
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>

extern void	ft_putstr_non_printable(char *str);

/* Runs ft_putstr_non_printable(str) with stdout redirected into a pipe and
** returns the captured bytes and their count (-1 on capture failure). */
static int	capturePutstr(char *str, char *out, int cap)
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
    ft_putstr_non_printable(str);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    return (n);
}

static void	assertOutput(char *input, char *expected)
{
    char	out[256];
    int		n;

    n = capturePutstr(input, out, sizeof(out));
    TEST_ASSERT_EQUAL_INT((int)strlen(expected), n);
    TEST_ASSERT_EQUAL_STRING(expected, out);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A fully printable string is echoed unchanged. */
static void	testPutstrPrintableUnchanged(void)
{
    assertOutput("Hello world!", "Hello world!");
    assertOutput(" ~", " ~");
}

/* The subject's own example. */
static void	testPutstrSubjectExample(void)
{
    assertOutput("Coucou\ntu vas bien ?", "Coucou\\0atu vas bien ?");
}

/* Common control characters become backslash plus two lowercase hex digits. */
static void	testPutstrControlChars(void)
{
    assertOutput("a\tb", "a\\09b");
    assertOutput("\n", "\\0a");
    assertOutput("bell\a", "bell\\07");
    assertOutput("\x01\x02", "\\01\\02");
}

/* 127 (DEL) is not printable. */
static void	testPutstrDelChar(void)
{
    assertOutput("x\x7fy", "x\\7fy");
}

/* Bytes above 127 print their unsigned hex value (no sign extension). */
static void	testPutstrHighBytes(void)
{
    assertOutput("\x80", "\\80");
    assertOutput("a\xffz", "a\\ffz");
}

/* An empty string prints nothing. */
static void	testPutstrEmpty(void)
{
    assertOutput("", "");
}

void	glorpRun(void)
{
    RUN_TEST(testPutstrPrintableUnchanged);
    RUN_TEST(testPutstrSubjectExample);
    RUN_TEST(testPutstrControlChars);
    RUN_TEST(testPutstrDelChar);
    RUN_TEST(testPutstrHighBytes);
    RUN_TEST(testPutstrEmpty);
}
