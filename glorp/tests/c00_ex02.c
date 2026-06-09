/*
** c00_ex02 -- ft_print_reverse_alphabet: void ft_print_reverse_alphabet(void);
** Writes the lowercase alphabet in reverse order, on one line, to stdout (fd 1).
*/

#include "runner.h"
#include <unistd.h>

extern void	ft_print_reverse_alphabet(void);

/* Runs ft_print_reverse_alphabet() with fd 1 redirected into a pipe, restores
** stdout, then returns the bytes emitted via `out` and their count (-1 on
** failure). */
static int	captureReverseAlphabet(char *out, int cap)
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
    ft_print_reverse_alphabet();
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

/* The whole output is exactly the reversed 26-letter lowercase alphabet. */
static void	testReverseAlphabetOutputsTheReversedAlphabet(void)
{
    char	out[32];
    int		n;

    n = captureReverseAlphabet(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(26, n);
    TEST_ASSERT_EQUAL_STRING("zyxwvutsrqponmlkjihgfedcba", out);
}

/* Exactly 26 bytes: no trailing newline and no extra characters. */
static void	testReverseAlphabetWritesExactlyTwentySixBytes(void)
{
    char	out[32];
    int		n;

    n = captureReverseAlphabet(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(26, n);
}

/* First byte is 'z', last byte is 'a'. */
static void	testReverseAlphabetStartsAtZEndsAtA(void)
{
    char	out[32];
    int		n;

    n = captureReverseAlphabet(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(26, n);
    TEST_ASSERT_EQUAL_CHAR('z', out[0]);
    TEST_ASSERT_EQUAL_CHAR('a', out[25]);
}

/* Letters are consecutive and in descending order. */
static void	testReverseAlphabetIsInReverseOrder(void)
{
    char	out[32];
    int		n;
    int		i;

    n = captureReverseAlphabet(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(26, n);
    i = 0;
    while (i < 26)
    {
        TEST_ASSERT_EQUAL_CHAR('z' - i, out[i]);
        i++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testReverseAlphabetOutputsTheReversedAlphabet);
    RUN_TEST(testReverseAlphabetWritesExactlyTwentySixBytes);
    RUN_TEST(testReverseAlphabetStartsAtZEndsAtA);
    RUN_TEST(testReverseAlphabetIsInReverseOrder);
}
