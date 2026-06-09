/*
** c00_ex03 -- ft_print_numbers: void ft_print_numbers(void);
** Writes the digits 0 to 9 in order, on one line, to stdout (fd 1).
*/

#include "runner.h"
#include <unistd.h>

extern void	ft_print_numbers(void);

/* Runs ft_print_numbers() with fd 1 redirected into a pipe, restores stdout,
** then returns the bytes emitted via `out` and their count (-1 on failure). */
static int	capturePrintNumbers(char *out, int cap)
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
    ft_print_numbers();
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

/* The whole output is exactly the 10 digits "0123456789". */
static void	testPrintNumbersOutputsAllTenDigits(void)
{
    char	out[16];
    int		n;

    n = capturePrintNumbers(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(10, n);
    TEST_ASSERT_EQUAL_STRING("0123456789", out);
}

/* Exactly 10 bytes: no trailing newline and no extra characters. */
static void	testPrintNumbersWritesExactlyTenBytes(void)
{
    char	out[16];
    int		n;

    n = capturePrintNumbers(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(10, n);
}

/* First byte is '0', last byte is '9'. */
static void	testPrintNumbersStartsAtZeroEndsAtNine(void)
{
    char	out[16];
    int		n;

    n = capturePrintNumbers(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(10, n);
    TEST_ASSERT_EQUAL_CHAR('0', out[0]);
    TEST_ASSERT_EQUAL_CHAR('9', out[9]);
}

/* Digits are consecutive and in ascending order. */
static void	testPrintNumbersIsInOrder(void)
{
    char	out[16];
    int		n;
    int		i;

    n = capturePrintNumbers(out, sizeof(out));
    TEST_ASSERT_EQUAL_INT(10, n);
    i = 0;
    while (i < 10)
    {
        TEST_ASSERT_EQUAL_CHAR('0' + i, out[i]);
        i++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testPrintNumbersOutputsAllTenDigits);
    RUN_TEST(testPrintNumbersWritesExactlyTenBytes);
    RUN_TEST(testPrintNumbersStartsAtZeroEndsAtNine);
    RUN_TEST(testPrintNumbersIsInOrder);
}
