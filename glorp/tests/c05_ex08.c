/*
** c05_ex08 -- ft_ten_queens_puzzle: int ft_ten_queens_puzzle(void);
** Prints every way to place 10 non-attacking queens on a 10x10 board and
** returns the count (724). Each solution is one line of 10 digits + '\n', where
** the digit at position i is the row (0-9) of the queen in column i. Solutions
** come out in column-major DFS order (column 0 outermost, rows 0..9 ascending),
** i.e. lexicographically: first "0257948136", last "9742051863".
**
** The function writes to fd 1, so it is run with stdout redirected into a pipe
** and the bytes read back. The whole output is 724 * 11 = 7964 bytes, far under
** the pipe buffer, so the write-then-read capture cannot deadlock. The expected
** text is produced by an in-test reference using the same canonical search, so
** the comparison pins content, order and format at once.
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>

extern int	ft_ten_queens_puzzle(void);

/* Row/diagonal safety against the queens already placed in columns 0..col-1. */
static int	refSafe(int *board, int col, int row)
{
    int		i;
    int		d;

    i = 0;
    while (i < col)
    {
        d = col - i;
        if (board[i] == row || board[i] == row - d || board[i] == row + d)
            return (0);
        i++;
    }
    return (1);
}

/* Canonical backtracking: column outermost, rows 0..9 ascending. Appends each
** completed board as 10 digits + '\n' to out/len; returns the count found. */
static int	refSolve(int *board, int col, char *out, int *len)
{
    int		row;
    int		count;

    if (col == 10)
    {
        row = 0;
        while (row < 10)
        {
            out[(*len)++] = (char)(board[row] + '0');
            row++;
        }
        out[(*len)++] = '\n';
        return (1);
    }
    count = 0;
    row = 0;
    while (row < 10)
    {
        if (refSafe(board, col, row))
        {
            board[col] = row;
            count += refSolve(board, col + 1, out, len);
        }
        row++;
    }
    return (count);
}

/* Builds the full expected output (NUL-terminated) and returns the count. */
static int	refTenQueens(char *out)
{
    int		board[10];
    int		len;
    int		count;

    len = 0;
    count = refSolve(board, 0, out, &len);
    out[len] = '\0';
    return (count);
}

/* Runs ft_ten_queens_puzzle() with fd 1 redirected into a pipe; returns the
** captured byte count via the buffer, the function's return value via `ret`. */
static int	captureTenQueens(char *out, int cap, int *ret)
{
    int		pfd[2];
    int		saved;
    int		total;
    int		n;

    *ret = -1;
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
    *ret = ft_ten_queens_puzzle();
    dup2(saved, STDOUT_FILENO);
    close(saved);
    total = 0;
    n = (int)read(pfd[0], out + total, cap - 1 - total);
    while (n > 0)
    {
        total += n;
        if (total >= cap - 1)
            break ;
        n = (int)read(pfd[0], out + total, cap - 1 - total);
    }
    close(pfd[0]);
    out[total] = '\0';
    return (total);
}

/* Asserts one 11-byte line is a real solution: digits 0-9, no two queens on the
** same row or diagonal (columns are distinct by position). */
static void	assertValidQueensLine(const char *line)
{
    int		col;
    int		c2;
    int		r1;
    int		r2;

    col = 0;
    while (col < 10)
    {
        r1 = line[col] - '0';
        TEST_ASSERT_TRUE(r1 >= 0 && r1 <= 9);
        c2 = col + 1;
        while (c2 < 10)
        {
            r2 = line[c2] - '0';
            TEST_ASSERT_TRUE(r1 != r2);
            TEST_ASSERT_TRUE(c2 - col != r1 - r2);
            TEST_ASSERT_TRUE(c2 - col != r2 - r1);
            c2++;
        }
        col++;
    }
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* The return value is the number of solutions: 724. */
static void	testTenQueensReturnsSolutionCount(void)
{
    char	out[16384];
    int		ret;

    captureTenQueens(out, sizeof(out), &ret);
    TEST_ASSERT_EQUAL_INT(724, ret);
}

/* Printed output matches the reference exactly: content, order and format. */
static void	testTenQueensOutputMatchesReference(void)
{
    char	out[16384];
    char	expected[16384];
    int		ret;
    int		refCount;
    int		n;

    n = captureTenQueens(out, sizeof(out), &ret);
    refCount = refTenQueens(expected);
    TEST_ASSERT_EQUAL_INT(refCount, ret);
    TEST_ASSERT_EQUAL_INT((int)strlen(expected), n);
    TEST_ASSERT_EQUAL_STRING(expected, out);
}

/* Format anchored to the subject, independent of the reference solver. */
static void	testTenQueensFormatAnchors(void)
{
    char	out[16384];
    int		ret;
    int		n;
    int		i;

    n = captureTenQueens(out, sizeof(out), &ret);
    TEST_ASSERT_EQUAL_INT(724 * 11, n);
    TEST_ASSERT_EQUAL_INT(0, strncmp(out, "0257948136\n", 11));
    TEST_ASSERT_EQUAL_INT(0, strncmp(out + n - 11, "9742051863\n", 11));
    i = 0;
    while (i < n)
    {
        if ((i + 1) % 11 == 0)
            TEST_ASSERT_EQUAL_CHAR('\n', out[i]);
        else
            TEST_ASSERT_TRUE(out[i] >= '0' && out[i] <= '9');
        i++;
    }
}

/* Every printed line is a genuine non-attacking arrangement. */
static void	testTenQueensEverySolutionIsValid(void)
{
    char	out[16384];
    int		ret;
    int		n;
    int		i;

    n = captureTenQueens(out, sizeof(out), &ret);
    i = 0;
    while (i + 11 <= n)
    {
        assertValidQueensLine(out + i);
        i += 11;
    }
}

/* No solution is printed twice (so the 724 are the full, distinct set). */
static void	testTenQueensSolutionsAreDistinct(void)
{
    char	out[16384];
    int		ret;
    int		n;
    int		i;
    int		j;

    n = captureTenQueens(out, sizeof(out), &ret);
    i = 0;
    while (i + 11 <= n)
    {
        j = i + 11;
        while (j + 11 <= n)
        {
            TEST_ASSERT_TRUE(strncmp(out + i, out + j, 11) != 0);
            j += 11;
        }
        i += 11;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testTenQueensReturnsSolutionCount);
    RUN_TEST(testTenQueensOutputMatchesReference);
    RUN_TEST(testTenQueensFormatAnchors);
    RUN_TEST(testTenQueensEverySolutionIsValid);
    RUN_TEST(testTenQueensSolutionsAreDistinct);
}
